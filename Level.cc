#include "Level.h"

namespace Level 
    {
    auto Level::canMove ( const Block& block, u8 x, u8 y) const -> bool
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

    auto Level::tryMove ( Block& block, u8 dir ) -> bool
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
    }