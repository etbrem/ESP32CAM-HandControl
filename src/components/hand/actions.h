#pragma once

#include "components/hand/events.h"

namespace Hand {
    namespace HandStateActions {
        using namespace HandStateEvents;

        void hand_lockon_target(const object_detected& event);

        void hand_move_up(const move_up &event);
        void hand_move_down(const move_down &event);
        void hand_move_left(const move_left &event);
        void hand_move_right(const move_right &event);
        
        void hand_extend();
        void hand_retract();
        
        void hand_open_fingers();
        void hand_close_fingers();
        
        void hand_stop_all();

        // Define actions
        auto no_action      = [] {};
        
        auto lockon_target_action = [](const object_detected& e) { hand_lockon_target(e); };

        auto move_up_action = [](const move_up &e){ hand_move_up(e); };
        auto move_down_action = [](const move_down &e){ hand_move_down(e); };
        auto move_left_action = [](const move_left &e){ hand_move_left(e); };
        auto move_right_action = [](const move_right &e){ hand_move_right(e); };

        auto extend_action = [] { hand_extend(); };
        auto retract_action = [] { hand_retract(); };

        auto open_action = [] { hand_open_fingers(); };
        auto close_action = [] { hand_close_fingers(); };
        
        auto stop_action = [] { hand_stop_all(); };

    }
}