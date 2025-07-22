#include "hand.h"

#include "utils/macros.h"

#include "components/hand/events.h"
#include "components/hand/hand.h"

namespace Hand {
    namespace HandStateActions {

        void hand_lockon_target(const object_detected &event, Hand& hand) {
            log_d("ACTION: Locking on target via free function...");
            hand.lockon_target(event);
        }


        void hand_move_up(const move_up &event) {
            log_d("ACTION: Move up %d\n", event.amount);
            blink_sequence_start(BlinkSequence_UP);
        }
        
        void hand_move_down(const move_down &event) {
            log_d("ACTION: Move down %d\n", event.amount);
            blink_sequence_start(BlinkSequence_DOWN);
        }
        
        void hand_move_left(const move_left &event) {
            log_d("ACTION: Move left %d\n", event.amount);
            blink_sequence_start(BlinkSequence_LEFT);
        }

        void hand_move_right(const move_right &event) {
            log_d("ACTION: Move right %d\n", event.amount);
            blink_sequence_start(BlinkSequence_RIGHT);
        }

        void hand_extend() {
            log_d("ACTION: Extending hand...");
            blink_sequence_start(BlinkSequence_EXTEND);
        }

        void hand_retract() {
            log_d("ACTION: Retracting hand...");
            blink_sequence_start(BlinkSequence_STOP);
        }

        void hand_open_fingers() {
            log_d("ACTION: Opening fingers...");
            blink_sequence_start(BlinkSequence_STOP);
        }

        void hand_close_fingers() {
            log_d("ACTION: Closing fingers...");
            blink_sequence_start(BlinkSequence_GRASP);
        }

        void hand_stop_all() {
            log_d("ACTION: Stopping all movement...");
            blink_sequence_start(BlinkSequence_STOP);
        }



    }
}