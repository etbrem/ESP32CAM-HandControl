#undef SEQ
#define SEQ(name, ...)  BlinkSequence CAT(BlinkSequence_, name) = (BlinkSequence) CAT(BlinkSequenceData_, name);