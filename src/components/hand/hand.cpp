
#include "utils/macros.h"

#include "blink/sequences.h"
#include "video/object_analysis.h"
#include "components/hand/hand.h"



namespace Hand {


    ///////////////////////////////////
    // Blink sequences data & init
    ///////////////////////////////////

    #pragma push_macro("SEQ")
        // Initialize  BlinkSequenceData_${seq}
        #include "blink/_internals/initialize_blinksequence_data.h"
        #include "blink_sequences.list"

        // Initialize  BlinkSequence_${seq}  with  BlinkSequenceData_${seq}
        #include "blink/_internals/initialize_blinksequence.h"
        #include "blink_sequences.list"
    #pragma pop_macro("SEQ")


    ///////////////////////////////////
    // State Machine setup & update
    ///////////////////////////////////

    bool Hand::setup(){
        log_setup();

        // Start the state machine cycle
        sm.process_event(start_cycle{});
        return true;
    }

    void Hand::update(){
        log_update();

        // State-specific logic to dispatch events
        if (sm.is("Idle"_s)) {
            log_d("State: Idle");

            // In a real scenario, you might wait for a button press or network command to start.
            sm.process_event(start_cycle{});


        } else if (sm.is("Searching"_s)) {
            log_d("State: Searching");
            blink_sequence_start(BlinkSequence_SEARCH);
            
            // camera_fb_t* fb = camera_get_frame();
            // if (!fb) {
            //     log_d("Camera frame capture failed");
            //     return;
            // }
            // if (is_target_object(fb)) {
            //     sm.process_event(object_detected{});
            // } else {
            //     // No object found, stay in searching state
            // }
            // camera_return_frame(fb); // Return frame buffer

            delay(1000 * 10);

            log_i("Detected object");
            sm.process_event(object_detected{});


        } else if (sm.is("Aligning"_s)) {
            log_d("State: Aligning");

            // The align_robot action was already called. Here, we would check
            // if the alignment is complete. This could be time-based or sensor-based.
            // For this example, we'll assume it's complete after a short delay.
            delay(1000 * 10);
            sm.process_event(move_up{});
            delay(1000 * 10);
            sm.process_event(move_down{});
            delay(1000 * 10);
            sm.process_event(move_left{});
            delay(1000 * 10);
            sm.process_event(move_right{});
            delay(1000 * 10);

            sm.process_event(alignment_complete{});

        } else if (sm.is("Extending"_s)) {
            log_d("State: Extending");

            // Similar to aligning, we wait for the action to complete.
            delay(1000 * 10);
            sm.process_event(contact_made{}); // Assume contact is made

        } else if (sm.is("Grasping"_s)) {
            log_d("State: Grasping");

            delay(1000 * 10);
            sm.process_event(grasp_complete{});

        } else if (sm.is("Retracting"_s)) {
            log_d("State: Retracting");

            delay(1000 * 10);
            sm.process_event(retraction_complete{});

        } else if (sm.is("Releasing"_s)) {
            log_d("State: Releasing");
            
            delay(1000 * 10);
            sm.process_event(release_complete{});
        }
    }

}