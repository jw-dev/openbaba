#ifndef BLOCK_H
#define BLOCK_H

#include "Common.h"
#include <vector>
#include <stdexcept>

enum class BlockID 
    {
    EMPTY,
    BABA,
    IS,
    YOU,
    FLAG,
    WIN,
    MOVE,
    };

enum class BlockType 
    {
    ENTITY,
    WORD,
    };

enum class Property: u64
    {
    YOU = 0x1,
    MOVE = 0x2,
    STOP = 0x4,
    WIN = 0x8,
    };

class Block 
    {
    public:
        BlockID id; 
        BlockType type;
        u8 x;
        u8 y;
        Direction direction = Direction::RIGHT;
        explicit Block (BlockID id, BlockType type);
        auto addProp (Property p) -> void;
        auto hasProp (Property p) const -> bool;
        auto removeProp (Property p) -> void;
        auto removeAllProps () -> void;

        // Helper functions 
        auto isWord () -> bool;
        auto isNoun () -> bool;
        auto isJoiner () -> bool;
        auto isProperty () -> bool;
    private:
        u64 m_props;
    };

Block makeBlock (BlockID id, BlockType type, int x, int y);

#endif