#include "network.h"
#include "credentials.h"

#include "video/camera.h"

///////////////////////////////////
// GLOBAL VARIABLES
///////////////////////////////////

WiFiServer tcpServer;

///////////////////////////////////
// NETWORK SETUP
///////////////////////////////////

bool network_setup() {
    log_setup();
    
    int wifi_attempts = 0;

    // WiFi credentials
#ifdef WIFI_SSID
    const char *ssid = WIFI_SSID;
#else
    const char *ssid = "";
#endif

#ifdef WIFI_PASSWORD
    const char *password = WIFI_PASSWORD;
#else
    const char *password = "";
#endif

    log_n("Connecting to WiFi %s", ssid);

    WiFi.begin(ssid, password);

    while( (WiFi.status() != WL_CONNECTED) ){
        delay(500);
        log_printf(".");

        if(wifi_attempts++ >= NETWORK_MAX_WIFI_ATTEMPTS){
            log_printf(" FAILED !!!\n");
            return false;
        }
    }

    log_printf("OK!\n");
    log_n("IP address: %s", WiFi.localIP().toString());

    // Start TCP server
    tcpServer.begin(1337);
    return true;
}

///////////////////////////////////
// LOOP CODE for streaming
///////////////////////////////////

void camera_currframe_send_tcp(WiFiClient client) {
    camera_fb_t *fb = camera_get_frame();

    if (!fb) {
        log_e("Camera capture failed");
        return;
    }

    // TODO: Proper encoding

    // Send JPEG frame size first (4 bytes, little endian)
    uint32_t frameSize = fb->len;
    client.write((uint8_t *)&frameSize, 4);

    // Send JPEG frame data
    client.write(fb->buf, fb->len);
    camera_return_frame(fb);
}


// Function to handle client connection in a separate task
void handleClientTask(void *parameter) {
    WiFiClient *client = (WiFiClient *)parameter;
    
    // TODO: Authentication
    
    log_d("Sending camera stream to client %s", client->remoteIP().toString());
    
    while (client->connected()) {
        camera_currframe_send_tcp(*client);
        delay(100); // Adjust delay as needed
    }
    
    log_i("Client disconnected %s", client->remoteIP().toString());
    client->stop();
    delete client; // Free the client object
    vTaskDelete(NULL); // Delete this task when done
}

void network_update() {
    log_update();

    // Check for new client connections
    if (tcpServer.hasClient()) {

        WiFiClient* client = new WiFiClient(tcpServer.available());
        if (client && client->connected()) {
            log_i("New client connected %s", client->remoteIP().toString() );
            
            // Create a new task to handle this client
            xTaskCreate(
                handleClientTask,   // Task function
                "ClientHandler",   // Name of task
                4096,              // Stack size (bytes)
                (void*)client,  // Parameter to pass to function
                1,                 // Task priority
                NULL               // Task handle
            );

        } else {
            // Clean up if client connection failed
            if (client) {
                delete client;
            }
        }
    }
}
