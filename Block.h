#ifndef BLOCK_H
#define BLOCK_H

#include "Common.h"
#include <vector>
#include <stdexcept>
#include <map>

enum class BlockID 
    {
    EMPTY,
    BABA,
    IS,
    YOU,
    FLAG,
    WIN,
    ROCK,
    PUSH,
    };

enum class BlockType 
    {
    ENTITY,
    WORD,
    };

enum class Property: u64
    {
    YOU = 0x1,
    PUSH = 0x2,
    STOP = 0x4,
    WIN = 0x8,
    };

static const std::map <BlockID, Property> mapBlockToProperty = 
    {
    std::make_pair ( BlockID::YOU, Property::YOU ),
    std::make_pair ( BlockID::PUSH, Property::PUSH ),
    std::make_pair ( BlockID::WIN, Property::WIN ),
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
        auto isWord () const -> bool;
        auto isNoun () const -> bool;
        auto isJoiner () const -> bool;
        auto isProperty () const -> bool;
    private:
        u64 m_props;
    };

Block makeBlock (BlockID id, BlockType type, int x, int y);

#endif