#ifndef INPUT_H
#define INPUT_H

#include <vector>
#include <map>
#include <SDL2/SDL.h>

#include "Common.h"

enum Pressed: u32
    {
    PRESSED_LEFT            = 0x1,
    PRESSED_RIGHT           = 0x2,
    PRESSED_UP              = 0x4,
    PRESSED_DOWN            = 0x8,
    PRESSED_WAIT            = 0x10,
    PRESSED_PAUSE           = 0x20,
    PRESSED_QUIT            = 0x40,
    PRESSED_LEFTCLICK       = 0x80,
    PRESSED_RIGHTCLICK      = 0x100,
    PRESSED_SCROLLUP        = 0x200,
    PRESSED_SCROLLDOWN      = 0x400,
    PRESSED_WIDTH_PLUS      = 0x800,
    PRESSED_WIDTH_MINUS     = 0x1000,
    PRESSED_HEIGHT_PLUS     = 0x2000,
    PRESSED_HEIGHT_MINUS    = 0x4000,
    PRESSED_SAVE            = 0x8000,
    };

static const std::map <u32, u32> sdlkToBabaMap = 
    {
    std::make_pair ( KEY_LEFT, PRESSED_LEFT ),
    std::make_pair ( KEY_RIGHT, PRESSED_RIGHT ),
    std::make_pair ( KEY_UP, PRESSED_UP ),
    std::make_pair ( KEY_DOWN, PRESSED_DOWN ),
    std::make_pair ( KEY_PAUSE, PRESSED_PAUSE ),
    std::make_pair ( KEY_WIDTH_PLUS, PRESSED_WIDTH_PLUS ),
    std::make_pair ( KEY_WIDTH_MINUS, PRESSED_WIDTH_MINUS ),
    std::make_pair ( KEY_HEIGHT_PLUS, PRESSED_HEIGHT_PLUS ),
    std::make_pair ( KEY_HEIGHT_MINUS, PRESSED_HEIGHT_MINUS ),
    std::make_pair ( KEY_QUIT, PRESSED_QUIT ),
    std::make_pair ( KEY_SAVE, PRESSED_SAVE ),
    };

class Input 
    {
public: 
    auto pressed ( u32 press ) const -> bool;
    auto x () const -> u32;
    auto y () const -> u32;
    auto update () -> void;
private:
    u32 m_pressed=0U;
    u32 m_x=0U;
    u32 m_y=0U;
    };

#endif 