#include <Arduino.h>
#include "blink.h"

#ifndef BLINK_DISABLE
///////////////////////////////////
// Blink - Enabled
///////////////////////////////////

// Static JLED instance for the built-in LED
static auto led = JLed(LED_BUILTIN);

// Store the current sequence
static BlinkSequence currentSequence = nullptr;
static int currentStep = 0;
static unsigned long lastChange = 0;

static TaskHandle_t blinkTaskHandle = NULL;


void blinkTask(void * parameter) {
    lastChange = millis();
    
    for(;;) {
        // unsigned long currentTime = millis();
        // unsigned long elapsedTime = currentTime - lastChange;
        long delayTime = 50; // Default delay

        if (currentSequence) {
            blink_update();

            // // Calculate remaining time for the current step's duration
            // // Assuming duration is in milliseconds
            // long expectedDuration = currentSequence[currentStep].duration;
            // long remainingTime = expectedDuration - elapsedTime;

            // if (remainingTime > 0) {
            //     delayTime = remainingTime; // Delay for remaining time
            // } 
        }
        
        vTaskDelay(pdMS_TO_TICKS(delayTime));
    }
}

bool blink_setup() {
    log_d("blink");
    
    xTaskCreatePinnedToCore(
        blinkTask,
        "BlinkTask",
        2048,
        NULL,
        4,
        &blinkTaskHandle,
        0
    );
    return true;
}

inline void blink_set_current(BlinkInterval *interval){
    // Set brightness and duration
    led.Set(interval->brightness);
    led.DelayAfter(interval->duration);
    led.Update();
}

void blink_sequence_start(BlinkSequence sequence) {
    if (!sequence || (currentSequence == sequence) ) return;

    currentSequence = sequence;
    currentStep = 0;
    
    // Start with the first interval
    BlinkInterval *curr = &sequence[0];
    blink_set_current(curr);
}

inline void blink_update() {
    if (!currentSequence) return;
    
    // If the current effect is done
    if (led.IsRunning()) {
        led.Update();

    } else {
        // Move to next step
        currentStep++;
        BlinkInterval *curr = &currentSequence[currentStep];
        
        if(!curr->duration){
            currentStep = 0;
            curr = &currentSequence[0];
        }
        blink_set_current(curr);
    }

    // lastChange = millis();
}

void blink_stop() {
    currentSequence = nullptr;
    currentStep = 0;
    lastChange = 0;
    led.Stop();
    led.Off().Update();
}


#else  /* BLINK_DISABLE */
///////////////////////////////////
// Blink - Disabled
///////////////////////////////////
#define blink_setup(...)
#define blink_sequence_start(...)
#define blink_update(...)
#define blink_stop(...)

#endif  /* BLINK_DISABLE */