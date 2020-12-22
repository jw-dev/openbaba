#ifndef LEVEL_H
#define LEVEL_H

#include "Common.h"
#include "Block.h"
#include "Input.h"

class Level 
    {
public:
    int id; 
    unsigned int frames; 
    std::string name;
    u8 width, height;
    std::vector<Block> blocks;
    
    Level (int id);
    auto load (const std::string& path) -> void;
    auto save (const std::string& path) -> void;
    auto update (Input* input) -> bool;
private:
    bool m_hasMovedBlock = false;
    bool m_hasMoved = false;

    auto doInput (Input* input) -> bool;

    auto checkWin () -> bool;
    auto parseRules (BlockID id = BlockID::EMPTY) -> void;
    auto applyRule (BlockID noun, Block* target) -> void;

    // Try and move a block.
    // This attempts to move a block in a direction and any blocks that are in the way.
    auto tryMove (Block& b, Direction d) -> bool;

    auto getBlocks (u8 x, u8 y) -> std::vector< Block* >;
    auto getBlocks (BlockID byId, BlockType byType) -> std::vector< Block* >;

    auto getAdjacentBlocks (u8 x, u8 y, Direction d) -> std::vector<Block>;
    };

#endif 