#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <chrono>
#include "Level.h"

namespace Level 
    {
    class Parser 
        {
    public: 
        virtual auto parse (const Level* level) -> std::vector<Rule>;
        };
    }

#endif 