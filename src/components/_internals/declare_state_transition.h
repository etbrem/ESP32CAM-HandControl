#undef STATE
#define STATE(name, event_name, guard, action, next)  \
    CAT(STRINGIFY(name), _s)    + event<event_name>      [guard]      /action      =   CAT(STRINGIFY(next), _s) ,
