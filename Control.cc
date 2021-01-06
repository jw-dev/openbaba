#include "Control.h"

namespace Level 
    {
    Control::Control(Parser* parser, Draw* draw, Input* input, IO* io)
      : m_level (nullptr),
        m_parser (parser),
        m_draw (draw),
        m_input (input),
        m_io (io),
        m_editMode (false)
        {

        }

    auto Control::gameLoop() -> void
        {
        if ( m_level == nullptr)
            {
            m_level = m_io->load("Data/1.txt");
            m_level->rules = m_parser->parse (m_level);
            }
        for (;;)
            {
            // refresh input
            m_input->update ();
            // update game
            update ();
            draw ();
            if ( m_input->pressed(PRESSED_QUIT) )
                break;
            }
        }

    auto Control::draw () -> void 
        {
        // Check if we need to move into edit mode 
        if ( m_input->pressed (PRESSED_PAUSE) )
            {
            m_editMode = !m_editMode; 
            m_draw->toggleMode();
            }

        const auto addEffect = 
            [this] ( const Block& block, u8 prop, ParticleType type, unsigned frames, unsigned ticksPerFrame )
                {
                if ( block.hasProp(prop) && Random::rollOdds ( PARTICLE_SPAWN_CHANCE ) )
                    {
                    m_draw->createParticles ( block.x, block.y, type, frames, ticksPerFrame ); 
                    }
                };

        // Do particle effects 
        for ( const auto& block: m_level->blocks ) 
            {
            addEffect ( block, PROPERTY_WIN, ParticleType::STARS, 4, 7 );
            addEffect ( block, PROPERTY_TELE, ParticleType::BUBBLES, 4, 7 );
            }
    
        m_draw->draw ( m_input, m_level );
        }

    auto Control::update () -> void 
        {
        m_level->frames ++;

        if ( m_editMode )
            return;

        if ( m_input->dir() == DIRECTION_NONE && m_level->frames == 1 )
            return; // No movement and not first frame, nothing to update 

        // Reset all blocks to have no props
        for ( auto& block: m_level->blocks ) 
            if ( block.isEntity() )
                block.removeAllProps ();

        // Iterate rules and apply all 
        for ( const auto &rule: m_level->rules )
            {
            if ( !rule.active )
                continue;
            for ( auto& block: m_level->blocks )
                {
                if ( rule.target != block.id ) 
                    continue;
                if ( !block.isEntity() )
                    continue;
                switch ( rule.op )
                    {
                    case WORD_IS: 
                        if ( rule.flags == TILE_ENTITY ) 
                            {
                            Block transform = makeBlock ( rule.arg, TILE_ENTITY );
                            block.id = transform.id;
                            block.name = transform.name;
                            block.rotation = transform.rotation;
                            }
                        else if ( rule.flags == TILE_WORD )
                            {
                            block.addProp ( rule.arg );
                            }
                        break;
                    default:
                        break;
                    }
                }
            }

        // Move whatever is "YOU"
        move ();

        // Check if we need to reparse level
        if ( m_level->flags & LEVELFLAG_PARSEWORDS )
            {
            m_level->rules = m_parser->parse (m_level);
            m_level->flags &= ~LEVELFLAG_PARSEWORDS;
            }
        }

    auto Control::move () -> void 
        {
        u8 dir = m_input->dir ();
        if ( dir == DIRECTION_NONE )
            return;

        std::vector <Block*> toMove; 

        for ( auto& block: m_level->blocks )
            if ( block.hasProp ( PROPERTY_YOU )) 
                toMove.push_back (&block);

        //We want to process the movement in the "opposite" direction of movement. 
        //For example, if we are moving LEFT, then we should process movements LEFT->RIGHT, to prevent YOU targets from blocking each other.
        std::sort ( toMove.begin(), toMove.end(), [&dir] (Block* a, Block* b) 
            {
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
            m_level->tryMove ( *block, dir );
        }
    }