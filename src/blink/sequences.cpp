#include "sequences.h"


#ifndef BLINK_DISABLE
///////////////////////////////////
// Blink - Enabled
///////////////////////////////////

// BlinkInterval BlinkSequenceData_Boot[] = {
//   {100, 10}, {300, 0}, {100, 10}, {1000, 0}, 

//   {300, 10}, {200, 20}, {500, 15}, {500, 0}, 
//   {300, 10}, {200, 20}, {500, 15}, {500, 0}, 
  
//   {100, 10}, {300, 0}, {100, 10}, {1000, 0}, 

//   {0, 0}
// };


// BlinkInterval BlinkSequenceData_Idle[] = {
//   {500, 2}, {500, 4}, {4000, 0}, 
  
//   {0, 0}
// };

// BlinkInterval BlinkSequenceData_Connected[] = {
//   {500, 2}, {500, 4}, {500, 2}, 
//   {500, 2}, {500, 4}, {500, 2}, 
  
//   {0, 0}
// };

#include "utils/macros.h"


// BlinkSequence BlinkSequence_Boot = (BlinkSequence) BlinkSequenceData_Boot;
// BlinkSequence BlinkSequence_Idle = (BlinkSequence) BlinkSequenceData_Idle;
// BlinkSequence BlinkSequence_Connected = (BlinkSequence) BlinkSequenceData_Connected;


#pragma push_macro("SEQ")
    // Initialize  BlinkSequenceData_${seq}
    #include "_internals/initialize_blinksequence_data.h"
    #include "sequences.list"

    // Initialize  BlinkSequence_${seq}  with  BlinkSequenceData_${seq}
    #include "_internals/initialize_blinksequence.h"
    #include "sequences.list"
#pragma pop_macro("SEQ")

#endif  /* BLINK_DISABLE */
