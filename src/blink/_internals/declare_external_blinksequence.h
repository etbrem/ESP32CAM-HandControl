#undef SEQ
#define SEQ(name, ...)  extern BlinkSequence CAT(BlinkSequence_, name); /* sequences of intervals of { duration_ms, brightness } */
