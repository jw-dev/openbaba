#include "Level.h" 

Level::Level (int id) 
    {
    std::ostringstream oss; 
    oss << "Data/"
        << id 
        << ".txt";
    load ( oss.str() );

    parseRules ();
    }

auto Level::load (const std::string& path) -> void
    {
    std::ifstream in ( path );
    if ( in && in.good () )
        {
        int theWidth; 
        int theHeight;
        int id; 
        int type;
        int x; 
        int y; 
        int direction;
        in >> name;
        in >> theWidth;
        in >> theHeight;
        while ( in >> id
                   >> type
                   >> x 
                   >> y
                   >> direction )
            {
            Block block = makeBlock ( (BlockID) id, (BlockType) type, x, y );
            block.direction = (Direction) direction;
            blocks.push_back ( block );
            }
        width = (u8) theWidth;
        height = (u8) theHeight;
        in.close ();
        }
    else 
        {
        throw std::runtime_error ( "unable to open/read from file" );
        }
    // When loading a new level, we need to parse the rules 
    flags |= LEVEL_PARSE_WORDS;
    }

auto Level::save (const std::string& path) -> void
    {
    std::ofstream out ( path );
    if (out && out.good ()) 
        {
        const int theWidth = (int) width;
        const int theHeight = (int) height;
        out << name << std::endl; 
        out << theWidth << std::endl;
        out << theHeight << std::endl;
        for (const auto& block: blocks) 
            {
            out << static_cast<int>(block.id)
                << " "
                << (int)block.type 
                << " "
                << (int)block.x
                << " "
                << (int)block.y
                << " "
                << (int)block.direction
                << std::endl;
            }
        out.close ();
        }
    else 
        {
        throw std::runtime_error ( "unable to open/write to file" );
        }
    out.close();
    }

auto Level::tick() -> bool
    {
    if ( flags & LEVEL_PARSE_WORDS )
        {
        parseRules ();
        }
    return checkWin ();
    }

auto Level::checkWin () -> bool 
    {
    for (auto& you: blocks) 
        {
        // Check for overlapping "YOU" and "WIN". 
        // This also handles the case where something that is "YOU" is also "WIN".
        if (!you.isWord() && you.hasProp(Property::YOU))
            {
            // Check blocks on same space 
            for (const auto& win: blocks) 
                {
                if ( win.x == you.x
                  && win.y == you.y
                  && win.hasProp ( Property::WIN )
                  && !win.isWord ())
                    {
                    return true;
                    }
                }
            }
        }
    return false;
    }

auto Level::tryMove ( Block& block, Direction dir ) -> bool
    {
    u8 newX = block.x;
    u8 newY = block.y;
    switch (dir) 
        {
        case Direction::UP: 
            if (block.y == 0) return false;
            newY--;
            break;
        case Direction::DOWN:
            if (block.y == height-1) return false;
            newY++;
            break;
        case Direction::LEFT: 
            if (block.x == 0) return false;
            newX--;
            break;
        case Direction::RIGHT: 
            if (block.x == width-1) return false;
            newX++;
            break;
        case Direction::NONE:
            return false;
        }
    // no movement
    if (newX == block.x && newY == block.y) 
        return false;

    // check all blocks in the way, recursively - if any of them can't move then neither can we 
    // std::vector< Block* > alsoToMove = getBlocks ( newX, newY );
    for (auto& toMove: blocks) 
        {
        if ( toMove.x == newX && toMove.y == newY) 
            {
            if ( toMove.hasProp ( Property::STOP ))
                return false; // Oops, we are stopped by that
            else if ( toMove.hasProp ( Property::PUSH ))
                {
                bool moved = tryMove ( toMove, dir );
                if (!moved)
                    return false;
                }
            }
        }
    if ( block.type == BlockType::WORD )
        flags |= LEVEL_PARSE_WORDS;
    block.x = newX;
    block.y = newY;
    return true;
    }

auto Level::hasBlockId ( u8 x, u8 y, BlockID id ) -> bool
    {
    for (const auto& block: blocks) 
        {
        if ( block.x == x && block.y == y && block.id == id)
            return true;
        }
    return false;
    }

auto Level::parseRules ( BlockID id ) -> void 
    {
    // First remove all props 
    // We should probably do better than this by "merging" props rather than remove and replace... 
    for (Block& block: blocks)   
        {
        if (block.type == BlockType::ENTITY)
            block.removeAllProps();
        }

    for ( Block& noun: blocks )
        {
        if ( !noun.isNoun() || (id != BlockID::EMPTY && noun.id != id) )
            continue; 

        for (int x: { 0, 1 }) 
            for (int y: { 0, 1 })
                {
                if (x == y)
                    continue;
                if ( !hasBlockId ( noun.x + x, noun.y + y, BlockID::IS ) )
                    continue;

                // Noun is joining to something, let's get the next word and see what we can apply 
                const u8 propX = noun.x + x*2; 
                const u8 propY = noun.y + y*2; 
                for (auto& prop: blocks) 
                    {
                    if ( !(prop.x == propX && prop.y == propY) )
                        continue;

                    const bool isProperty = prop.isProperty ();
                    const bool isNoun = prop.isNoun ();
                    if ( isProperty || isNoun )
                        {
                        applyRule ( noun.id, prop );
                        if (isNoun)
                            parseRules ( prop.id );
                        }
                    }
                }
        }
    }

auto Level::applyRule (BlockID noun, Block& prop) -> void
    {
    for (auto& block: blocks) 
        {
        if ( block.id == noun && block.type == BlockType::ENTITY ) 
            {
            if ( prop.isNoun() ) 
                {
                block.id = prop.id; 
                block.removeAllProps ();
                }
            else if ( prop.isProperty() )
                {
                if ( mapBlockToProperty.find(prop.id) != mapBlockToProperty.end() )
                    {
                    block.addProp ( mapBlockToProperty.at (prop.id) );
                    }
                }
            }
        }
    }