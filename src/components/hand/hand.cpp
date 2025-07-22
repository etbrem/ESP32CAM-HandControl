
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

    Hand::Hand() : sm{*this} {}

    bool Hand::setup(){
        log_setup();

        // Initialize screen center based on camera resolution (QVGA: 320x240)
        this->screen_center = {160, 120};

        // Start the state machine cycle
        sm.process_event(start_cycle{});

        log_i("Hand component ready");
        return true;
    }

    void Hand::lockon_target(const object_detected& event) {
        log_d("ACTION: Locking on target at x:%d, y:%d", event.tracking_info.x, event.tracking_info.y);
        this->current_target = event.tracking_info;
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

            camera_fb_t *fb = camera_get_frame();
            if (!fb) {
                log_e("Camera frame capture failed");
                return;
            }

            tracking_data_t tracking_info;
            object_metrics_t metrics = find_best_object(fb, &tracking_info);

            camera_return_frame(fb);

            // If a valid object with a high enough score is found, lock on.
            if (metrics.is_valid && metrics.signature_score > 0.8f) {
                sm.process_event(object_detected{tracking_info});
            }

        } else if (sm.is("Aligning"_s)) {
            log_d("State: Aligning");

            camera_fb_t *fb = camera_get_frame();
            if (!fb) {
                log_e("Camera frame capture failed");
                return;
            }

            object_metrics_t metrics = track_object(fb, &this->current_target);
            camera_return_frame(fb);

            if (!metrics.is_valid) {
                sm.process_event(object_lost{});
                return;
            }

            // Define a tolerance for centering
            const int tolerance = 10; // pixels
            int dx = metrics.centroid.x - this->screen_center.x;
            int dy = metrics.centroid.y - this->screen_center.y;

            // Check if the object is centered
            if (abs(dx) <= tolerance && abs(dy) <= tolerance) {
                log_i("Object is centered. Alignment complete.");
                sm.process_event(alignment_complete{});
            } else {
                // Issue move commands based on the largest error
                if (abs(dx) > abs(dy)) {
                    if (dx > 0) sm.process_event(move_left{});
                    else sm.process_event(move_right{});
                } else {
                    if (dy > 0) sm.process_event(move_up{});
                    else sm.process_event(move_down{});
                }
            }

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