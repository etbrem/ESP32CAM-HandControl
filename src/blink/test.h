


#ifdef BLINK_TEST
// If BLINK_TEST is defined declare the prototype
void blink_test();

#else

// If BLINK_TEST is undefined this does nothing
#define blink_test(...)

#endif
