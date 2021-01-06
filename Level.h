#ifndef LEVEL_H
#define LEVEL_H

#include "Common.h"
#include "Block.h"
#include "Rule.h"

namespace Level 
    {
    enum 
        {
        FLAG_PARSEWORDS,
        FLAG_WIN,
        };

    struct Level 
        {
        int id;
        unsigned frames;
        u8 width, height;
        u32 flags; 
        std::string name;
        std::vector<Block> blocks;
        std::vector<Rule> rules;
        auto canMove ( const Block& block, u8 x, u8 y) const -> bool;
        auto tryMove ( Block& block, u8 dir ) -> bool;
        };
    }
    
#endif 