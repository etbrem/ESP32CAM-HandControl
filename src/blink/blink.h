#pragma once

#ifndef _BLINK_H_
#define _BLINK_H_

#include "utils/macros.h"



// Define the built-in LED pin for ESP32-CAM
#ifndef LED_BUILTIN
#define LED_BUILTIN 4
#endif

// Keep existing sequence structure for compatibility
typedef struct _BlinkInterval {
  int duration;
  int brightness;
} BlinkInterval;

typedef BlinkInterval *BlinkSequence;


#ifndef BLINK_DISABLE
///////////////////////////////////
// Blink - Enabled
///////////////////////////////////

#include <jled.h>

// Initialize the LED controller
bool blink_setup();

// Start a sequence and return immediately (non-blocking)
void blink_sequence_start(BlinkSequence sequence);

// Update the LED state - should be called in every loop iteration
void blink_update();

// Stop any running sequence
void blink_stop();


#else  /* BLINK_DISABLE */
///////////////////////////////////
// Blink - Disabled
///////////////////////////////////
#define blink_setup(...)
#define blink_sequence_start(...)
#define blink_update(...)
#define blink_stop(...)

#endif  /* BLINK_DISABLE */

#endif /* _BLINK_H_ */