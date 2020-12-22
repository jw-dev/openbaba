#ifndef DRAW_H
#define DRAW_H

#include "Window.h"
#include "Level.h"

class LevelDraw 
    {
public:
    explicit LevelDraw (Window* win);
    auto draw (const Level& level) -> void;
private: 
    // Constants 
    static const unsigned SPRITE_SIZE = 24U;
    static const unsigned PADDING = 25U;
    static const unsigned ANIMATION_TIMER = 10U;
    const char * const WORD_TEXTURE_PATH = "Data/words.png";
    const char * const SPRITE_TEXTURE_PATH = "Data/sprites.png";

    // Functions 
    auto refreshCanvas (const Level& level) -> void;
    auto drawBackground () -> void;
    auto drawCanvas () -> void;
    auto drawGrid (const Level& level) -> void;
    auto drawBlocks (const Level& level) -> void;
    auto drawParticleEffects (const Level& level) -> void;
    
    // Level data
    int m_levelId;
    unsigned m_animationFrame;
    unsigned m_startTime;
    
    // Textures 
    SDL_Texture * m_words = nullptr;
    SDL_Texture * m_sprites = nullptr;

    // Canvas 
    int m_canvasX, m_canvasY;
    int m_canvasWidth, m_canvasHeight;
    int m_cellSize;

    // Window 
    Window * m_win;
    };
#endif