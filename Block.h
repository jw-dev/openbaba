#ifndef BLOCK_H
#define BLOCK_H

#include "Enums.h"
#include <set>
#include <vector>
#include <stdexcept>

class Block 
    {
    public:
        std::string name;
        u16 id;
        u8 tile; 
        u8 x;
        u8 y;
        u8 direction;
        bool rotation;

        Block ( std::string name, u16 id, u8 tile, bool rotate=false );
        auto addProp (u8 prop) -> void;
        auto hasProp (u8 prop) const -> bool;
        auto removeProp (u8 prop) -> void;
        auto removeAllProps () -> void;

        // Helper functions 
        auto isEntity () const -> bool;
        auto isWord () const -> bool;
        auto isNoun () const -> bool;
        auto isCondition () const -> bool;
        auto isOperator () const -> bool;
        auto isProp () const -> bool;
        auto isPrefix () const -> bool;
        auto isInfix () const -> bool;
    private:
        std::set <u8> m_props;
    };

Block makeBlock (u8 id, u8 tile); 
Block makeBlock (u8 id, u8 tile, u8 x, u8 y);
Block getBlock ( size_t index );
size_t countBlocks ();

#endif