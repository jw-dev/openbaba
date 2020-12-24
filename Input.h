#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>
#include "Common.h"

enum class Key 
    {
    NONE,
    LEFT = 0x1,
    RIGHT = 0x2,
    UP = 0x4,
    DOWN = 0x8,
    WAIT = 0x10,
    PAUSE = 0x20,
    EDITOR_HEIGHT_PLUS = 0x40,
    EDITOR_HEIGHT_MINUS = 0x80,
    EDITOR_WIDTH_PLUS = 0x100,
    EDITOR_WIDTH_MINUS = 0x200,
    };

class Input 
    {
public: 
    auto moveDirection () const -> Direction;
    auto moving () const -> bool;
    auto quit () const -> bool;
    auto pressed () const -> Key;
    auto update () -> void;
private:
    bool m_isQuit = false;
    Key m_key = Key::NONE;
    Direction m_moveDirection = Direction::NONE;
    };

#endif 