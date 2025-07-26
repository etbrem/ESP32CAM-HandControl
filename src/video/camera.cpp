#include "utils/macros.h"

#include "camera.h"

void camera_config_setup(
    camera_config_t *config,
    framesize_t frame_size = FRAMESIZE_QVGA,  // FRAMESIZE_QVGA = 320x240, TODO: FRAMESIZE_96X96
    pixformat_t format_type = PIXFORMAT_JPEG, // TODO: PIXFORMAT_GRAYSCALE  (see ~/.platformio/packages/framework-arduinoespressif32/tools/sdk/esp32/include/esp32-camera/driver/include/sensor.h)
    int clock_frequency = 20000000  // 20 MHz
){
    config->ledc_channel = LEDC_CHANNEL_0;
    config->ledc_timer = LEDC_TIMER_0;
    config->pin_d0 = Y2_GPIO_NUM;
    config->pin_d1 = Y3_GPIO_NUM;
    config->pin_d2 = Y4_GPIO_NUM;
    config->pin_d3 = Y5_GPIO_NUM;
    config->pin_d4 = Y6_GPIO_NUM;
    config->pin_d5 = Y7_GPIO_NUM;
    config->pin_d6 = Y8_GPIO_NUM;
    config->pin_d7 = Y9_GPIO_NUM;
    config->pin_xclk = XCLK_GPIO_NUM;
    config->pin_pclk = PCLK_GPIO_NUM;
    config->pin_vsync = VSYNC_GPIO_NUM;
    config->pin_href = HREF_GPIO_NUM;
    config->pin_sccb_sda = SIOD_GPIO_NUM;
    config->pin_sccb_scl = SIOC_GPIO_NUM;
    config->pin_pwdn = PWDN_GPIO_NUM;
    config->pin_reset = RESET_GPIO_NUM;
    
    config->xclk_freq_hz = clock_frequency;
    config->pixel_format = format_type;

    // Frame parameters
    config->frame_size = frame_size;
    config->jpeg_quality = 12;
    config->fb_count = 2;
}

bool camera_setup() {
    log_setup();
    
    camera_config_t config;
    camera_config_setup(&config);

    // Initialize camera
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        log_e("Camera init failed with error 0x%x\n", err);
        log_e("Power: %d, Clock: %d, Data: %d\n", 
            config.pin_pwdn, config.pin_sccb_scl, config.pin_sccb_sda);
        return false;
    }

    log_d("Camera initialized");
    return true;
}

