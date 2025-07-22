#pragma once

#include "video/object_analysis.h"


namespace Hand {
    namespace HandStateGuards {

        // --- Guards ---
        // A condition that must be true for a transition to be taken.
        auto no_guard       = []{ return true; };
        auto centered_guard = [] { return robot_is_centered(); };
        
    }
}