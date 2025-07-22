#pragma once

#ifndef _BLINKSEQUENCES_H_
#define _BLINKSEQUENCES_H_

#ifndef BLINK_DISABLE
///////////////////////////////////
// Blink - Enabled
///////////////////////////////////

#include "utils/macros.h"
#include "blink.h"

// extern BlinkSequence BlinkSequence_Boot;
// extern BlinkSequence BlinkSequence_Idle;
// extern BlinkSequence BlinkSequence_Connected;


#pragma push_macro("SEQ")
    #include "_internals/declare_external_blinksequence.h"
    #include "sequences.list"
#pragma pop_macro("SEQ")


#endif  /* BLINK_DISABLE */

#endif /* _BLINKSEQUENCES_H_ */
