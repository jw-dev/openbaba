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