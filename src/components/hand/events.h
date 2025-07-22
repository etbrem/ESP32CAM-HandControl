#pragma once
#include "video/object_analysis.h"

namespace Hand {
    namespace HandStateEvents {

        // --- Events ---
        // These structs represent the triggers for state transitions.

        struct start_cycle {};

        struct object_detected {
            tracking_data_t tracking_info;
        };
        struct object_lost {};

        struct move_up {
            int amount;
        };
        struct move_down {
            int amount;
        };
        struct move_left {
            int amount;
        };
        struct move_right {
            int amount;
        };
        struct move_tilt {
            int degrees;
        };
        struct alignment_complete {};

        struct contact_made {};
        struct grasp_complete {};

        struct retraction_complete {};
        struct release_complete {};
        struct reset {};

    }
}