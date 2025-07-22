#undef SEQ
#define SEQ(name, ...)  BlinkInterval CAT(BlinkSequenceData_, name)[] = { __VA_ARGS__  {0, 0} };