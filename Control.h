#ifndef CONTROL_H 
#define CONTROL_H 

#include <string>
#include <vector>
#include <algorithm>
#include <stdint.h>

#include "IO.h"
#include "Parser.h"
#include "Input.h"
#include "Block.h"
#include "Rule.h"
#include "Common.h"
#include "Level.h"
#include "Draw.h"

namespace Level 
    {
    class Control 
        {
    public: 
        Control (Parser* parser, Draw* draw, Input* input, IO* io);
        auto gameLoop() -> void;
    private: 
        Level * m_level;
        Parser * m_parser;
        Draw * m_draw;
        Input * m_input;
        IO * m_io; 
        bool m_editMode; // We are on edit mode

        auto move () -> void;
        auto draw () -> void;
        auto update () -> void;
        };
    }

#endif 