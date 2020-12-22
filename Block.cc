#include "Block.h"

static std::vector <Block> allBlocks; 

auto initAllBlockTypes () -> void 
    {
    // entities
    allBlocks.emplace_back ( BlockID::BABA, BlockType::ENTITY );
    allBlocks.emplace_back ( BlockID::FLAG, BlockType::ENTITY );
    // words
    allBlocks.emplace_back ( BlockID::BABA, BlockType::WORD );
    allBlocks.emplace_back ( BlockID::IS, BlockType::WORD );
    allBlocks.emplace_back ( BlockID::YOU, BlockType::WORD );
    allBlocks.emplace_back ( BlockID::FLAG, BlockType::WORD );
    allBlocks.emplace_back ( BlockID::WIN, BlockType::WORD );
    }

auto destroyAllBlockTypes () -> void 
    {
    allBlocks.clear();
    }

Block makeBlock (BlockID id, BlockType type, int x, int y)
    {
    for ( const auto& b: allBlocks )
        {
        if (b.id == id && b.type == type)
            {
            Block res = b;
            res.x = x;
            res.y = y;
            return res;
            }
        }
    throw std::runtime_error ("invalid block specified");
    }

Block::Block (BlockID p_id, BlockType p_type) 
  : m_props (0U)
    {
    id = p_id;
    type = p_type;
    if (type == BlockType::WORD)
        {
        addProp (Property::MOVE); // All words are default moveable
        }
    }

auto Block::isWord () -> bool
    {
    return type == BlockType::WORD;
    }

auto Block::isNoun () -> bool
    {
    if ( !isWord() ) return false;
    return id == BlockID::BABA
        || id == BlockID::FLAG;
    }

auto Block::isJoiner () -> bool
    {
    if ( !isWord() ) return false;
    return id == BlockID::IS;
    }

auto Block::isProperty () -> bool
    {
    if ( !isWord() ) return false;
    return id == BlockID::YOU
        || id == BlockID::WIN
        || id == BlockID::MOVE
        || id == BlockID::EMPTY;
    }

auto Block::addProp (Property p) -> void 
    {
    u64 prop = (u64)p;
    if (prop && (m_props & prop) == 0)
        {
        m_props |= prop;
        }
    }

auto Block::hasProp (Property p) const -> bool 
    {
    u64 prop = (u64)p;
    if (prop)
        {
        return (m_props & prop);
        }
    return false;
    }

auto Block::removeProp (Property p) -> void 
    {
    u64 prop = (u64)p;
    if (prop)
        {
        m_props &= (~prop);
        }
    }

auto Block::removeAllProps () -> void
    {
    m_props = 0U;
    }