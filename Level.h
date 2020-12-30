#ifndef LEVEL_H
#define LEVEL_H

#include <fstream>
#include <iostream>
#include <sstream> 
#include <functional>
#include <chrono>
#include <algorithm>

#include "Common.h"
#include "Block.h"
#include "Rule.h"

class Level 
    {
public:
    int id; 
    unsigned int frames; 
    std::string name;
    u8 width, height;
    u32 flags;
    std::vector <Block> blocks;
    std::vector <Rule> rules;
    Level (int id);
    auto load (const std::string& path) -> void;
    auto save (const std::string& path) -> void;
    auto movement (u8 movement) -> void;
    auto tick () -> bool;
private:
    auto tryMove (Block& block, u8 direction) -> bool;
    auto canMove ( Block& block, u8 x, u8 y ) -> bool;

    auto getRules () -> void;
    auto addRule (Rule& rule) -> void;

    auto doReset () -> void;
    auto doTransformations () -> void;
    auto doActions () -> void;
    auto doWinConditions () -> void;
    };

#endif 