#include "utils/macros.h"

#include "blink/sequences.h"
#include "blink/test.h"

#include "components/hand/hand.h"
#include "network/network.h"

// #include <pixelnova87-project-1-v3/edge-impulse-sdk/classifier/ei_run_classifier.h>

// Hand::Hand hand1;

void setup() {
    serial_setup(115200); // This must match your monitor_speed
    log_setup();

    blink_setup();
    blink_sequence_start(BlinkSequence_BOOT);
    blink_test();  // If BLINK_TEST is undefined this does nothing

    camera_setup();
    network_setup();

    // hand1.setup();

    blink_sequence_start(BlinkSequence_DARK);
}



void loop() {
    log_update();

    // Accept new client if one is available
    network_update();

    // Update the hand state
    // hand1.update();
    
/*
    https://g.co/gemini/share/543de341c1fa

    // Run inference
    ei_impulse_result_t result = { 0 };
    // Ensure the ei_camera_get_data function correctly processes the camera buffer
    // into the format expected by the Edge Impulse model (e.g., grayscale float array).
    if (run_classifier(&ei_camera_get_data, &result, false) != EI_IMPULSE_OK) {
        Serial.println("ERR: Failed to run classifier");
        return;
    }

    // Process bounding box results
    Serial.printf("Found %d detections:\n", result.bounding_boxes_count);
    for (size_t i = 0; i < result.bounding_boxes_count; i++) {
        ei_object_detection_result_t bb = result.bounding_boxes[i];
        Serial.printf("  %s (%f): x=%d, y=%d, width=%d, height=%d\n",
                      bb.label, bb.value, bb.x, bb.y, bb.width, bb.height);

        // Use these bounding box coordinates for your object tracking logic
        // For example:
        // - Store previous bounding box to compare with current
        // - Calculate centroid of bounding box for simple tracking
        // - Implement more advanced tracking algorithms (Kalman filter, etc.)
*/

    delay(2000); // Small delay to prevent spamming the loop
}
