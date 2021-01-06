#include "Draw.h"

namespace Level 
    {
    Draw::Draw (Window& window)
      : m_animationFrame (0U),
        m_paused (false),
        m_win (window)
        {
        m_textures [TextureType::PARTICLES] = m_win.openTexture (PARTICLE_TEXTURE_PATH);
        m_textures [TextureType::SPRITES] = m_win.openTexture (SPRITE_TEXTURE_PATH);
        m_textures [TextureType::WORDS] = m_win.openTexture (WORD_TEXTURE_PATH);
        }

    Draw::~Draw ()
        {
        for ( auto& kv: m_textures )
            {
            if ( kv.second )
                {
                SDL_DestroyTexture ( kv.second );
                }
            }
        }

    auto Draw::getTexture (TextureType type) -> SDL_Texture*
        {
        if ( m_textures.find(type) == m_textures.end() || m_textures.at(type) == nullptr )
            {
            throw std::runtime_error ( "texture has not been initialized" );
            }
        return m_textures.at(type);
        }

    auto Draw::refreshCanvas ( u8 width, u8 height ) -> void 
        {
        const unsigned padding = m_mode == DrawMode::EDIT ? PADDING * 5: PADDING;
        const int wWidth = m_win.width();
        const int wHeight = m_win.height();
        const int widthCellSize = (wWidth - padding) / width;
        const int heightCellSize = (wHeight - padding) / height;
        m_canvas.cellSize = widthCellSize < heightCellSize ? widthCellSize : heightCellSize;
        m_canvas.width = m_canvas.cellSize * width;
        m_canvas.height = m_canvas.cellSize * height;
        m_canvas.x = (wWidth / 2) - (m_canvas.width / 2);
        m_canvas.y = (wHeight / 2) - (m_canvas.height / 2);
        }

    auto Draw::drawBackground() -> void
        {
        m_win.brush ( 0, 21, 64 );
        m_win.clear ();
        }

    auto Draw::drawCanvas() -> void
        {
        m_win.brush ( 0, 28, 87 );
        m_win.drawFilledRect ( m_canvas.x, m_canvas.y, m_canvas.width, m_canvas.height);
        }

    auto Draw::drawGrid (u8 width, u8 height) -> void
        {
        switch ( m_mode ) // Only draw grid in edit mode
            {
            case DrawMode::EDIT: 
                m_win.brush ( 0, 49, 110 );
                for (int column = 0; column <= width; ++column)
                    m_win.drawLine (m_canvas.x+column*m_canvas.cellSize, m_canvas.y, m_canvas.x+column*m_canvas.cellSize, m_canvas.y+m_canvas.height);
                for (int row = 0; row <= height; ++row) 
                    m_win.drawLine (m_canvas.x, m_canvas.y+row*m_canvas.cellSize, m_canvas.x+m_canvas.width, m_canvas.y+row*m_canvas.cellSize);
                break;
            default:
                break;
            }
        }

    auto Draw::drawBlock ( const Block& block ) -> void
        {
        SDL_Texture * texture;
        SDL_Rect src, trg;
        bool flipped = false;

        if (block.tile == TILE_WORD)
            {
            src.x = SPRITE_SIZE*m_animationFrame;
            src.w = SPRITE_SIZE;
            src.y = block.id * SPRITE_SIZE;
            src.h = SPRITE_SIZE;
            texture = getTexture ( TextureType::WORDS );
            }
        else 
            {
            unsigned int frameAdd = 0U;
            if (block.rotation)
                {
                frameAdd = block.direction;
                if (block.direction == DIRECTION_LEFT)
                    {
                    frameAdd = 0;
                    flipped = true;
                    }
                } 
            
            src.x = (frameAdd*SPRITE_SIZE)+(3*SPRITE_SIZE*m_animationFrame);
            src.w = SPRITE_SIZE;
            src.y = block.id * SPRITE_SIZE;
            src.h = SPRITE_SIZE;
            texture = getTexture ( TextureType::SPRITES );
            }
        trg.x = m_canvas.x+block.x*m_canvas.cellSize;
        trg.y = m_canvas.y+block.y*m_canvas.cellSize;
        trg.w = m_canvas.cellSize;
        trg.h = m_canvas.cellSize;
        m_win.drawTexture(texture, &src, &trg, flipped);
        }

    auto Draw::drawBlocks (const std::vector<Block> &blocks) -> void 
        {
        for ( auto i = blocks.crbegin(); i < blocks.crend(); ++i )
            {
            drawBlock ( *i );
            }
        }

    auto Draw::createParticles ( u8 x, u8 y, ParticleType type, unsigned frames, unsigned ticksPerFrame ) -> void
        {
        Particle p; 
        p.x = (m_canvas.x + m_canvas.cellSize*x) + Random::getRandomInt ( 0, m_canvas.cellSize );
        p.y = (m_canvas.y + m_canvas.cellSize*y) + Random::getRandomInt ( 0, m_canvas.cellSize );
        p.type = type; 
        p.frames = frames;
        p.ticksPerFrame = ticksPerFrame;
        m_particles.push_back ( p );
        }

    auto Draw::drawParticleEffects () -> void
        {   
        SDL_Texture* texture = getTexture (TextureType::PARTICLES);

        for ( size_t i = 0; i < m_particles.size(); ++i ) 
            {
            Particle * part = &m_particles[i];
            part->ticks++;
            if (part->ticks % part->ticksPerFrame == 0) 
                {
                part->currentFrame++;
                if (part->currentFrame > part->frames) 
                    {
                    m_particles.erase(m_particles.begin() + i);
                    continue;
                    }
                }
            // Draw 
            SDL_Rect src, trg; 
            src.x = part->currentFrame * PARTICLE_SIZE;
            src.y = (int)part->type * PARTICLE_SIZE;
            src.w = PARTICLE_SIZE;
            src.h = PARTICLE_SIZE;
            trg.w = m_canvas.cellSize / 2;
            trg.h = m_canvas.cellSize / 2;
            trg.x = part->x - m_canvas.cellSize / 4;
            trg.y = part->y - m_canvas.cellSize / 4;
            m_win.drawTexture ( texture, &src, &trg, false );
            }
        }

    auto Draw::toggleMode () -> void
        {
        m_mode = ( m_mode == DrawMode::EDIT ? DrawMode::GAME : DrawMode::EDIT );
        }

    auto Draw::drawEditorControls (const Input* input, Level* level) -> void
        {
        if ( m_mode != DrawMode::EDIT )
            return;
        }

    auto Draw::draw (const Input* input, Level* level) -> void
        {
        if ( m_mode == DrawMode::GAME 
          && level->frames % ANIMATION_TIMER == 0 )
            {
            m_animationFrame++;
            m_animationFrame%=3;
            }
        m_win.clear ();
        refreshCanvas ( level->width, level->height);
        drawBackground ();
        drawCanvas ();
        drawGrid ( level->width, level->height );
        drawBlocks ( level->blocks );
        drawParticleEffects ();
        drawEditorControls (input, level);
        m_win.draw ();
        }
    }