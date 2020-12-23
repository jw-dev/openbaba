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

auto Level::update(const Input& input) -> bool
    {
    bool isDone = false;
    
    (void) doInput(input);
    if ( m_hasMovedBlock )
        {
        parseRules ();
        }
    
    if ( m_hasMoved ) 
        {
        isDone = checkWin ();
        }
    m_hasMoved = false;
    m_hasMovedBlock = false;
    frames++;
    return isDone;
    }

auto Level::doInput (const Input& input) -> bool 
    {
    Direction move = input.moveDirection();
    bool isMove = move != Direction::NONE; 
    if ( isMove )
        {
        for ( auto& block: blocks )
            {
            // Only "YOU" can move
            if ( block.hasProp (Property::YOU )) 
                {
                bool moved = tryMove ( block, move );
                if (moved)
                    {
                    m_hasMoved = true;
                    block.direction = move;
                    }
                }
            }
        }
    return isMove;
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
            std::vector<Block*> sameSpace = getBlocks ( you.x, you.y );
            if ( !sameSpace.empty() )
                {
                for ( Block* win: sameSpace )
                    {
                    if ( !win->isWord() && win->hasProp( Property::WIN ) )
                        return true;
                    }
                }
            }
        }
    return false;
    }

auto Level::getBlocks ( u8 x, u8 y ) -> std::vector< Block* >
    {
    std::vector< Block* > res; 
    for (auto& block: blocks) 
        {
        if (block.x == x && block.y == y)
            {
            res.push_back( &block );
            }
        }
    return res;
    }

auto Level::getBlocks (BlockID byId, BlockType byType) -> std::vector< Block* >
    {
    std::vector< Block* > res; 
    for (auto& block: blocks) 
        {
        if (block.id == byId && block.type == byType)
            {
            res.push_back( &block );
            }
        }
    return res;
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
    std::vector< Block* > alsoToMove = getBlocks ( newX, newY );
    if ( !alsoToMove.empty() )
        {
        for ( size_t i = 0 ; i < alsoToMove.size(); ++i )
            {
            Block* toMove = alsoToMove [i];
            if ( toMove->hasProp ( Property::STOP ))
                return false; // Oops, we are stopped by that
            else if ( toMove->hasProp ( Property::PUSH ))
                {
                bool moved = tryMove ( *toMove, dir );
                if (!moved)
                    return false;
                }
            }
        }
    if ( block.type == BlockType::WORD )
        m_hasMovedBlock = true;
    block.x = newX;
    block.y = newY;
    return true;
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

    for (Block& block: blocks) 
        {
        if ( !block.isNoun() || (id != BlockID::EMPTY && block.id != id) )
            continue; 

        // found a noun 
        // this is horrendously bad and will be rewritten, just for the POC 
        // this only supports simple sentences , and only L->R
        bool ok = false;
        std::vector < Block* > joiners = getBlocks ( block.x + 1, block.y );
        for ( Block* join: joiners )
            if ( join->isJoiner() )
                {
                ok = true;
                break;
                }

        if (ok) 
            {
            std::vector < Block* > properties = getBlocks ( block.x + 2, block.y );
            for ( Block* prop: properties )
                {
                const bool isProp = prop->isProperty ();
                const bool isNoun = prop->isNoun ();

                if (isProp || isNoun)
                    {
                    applyRule ( block.id, *prop );
                    // If we've changed one noun into another, we need to reapply the rules for this noun 
                    if (isNoun) 
                        parseRules ( prop->id );
                    }
                }
            }
        }
    }

auto Level::applyRule (BlockID noun, Block& prop) -> void
    {
    std::vector< Block* > toApply = getBlocks ( noun, BlockType::ENTITY );
    if (toApply.empty())
        return;
    for (size_t i = 0; i < toApply.size(); ++i)
        {
        Block* block = toApply[i];
        if ( prop.isNoun() )
            {
            block->id = prop.id; 
            block->removeAllProps();
            }
        else if ( prop.isProperty() )
            {
            // todo: replace this switch with a map between BlockID and Property 
            // some properties might have special logic but we should handle them separately, not by default
            if ( mapBlockToProperty.find(prop.id) != mapBlockToProperty.end() )
                {
                block->addProp ( mapBlockToProperty.at (prop.id) );
                }
            }
        }
    }