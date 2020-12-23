#include "Draw.h"

LevelDraw::LevelDraw (Window* win) 
  : m_win (win)
    {
    m_animationFrame = 0U;
    m_startTime = 0U;
    m_levelId = -1;
    m_canvasX = 0;
    m_canvasY = 0;
    m_canvasWidth = 0;
    m_canvasHeight = 0;
    m_cellSize = 0;
    m_words = win->openTexture (WORD_TEXTURE_PATH);
    m_sprites = win->openTexture (SPRITE_TEXTURE_PATH);
    m_parts = win->openTexture (PARTICLE_TEXTURE_PATH);
    }

auto LevelDraw::refreshCanvas ( const Level& level ) -> void 
    {
    const int wWidth = m_win->width();
    const int wHeight = m_win->height();
    const int widthCellSize = (wWidth - PADDING) / level.width;
    const int heightCellSize = (wHeight - PADDING) / level.height;
    m_cellSize = widthCellSize < heightCellSize ? widthCellSize : heightCellSize;
    m_canvasWidth = m_cellSize * level.width;
    m_canvasHeight = m_cellSize * level.height;
    m_canvasX = (wWidth / 2) - (m_canvasWidth / 2);
    m_canvasY = (wHeight / 2) - (m_canvasHeight / 2);
    }

auto LevelDraw::drawBackground() -> void
    {
    m_win->brush ( 0, 21, 64 );
    m_win->clear ();
    }

auto LevelDraw::drawCanvas() -> void
    {
    m_win->brush ( 0, 28, 87 );
    m_win->drawFilledRect ( m_canvasX, m_canvasY, m_canvasWidth, m_canvasHeight);
    }

auto LevelDraw::drawGrid(const Level& level) -> void
    {
    m_win->brush ( 0, 49, 110 );
    for (int column = 0; column <= level.width; ++column)
        m_win->drawLine (m_canvasX+column*m_cellSize, m_canvasY, m_canvasX+column*m_cellSize, m_canvasY+m_canvasHeight);
    for (int row = 0; row <= level.height; ++row) 
        m_win->drawLine (m_canvasX, m_canvasY+row*m_cellSize, m_canvasX+m_canvasWidth, m_canvasY+row*m_cellSize);
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
            texture = m_words;
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
            texture = m_sprites;
            }
        trg.x = m_canvasX+block.x*m_cellSize;
        trg.y = m_canvasY+block.y*m_cellSize;
        trg.w = m_cellSize;
        trg.h = m_cellSize;
        m_win->drawTexture(texture, &src, &trg, flipped);
        }
    }

auto LevelDraw::drawParticleEffects (const Level& level) -> void
    {
    for (const Block& block: level.blocks) 
        {
        // todo move this into a vector of Properties -> Particles, with particle data
        if ( block.hasProp (Property::WIN )) 
            {
            if ( m_random.rollOdds ( PARTICLE_SPAWN_CHANCE ) ) 
                {
                Particle p; 
                p.x = (m_canvasX + m_cellSize*block.x) + m_random.getRandomInt ( 0, m_cellSize );
                p.y = (m_canvasY + m_cellSize*block.y) + m_random.getRandomInt ( 0, m_cellSize );
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
        trg.w = m_cellSize / 2;
        trg.h = m_cellSize / 2;
        trg.x = part->x - m_cellSize / 4;
        trg.y = part->y - m_cellSize / 4;
        m_win->drawTexture ( m_parts, &src, &trg, false );
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