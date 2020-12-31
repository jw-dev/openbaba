#include "Block.h"

static const std::vector <Block> allBlocks = 
    {
    Block { "BABA", ENTITY_BABA, TILE_ENTITY, true  },
    Block { "FLAG", ENTITY_FLAG, TILE_ENTITY, false },
    Block { "ROCK", ENTITY_ROCK, TILE_ENTITY, false },
    Block { "BABA", ENTITY_BABA, TILE_WORD },
    Block { "IS", WORD_IS, TILE_WORD },
    Block { "YOU", WORD_YOU, TILE_WORD },
    Block { "FLAG", WORD_FLAG, TILE_WORD },
    Block { "WIN", WORD_WIN, TILE_WORD },
    Block { "PUSH", WORD_PUSH, TILE_WORD },
    Block { "ROCK", WORD_ROCK, TILE_WORD },
    Block { "STOP", WORD_STOP, TILE_WORD },
    Block { "PULL", WORD_PULL, TILE_WORD },
    Block { "SWAP", WORD_SWAP, TILE_WORD },
    Block { "TELE", WORD_TELE, TILE_WORD },
    };

Block makeBlock (u8 id, u8 tile)
    {
    return makeBlock ( id, tile, 0, 0 );
    }

Block makeBlock (u8 id, u8 tile, u8 x, u8 y)
    {
    for ( const auto& b: allBlocks )
        {
        if (b.id == id && b.tile == tile)
            {
            Block res = b;
            res.x = x;
            res.y = y;
            return res;
            }
        }
    fprintf(stderr, "makeBlock (%d, %d, %d, %d): invalid block specified", id, tile, x, y);
    throw std::runtime_error("makeBlock ()");
    }

Block getBlock ( size_t index )
    {
    if ( index > allBlocks.size() - 1 )
        throw std::runtime_error ( "getBlock(): invalid index specified" );
    return allBlocks.at(index);
    }

size_t countBlocks ()
    {
    return allBlocks.size ();
    }

Block::Block ( std::string p_name, u16 p_id, u8 p_tile, bool p_rotation )
  : name ( p_name ),
    id ( p_id ),
    tile ( p_tile ),
    rotation ( p_rotation ),
    x ( 0 ),
    y ( 0 ),
    direction ( DIRECTION_RIGHT ),
    m_props ()
    {
    if ( p_tile == TILE_WORD )
        {
        addProp ( PROPERTY_PUSH ); // All words are implicitly push
        }
    }

auto Block::addProp ( u8 prop ) -> void 
    {
    m_props.insert ( prop );
    }

auto Block::hasProp ( u8 prop ) const -> bool 
    {
    return m_props.find ( prop ) != m_props.end ();
    }

auto Block::removeProp ( u8 prop ) -> void 
    {
    m_props.erase ( prop );
    }

auto Block::removeAllProps () -> void 
    {
    m_props.clear ();
    }

auto Block::isEntity () const -> bool 
    {
    return tile == TILE_ENTITY;
    }

auto Block::isWord () const -> bool 
    {
    return tile == TILE_WORD;
    }

auto Block::isNoun () const -> bool 
    {
    return id >= WORD_BABA && id < WORD_YOU;
    }

auto Block::isProp () const -> bool 
    {
    return id >= WORD_YOU && id < WORD_IS;
    }

auto Block::isOperator () const -> bool 
    {
    return id >= WORD_IS && id < WORD_ON;
    }

auto Block::isCondition () const -> bool
    {
    return id >= WORD_ON && id < WORD_UNUSED;
    }

auto Block::isPrefix () const -> bool
    {
    return id >= WORD_LONELY && id < WORD_UNUSED;
    }

auto Block::isInfix () const -> bool
    {
    return isCondition() && !isPrefix();
    }