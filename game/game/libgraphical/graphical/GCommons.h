#ifndef GCOMMONS_H
#define GCOMMONS_H

#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<SDL2/SDL.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// WRAP_FIELD Macro
/*
 * From struct S, wrap a field F of type T width name A
 *
 * This macro must provide a simple and readable way of wrap attributes from
 * any struct. For this, it must accept four parameters, which one is optional:
 * strct    field               name(optional)      type
 * (Struct) (Attribute's field) (Attribute's name)  (Attribute's type)
 *
 * The parameter strct specify the data-type where the field is located.
 * This field have it's struct location and type passed by own "field" and "type" parameters,
 * while it's wrapped name, also called attribute's name, is set by "name" paremeter.
 * Note that this last one is optional, and when omitted, the attribute name become the field
 * name.
 *
 * This macro will expand into a static inline function:
 * static inline type* strct##_##name(const strct*s)
 * {
 *      return &s->field;
 * }
 */

#define WRAP_FIELD_VA_NUM_ARGS_IMPL(_0, _1, _2, _3, _4, N, ...) N
#define WRAP_FIELD_VA_NUM_ARGS(...) WRAP_FIELD_VA_NUM_ARGS_IMPL(__VA_ARGS__,, 4, 3 ,2, 1, 0)

#define WRAP_FIELD_CORE(strct, field, name, type) \
static inline type* strct##_##name(strct*s) \
{ \
    return &s->field; \
}

#define WRAP_FIELD_0 \
    _Static_assert(0, "WRAP_FIELD must have 3 or 4 parameters!")
#define WRAP_FIELD_1(arg1) \
    WRAP_FIELD_0
#define WRAP_FIELD_2(arg1, arg2) \
    WRAP_FIELD_0
#define WRAP_FIELD_3(strct, field, type) \
    WRAP_FIELD_CORE(strct, field, field, type)
#define WRAP_FIELD_4(strct, field, name, type) \
    WRAP_FIELD_CORE(strct, field, name, type)


#define WRAP_FIELD_CAT(a, b) a##b
#define WRAP_FIELD_SELECT(N) WRAP_FIELD_CAT(WRAP_FIELD_, N)
#define WRAP_FIELD(...) \
    WRAP_FIELD_SELECT(WRAP_FIELD_VA_NUM_ARGS(__VA_ARGS__))(__VA_ARGS__)

// DEFAULT_VALUE
/*
 * This macro attribute a default value for any variable if it's value is zero or
 * any other specified by zero_val parameter.
 * This macro must accept two normal parameters and one opptional:
 * var              def_val                     zero_val
 * (Variable)       (Variable default value)    (Zero value)
 *
 * The var parameter is the variable;
 * def_val pass the default value for this variable case it's value is equals zero_val;
 * And zero_val is a conditional value.
 *
 * This macro will be expanded into:
 * var = (var == (zero_val))? (def_val): var;
 */
#define DEFAULT_VALUE_VA_NUM_IMPL(_0, _1, _2, _3, N, ...) N
#define DEFAULT_VALUE_VA_NUM(...) DEFAULT_VALUE_VA_NUM_IMPL(__VA_ARGS__,, 3, 2, 1, 0)

#define DEFAULT_VALUE_CORE(var, def_val, zero_val) \
    var = (var == (zero_val))? (def_val): var

#define DEFAULT_VALUE_0 \
    _Static_assert(0, "DEFAULT_VALUE Macro accept 2 or 3 parameters!")
#define DEFAULT_VALUE_1(arg1) \
    DEFAULT_VALUE_0
#define DEFAULT_VALUE_2(var, def_val) \
    DEFAULT_VALUE_CORE(var, def_val, 0)
#define DEFAULT_VALUE_3(var, def_val, zero_val) \
    DEFAULT_VALUE_CORE(var, def_val, zero_val)

#define DEFAULT_VALUE_CAT(a, b) a##b
#define DEFAULT_VALUE_SELECT(N) DEFAULT_VALUE_CAT(DEFAULT_VALUE_, N)

#define DEFAULT_VALUE(...) \
    DEFAULT_VALUE_SELECT(DEFAULT_VALUE_VA_NUM(__VA_ARGS__))(__VA_ARGS__)

// Logs
/*
 * These macros represents a portable and fast-way to make logs across the software,
 * generally it is expanded into the already provied log messages, like
 * SDL_LogMessage from SDL graphical library
 */
#define GRAPHICAL_LOG(fmt, ...) \
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, fmt, ##__VA_ARGS__)

#define GRAPHICAL_LOG_CRITICAL(fmt, ...) \
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_CRITICAL, fmt, ##__VA_ARGS__)

#define GRAPHICAL_LOG_DEBUG(fmt, ...) \
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_DEBUG, fmt, ##__VA_ARGS__)

#define GRAPHICAL_LOG_WARN(fmt, ...) \
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, fmt, ##__VA_ARGS__)

/* Nome antigo (com erro de digitacao), mantido por compatibilidade. */
#define GRAPHICAL_LOG_WARM GRAPHICAL_LOG_WARN

#define GRAPHICAL_LOG_ERROR(fmt, ...) \
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, fmt, ## __VA_ARGS__)

#define GRAPHICAL_LOG_VERBOSE(fmt, ...) \
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_VERBOSE, fmt, ##__VA_ARGS__)

// Others usefull macros
#define MAX(a, b) ((a) > (b)? (a): (b))
#define MIN(a, b) ((a) > (b)? (b): (a))
#define RAD_TO_DEG(r) (180 / M_PI * (r))
#define DEG_TO_RAD(d) (M_PI / 180 * (d))
#define INVALID_PTR (void*)0xDEADBEEF

#endif
