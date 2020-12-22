#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>

typedef uint_least8_t u8;
typedef uint_least16_t u16;
typedef uint_least32_t u32;
typedef uint_least64_t u64;
typedef int_least8_t s8;
typedef int_least16_t s16;
typedef int_least32_t s32;

enum State 
    {
    STARTTITLE      = 0,
    STARTMAP        = 1<<8,
    STARTGAME       = 2<<8,
    STARTSAVEDGAME  = 3<<8,
    STARTPICTURE    = 4<<8,
    EXITGAME        = 5<<8,
    };

enum class Movement 
    {
    HORIZONTAL,
    VERTICAL,
    NONE,
    };

enum class Direction 
    {
    RIGHT,
    DOWN,
    UP,
    LEFT,
    NONE,
    };


#endif 