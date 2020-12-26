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

// Config
// Should be moved to a separate config file or something
const unsigned SPRITE_SIZE = 24U;
const unsigned PARTICLE_SIZE = 8U;
const unsigned PADDING = 25U;
const unsigned ANIMATION_TIMER = 10U;
constexpr float PARTICLE_SPAWN_CHANCE = 0.05f;
const char * const WORD_TEXTURE_PATH = "Data/words.png";
const char * const SPRITE_TEXTURE_PATH = "Data/sprites.png";
const char * const PARTICLE_TEXTURE_PATH = "Data/particles.png";

// controls 
#define KEY_QUIT            SDLK_ESCAPE
#define KEY_PAUSE           SDLK_SPACE
#define KEY_LEFT            SDLK_LEFT
#define KEY_UP              SDLK_UP
#define KEY_DOWN            SDLK_DOWN
#define KEY_RIGHT           SDLK_RIGHT
#define KEY_WIDTH_PLUS      SDLK_p
#define KEY_WIDTH_MINUS     SDLK_o
#define KEY_HEIGHT_PLUS     SDLK_l
#define KEY_HEIGHT_MINUS    SDLK_k

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