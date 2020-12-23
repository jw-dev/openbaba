#ifndef DRAW_H
#define DRAW_H

#include "Common.h"
#include "Window.h"
#include "Level.h"
#include "Particle.h"
#include "Random.h"

enum class TextureType
    {
    SPRITES,
    WORDS,
    PARTICLES
    };

struct Canvas 
    {
    int x, y, width, height;
    int cellSize;
    };

class LevelDraw 
    {
public:
    explicit LevelDraw (const Window& win);
    ~LevelDraw ();
    auto draw (const Level& level) -> void;
private: 
    Random m_random;

    // Level data
    int m_levelId;
    unsigned m_animationFrame;

    // Drawing
    std::vector <Particle> m_particles;
    std::map <TextureType, SDL_Texture*> m_textures;
    const Window& m_win;
    Canvas m_canvas;

    // Functions 
    auto refreshCanvas (const Level& level) -> void;
    auto drawBackground () -> void;
    auto drawCanvas () -> void;
    auto drawGrid (const Level& level) -> void;
    auto drawBlocks (const Level& level) -> void;
    auto drawParticleEffects (const Level& level) -> void;
    auto getTexture (TextureType type) -> SDL_Texture*;
    };
#endif