#include <Arduino.h>

#include "blink/sequences.h"


#ifdef BLINK_TEST
void blink_test() {
    log_update();

    #pragma push_macro("SEQ")
    
    #undef SEQ
    #define SEQ(seq, ...)                                           \
    do {                                                            \
        blink_sequence_start( CAT( Hand::BlinkSequence_ , seq ) );  \
        log_n("Testing blink sequence: %s\n", STRINGIFY(seq));    \
        delay(10 * 1000);                                           \
    } while(0);
    
    #include "components/hand/blink_sequences.list"
    #pragma pop_macro("SEQ")

}
#endif