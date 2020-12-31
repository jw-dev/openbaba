#include "Level.h" 

Level::Level (int p_id) 
  : id ( p_id ),
    flags ( LEVELFLAG_PARSEWORDS )
    {
    std::ostringstream oss; 
    oss << "Data/"
        << id 
        << ".txt";
    load ( oss.str() );
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
            Block block = makeBlock ( id, type, x, y );
            block.direction = direction;
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
    getRules ();
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
                << (int)block.tile 
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


auto Level::tick () -> bool 
    {
    if ( flags & LEVELFLAG_PARSEWORDS )
        {
        rules.clear();
        getRules ();
        flags &= ~LEVELFLAG_PARSEWORDS;
        }

    // Reset object state
    doReset ();

    // Do transformations 
    doTransformations ();

    // Perform actions 
    doActions ();

    // Do movements (TELE, MOVE, SHIFT)
    doMovements ();

    // Check for win 
    doWinConditions ();

    return flags & LEVELFLAG_WIN;
    }

auto Level::canMove ( Block& block, u8 x, u8 y ) -> bool
    {
    // No movement 
    if ( block.x == x && block.y == y) 
        return false;

    // Would move OOB - integer underflow
    if ( (block.x == 0 && x == UINT8_MAX) || (block.y == 0 && y == UINT8_MAX) )
        return false;

    // Would move OOB - past height/width 
    if ( x >= width || y >= height )
        return false;

    return true;
    }

auto Level::movement ( u8 dir ) -> void 
    {
    std::vector <Block*> toMove; 

    for ( auto& block: blocks )
        if ( block.hasProp ( PROPERTY_YOU )) 
            toMove.push_back (&block);

    //We want to process the movement in the "opposite" direction of movement. 
    //For example, if we are moving LEFT, then we should process movements LEFT->RIGHT, to prevent YOU targets from blocking each other.
    std::sort ( toMove.begin(), toMove.end(), [&dir] (Block* a, Block* b) 
        {
        // return true;
        switch ( dir )
            {
            case DIRECTION_LEFT:    return a->x < b->x; 
            case DIRECTION_RIGHT:   return a->x > b->x;
            case DIRECTION_UP:      return a->y < b->y; 
            case DIRECTION_DOWN:    return a->y > b->y;
            default:                return false;
            }
        });

    for ( auto& block: toMove ) 
        tryMove ( *block, dir );
    }

auto Level::tryMove (Block& block, u8 dir) -> bool
    {
    const u8 oldX = block.x; 
    const u8 oldY = block.y;
    u8 newX = block.x;
    u8 newY = block.y;

    switch (dir) 
        {
        case DIRECTION_UP: 
            newY--;
            break;
        case DIRECTION_DOWN: 
            newY++;
            break;
        case DIRECTION_LEFT: 
            newX--;
            break;
        case DIRECTION_RIGHT:
            newX++;
            break;
        }

    if ( !canMove (block, newX, newY) )
        return false;

    // check all blocks in the way, recursively - if any of them can't move then neither can we 
    for ( auto& toMove: blocks )
        {
        if ( (toMove.x == newX && toMove.y == newY ) )
            {
            // Check SWAP
            if ( toMove.hasProp ( PROPERTY_SWAP ) )
                {
                toMove.x = oldX;
                toMove.y = oldY;
                }

            else if ( !toMove.hasProp ( PROPERTY_YOU ) && ( toMove.hasProp ( PROPERTY_STOP ) || toMove.hasProp ( PROPERTY_PULL ) ) )
                return false; // Oops, we are stopped by that

            // Recursively check all PUSH blocks in the way and see if any of them are stopped 
            else if ( toMove.hasProp ( PROPERTY_PUSH ) )
                if ( !tryMove ( toMove, dir ) )
                    return false;
            }
        }

    // Actually move now
    block.x = newX;
    block.y = newY;
    block.direction = dir;

    // check all blocks that are PULL in the opposite direction of movement 
    u8 checkX = oldX;
    u8 checkY = oldY; 
    for ( ;; )
        {
        bool cont = false;
        checkX += ( oldX - newX );
        checkY += ( oldY - newY );
        
        for ( auto& check: blocks ) 
            {
            if ( check.x == checkX && check.y == checkY && check.hasProp ( PROPERTY_PULL ) )
                {
                check.x -= ( oldX - newX );
                check.y -= ( oldY - newY );
                cont = true;
                }
            }
        if ( !cont )
            break;
        }   

    // When moving words, reparse all tiles to find new potential rules 
    if ( block.tile == TILE_WORD ) 
        flags |= LEVELFLAG_PARSEWORDS;

    return true;
    }

auto Level::getRules () -> void 
    {
    // <CONDITION1,CONDITION2,> [NOUN] [OPERATOR<ARG>] [PROPERTY1<AND PROPERTY2<AND PROPERTYN...>]
    // LONELY AND IDLE BABA NEAR KEKE IS DEFEAT AND WIN
    // CONDITION: LONELY
    // CONDITION: IDLE
    // NOUN: BABA
    // OPERATOR: NEAR [KEKE]
    // PROPERTY: DEFEAT
    // PROPERTY: WIN
    const std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    enum State: u8
        {
        STATE_BEGIN,
        STATE_NOUN,
        STATE_OPERATOR,
        STATE_PROPERTY,
        STATE_FINISH,
        STATE_ERROR,
        };

    for ( const auto& block: blocks )
        {
        // if ( ignore.find (pos) != ignore.end() )
        //     continue; // block has already formed a rule

        for ( u8 xInc: { 0 , 1 })
            for ( u8 yInc: { 0, 1 })
                {
                if ( xInc == yInc ) continue;
                
                Rule rule;
                rule.active = true;

                u8 x = block.x;
                u8 y = block.y;
                u8 state = STATE_BEGIN;
                bool done = false;
                for (;;)
                    {
                    Block * block = nullptr; 
                    // Find block in this position
                    for ( Block& b : blocks )
                        {
                        if ( b.x == x && b.y == y && b.isWord() )
                            {
                            block = &b;
                            break;
                            }
                        }
                    if ( block == nullptr && state != STATE_PROPERTY && state != STATE_FINISH)
                        state = STATE_ERROR;

                    switch (state) 
                        {
                        case STATE_BEGIN: 
                            if ( block->isNoun() )
                                {
                                rule.target = block->id;
                                state = STATE_NOUN;
                                }
                            else 
                                state = STATE_ERROR;
                            break;
                        case STATE_NOUN: 
                            if ( block->isOperator() )
                                {
                                rule.op = block->id;
                                state = STATE_OPERATOR;
                                }
                            else 
                                state = STATE_ERROR;
                            break;
                        case STATE_OPERATOR: 
                            if ( block->isNoun() ) 
                                {
                                rule.flags = TILE_ENTITY;
                                rule.arg = block->id;
                                state = STATE_PROPERTY;
                                }
                            else if ( block->isProp() ) 
                                {
                                rule.flags = TILE_WORD;
                                rule.arg = block->id;
                                state = STATE_PROPERTY;
                                }
                            else 
                                state = STATE_ERROR;
                            break;
                        case STATE_PROPERTY:
                            state = STATE_FINISH;
                            break;
                        case STATE_ERROR:
                            done = true;
                            break;
                        case STATE_FINISH: 
                            addRule ( rule );
                            rule = Rule (); // reset
                            done = true;
                            break;
                        }
                    if ( done ) 
                        break ;
                    // printf ("   x %d, y %d, %s, state %d\n", x, y, block? block->name.c_str(): "<NULL>", state);
                    x += xInc;
                    y += yInc;
                    }
                }
        }

    const std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "parsing finished in " 
              << std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count() 
              << "ms ("
              << std::chrono::duration_cast<std::chrono::microseconds>(end-begin).count()
              << "µs)" 
              << std::endl; 
    }

auto Level::addRule ( Rule& rule ) -> void 
    {
    for ( Rule& other: rules ) 
        {
        if ( other.isNegationOf ( rule )) 
            {
            // We need to disable one of these rules. We should disable the one that is positive rule
            // For example, if we have BABA IS YOU and BABA IS NOT YOU then BABA IS YOU should be disabled
            if ( other.negated ) rule.active = false;
            else other.active = false;
            }
        }
    puts ( "   ===RULE ADDED===  ");
    rules.push_back ( rule );
    }

auto Level::doReset () -> void
    {
    for ( auto& block: blocks ) 
        {
        if ( block.isEntity () )
            {
            block.removeAllProps ();
            }
        }
    }

auto Level::doMovements () -> void 
    {
    // Store all movements in here, so we don't accidentally teleport things more than once 
    struct Tele  
        {
        Block& block; 
        u8 x, y;
        };
    
    std::vector<Tele> teleports;

    const auto isTelePair = [] ( const Block& b1, const Block& b2 ) -> bool
        {
        return b1.isEntity () 
            && b1.hasProp ( PROPERTY_TELE )
            && b2.isEntity () 
            && b2.hasProp ( PROPERTY_TELE )
            && b1.id == b2.id 
            && (b1.x != b2.x || b1.y != b2.y);
        };

    const auto doTele = [&] ( const Block& t1, const Block& t2 )
        {
        for ( auto& target: blocks )
            {
            if (!target.hasProp(PROPERTY_TELE) && ((target.x == t1.x && target.y == t1.y) || (target.x == t2.x && target.y == t2.y)))
                {
                teleports.push_back (Tele 
                    {
                    target, 
                    target.x == t1.x? t2.x: t1.x,
                    target.y == t1.y? t2.y: t1.y
                    });
                }
            }
        };

    for ( const auto& tele1: blocks ) 
        for ( const auto& tele2: blocks )
            {
            if ( isTelePair ( tele1, tele2 ) )
                {
                doTele ( tele1, tele2 );
                }
            }

    if ( !teleports.empty() )
        {
        for ( auto& tele: teleports )
            {
            tele.block.x = tele.x;
            tele.block.y = tele.y;
            }
        }
    }

auto Level::doTransformations () -> void
    {
    for ( auto& block: blocks ) 
        {
        if ( !block.isEntity() ) 
            continue;

        for ( const auto& rule: rules ) 
            {
            if ( !rule.active )
                continue; 

            if ( !(rule.op == WORD_IS && rule.flags == TILE_ENTITY) ) 
                continue; 

            if ( block.id == rule.target )
                {
                Block transform = makeBlock ( rule.arg, TILE_ENTITY );
                block.id = transform.id;
                block.name = transform.name;
                block.rotation = transform.rotation;
                break; // Do not transform more than once
                }
            }
        }
    }

auto Level::doActions () -> void
    {
    for ( const auto& rule: rules ) 
        {
        if ( !rule.active )
            continue;

        if ( !(rule.op == WORD_IS && rule.flags == TILE_WORD) )
            continue; 

        for ( auto& block: blocks ) 
            if ( block.isEntity() && block.id == rule.target) 
                {
                block.addProp ( rule.arg );
                }
        }
    }

auto Level::doWinConditions () -> void
    {

    }
