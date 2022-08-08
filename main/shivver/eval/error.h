#pragma once
#include <assert.h>
#include <stdarg.h>
#include "shivver/eval.h"

// Assert a property is true during evaluation.
//  This is inlined so we get a fast-path that does not need a function-call
//  when the property is true.
static inline void
reqeval ( eval_t*       state
        , bool          prop
        , char*         format
        , ...)
{
        if ( __builtin_expect (prop, true)) return;
        va_list args;
        va_start(args, format);
        shivver_eval_error(state, format, args);
        va_end(args);
}
