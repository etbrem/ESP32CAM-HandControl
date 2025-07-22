#include "utils/macros.h"

#include "blink/sequences.h"
#include "blink/test.h"

#include "components/hand/hand.h"
#include "network/network.h"

Hand::Hand hand1;

void setup() {
    serial_setup(115200); // This must match your monitor_speed
    log_setup();

    blink_setup();
    blink_sequence_start(BlinkSequence_BOOT);

    camera_setup();
    network_setup();
    
    hand1.setup();
}



void loop() {
    log_update();

    blink_test();  // If BLINK_TEST is undefined this does nothing
    
    // Accept new client if one is available
    network_update();

    // Update the hand state
    hand1.update();
    
    delay(100); // Small delay to prevent spamming the loop
}
