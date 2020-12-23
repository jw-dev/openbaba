#include "Draw.h"

LevelDraw::LevelDraw (Window* win) 
  : m_win (win)
    {
    m_animationFrame = 0U;
    m_levelId = -1;
    m_textures [TextureType::PARTICLES] = win->openTexture (PARTICLE_TEXTURE_PATH);
    m_textures [TextureType::SPRITES] = win->openTexture (SPRITE_TEXTURE_PATH);
    m_textures [TextureType::WORDS] = win->openTexture (WORD_TEXTURE_PATH);
    }

LevelDraw::~LevelDraw ()
    {
    for ( auto& kv: m_textures )
        {
        if ( kv.second )
            {
            SDL_DestroyTexture ( kv.second );
            }
        }
    }

auto LevelDraw::getTexture (TextureType type) -> SDL_Texture*
    {
    if ( m_textures.find(type) == m_textures.end() || m_textures.at(type) == nullptr )
        {
        throw std::runtime_error ( "texture has not been initialized" );
        }
    return m_textures.at(type);
    }

auto LevelDraw::refreshCanvas ( const Level& level ) -> void 
    {
    const int wWidth = m_win->width();
    const int wHeight = m_win->height();
    const int widthCellSize = (wWidth - PADDING) / level.width;
    const int heightCellSize = (wHeight - PADDING) / level.height;
    m_canvas.cellSize = widthCellSize < heightCellSize ? widthCellSize : heightCellSize;
    m_canvas.width = m_canvas.cellSize * level.width;
    m_canvas.height = m_canvas.cellSize * level.height;
    m_canvas.x = (wWidth / 2) - (m_canvas.width / 2);
    m_canvas.y = (wHeight / 2) - (m_canvas.height / 2);
    }

auto LevelDraw::drawBackground() -> void
    {
    m_win->brush ( 0, 21, 64 );
    m_win->clear ();
    }

auto LevelDraw::drawCanvas() -> void
    {
    m_win->brush ( 0, 28, 87 );
    m_win->drawFilledRect ( m_canvas.x, m_canvas.y, m_canvas.width, m_canvas.height);
    }

auto LevelDraw::drawGrid(const Level& level) -> void
    {
    m_win->brush ( 0, 49, 110 );
    for (int column = 0; column <= level.width; ++column)
        m_win->drawLine (m_canvas.x+column*m_canvas.cellSize, m_canvas.y, m_canvas.x+column*m_canvas.cellSize, m_canvas.y+m_canvas.height);
    for (int row = 0; row <= level.height; ++row) 
        m_win->drawLine (m_canvas.x, m_canvas.y+row*m_canvas.cellSize, m_canvas.x+m_canvas.width, m_canvas.y+row*m_canvas.cellSize);
    }

auto LevelDraw::drawBlocks (const Level& level) -> void 
    {
    for (const Block& block: level.blocks)
        {
        const int id = (int)block.id;
        const BlockType type = block.type;
        const Direction direction = block.direction;
        SDL_Texture * texture;
        SDL_Rect src, trg;
        bool flipped = false;

        if (type == BlockType::WORD)
            {
            src.x = SPRITE_SIZE*m_animationFrame;
            src.w = SPRITE_SIZE;
            src.y = id * SPRITE_SIZE;
            src.h = SPRITE_SIZE;
            texture = getTexture ( TextureType::WORDS );
            }
        else 
            {
            unsigned int frameAdd = (unsigned) direction;
            if (direction == Direction::LEFT)
                {
                frameAdd = 0;
                flipped = true;
                }
            src.x = (frameAdd*SPRITE_SIZE)+(3*SPRITE_SIZE*m_animationFrame);
            src.w = SPRITE_SIZE;
            src.y = id * SPRITE_SIZE;
            src.h = SPRITE_SIZE;
            texture = getTexture ( TextureType::SPRITES );
            }
        trg.x = m_canvas.x+block.x*m_canvas.cellSize;
        trg.y = m_canvas.y+block.y*m_canvas.cellSize;
        trg.w = m_canvas.cellSize;
        trg.h = m_canvas.cellSize;
        m_win->drawTexture(texture, &src, &trg, flipped);
        }
    }

auto LevelDraw::drawParticleEffects (const Level& level) -> void
    {   
    SDL_Texture* texture = getTexture (TextureType::PARTICLES);

    for (const Block& block: level.blocks) 
        {
        // todo move this into a vector of Properties -> Particles, with particle data
        if ( block.hasProp (Property::WIN )) 
            {
            if ( m_random.rollOdds ( PARTICLE_SPAWN_CHANCE ) ) 
                {
                Particle p; 
                p.x = (m_canvas.x + m_canvas.cellSize*block.x) + m_random.getRandomInt ( 0, m_canvas.cellSize );
                p.y = (m_canvas.y + m_canvas.cellSize*block.y) + m_random.getRandomInt ( 0, m_canvas.cellSize );
                p.type = ParticleType::STARS;
                p.frames = 5;
                p.ticksPerFrame = 10;
                m_particles.push_back ( p );
                }
            }
        }
    // Manage existing particles
    for ( int i = 0; i < m_particles.size(); ++i ) 
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
        m_win->drawTexture ( texture, &src, &trg, false );
        }
    }

auto LevelDraw::draw(const Level& level) -> void 
    {
    if (m_levelId == -1 || level.id != m_levelId)
        {
        refreshCanvas(level);
        }

    m_win->clear();
    drawBackground ();
    drawCanvas ();
    drawGrid (level);
    drawBlocks (level);
    drawParticleEffects (level);
    m_win->draw();

    if (level.frames % ANIMATION_TIMER == 0)
        {
        m_animationFrame++;
        m_animationFrame%=3;
        }
    SDL_Delay (16.66);
    }