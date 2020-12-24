#ifndef LEVEL_H
#define LEVEL_H

#include <fstream>
#include <iostream>
#include <sstream> 

#include "Common.h"
#include "Block.h"

enum LEVEL_FLAG 
    {
    LEVEL_PARSE_WORDS = 0x1,
    };

class Level 
    {
public:
    int id; 
    unsigned int frames; 
    std::string name;
    u8 width, height;
    std::vector<Block> blocks;
    u32 flags = LEVEL_PARSE_WORDS;
    
    Level (int id);
    auto load (const std::string& path) -> void;
    auto save (const std::string& path) -> void;
    auto tick () -> bool;
    auto tryMove (Block& b, Direction d) -> bool;

private:
    auto checkWin () -> bool;
    auto parseRules (BlockID id = BlockID::EMPTY) -> void;
    auto applyRule (BlockID noun, Block& target) -> void;

    // Try and move a block.
    // This attempts to move a block in a direction and any blocks that are in the way.
    auto getBlocks (u8 x, u8 y) -> std::vector< Block* >;
    auto getBlocks (BlockID byId, BlockType byType) -> std::vector< Block* >;
    };

#endif 