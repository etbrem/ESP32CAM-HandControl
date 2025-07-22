#pragma once

/*
The warning you're seeing (-Wsubobject-linkage) is a known issue with Boost.SML and newer GCC compilers (GCC 10+),
as confirmed by our web search.

The Boost.SML developers themselves consider this a "false positive" and have added 
a pragma to their sml.hpp header to suppress it.
*/

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsubobject-linkage"
#include <boost/sml.hpp>
#pragma GCC diagnostic pop


#include <string>

#include "utils/macros.h"

#include "blink/blink.h"
#include "video/object_analysis.h"

#include "components/hand/events.h"
#include "components/hand/guards.h"
#include "components/hand/actions.h"

namespace sml = boost::sml;
using namespace boost::sml::literals;


namespace Hand {
    using namespace HandStateEvents;
    using namespace HandStateGuards;
    using namespace HandStateActions;


    ///////////////////////////////////
    // Blink sequences  
    ///////////////////////////////////

    // Define the SEQ macro to declare "extern BlinkSequence_${seq}"
    // then include the list of sequences
    #pragma push_macro("SEQ")
        // Declare  BlinkSequence_${seq}
        #include "blink/_internals/declare_external_blinksequence.h"
        #include "blink_sequences.list"
    #pragma pop_macro("SEQ")


    ///////////////////////////////////
    // State machine definition
    ///////////////////////////////////

    // --- State Machine Definition ---
    // Uses boost.SML's declarative DSL to define the state machine logic.

    struct HandSM {
        auto operator()() const {
            using namespace sml;
            

            // --- Transition Table ---
            // Defines the core logic: Current State + Event [Guard] / Action = Next State
            return make_transition_table(

                #include "components/_internals/transition_table_begin.h"

                #pragma push_macro("STATE")
                #include "components/_internals/declare_state_transition.h"
                #include "components/hand/states.list"
                #pragma pop_macro("STATE")

                #include "components/_internals/transition_table_end.h"
            );
        }
    };


    ///////////////////////////////////
    // Class wrapper for state machine
    ///////////////////////////////////

    class Hand {
    public:
        sml::sm<HandSM> sm;

        // Data for the currently tracked object
        tracking_data_t current_target;
        // Center of the camera's region of interest
        point_t screen_center;

        Hand();

        bool setup();
        void update();

        // Action to store tracking data for the detected object
        void lockon_target(const object_detected& event);
    };
};

