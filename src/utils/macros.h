#pragma once

#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/variadic/to_seq.hpp>

#include <Arduino.h>

// STRINGIFY Macro
#define STRINGIFY(x) BOOST_PP_STRINGIZE(x)
#define STRINGIFY_ALL_ARGS(...) \
    BOOST_PP_SEQ_FOR_EACH(STRINGIFY_EACH_ARG, _, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))

// CAT (Concatenate) Macro
// The inner macro _CAT_ concatenates its two arguments.
// The outer macro CAT ensures both arguments are fully expanded before concatenation.
#define _CAT_(a, b) a##b
#define CAT(a, b) _CAT_(a, b)


#define serial_setup(baud)           \
    do {                             \
        Serial.begin(baud);          \
        Serial.setDebugOutput(true); \
    } while (0);


#define log_setup()  log_d("Setting up")
#define log_update(x) log_d("Updating")
