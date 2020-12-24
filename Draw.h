#ifndef DRAW_H
#define DRAW_H

#include "Common.h"
#include "Window.h"
#include "Level.h"
#include "Particle.h"
#include "Random.h"
#include "Input.h"

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
    explicit LevelDraw (const Input& input, const Window& win);
    ~LevelDraw ();
    auto draw (Level& level) -> bool;
protected: 
    Random m_random;

    // Level data
    int m_levelId;
    unsigned m_animationFrame;

    // Drawing
    std::vector <Particle> m_particles;
    std::map <TextureType, SDL_Texture*> m_textures;
    const Window& m_win;
    const Input& m_input;
    Canvas m_canvas;

    // Functions 
    virtual auto doInput (Level& level) -> void;
    virtual auto tick (Level& level) -> bool;

    auto refreshCanvas (const Level& level) -> void;
    auto drawBackground () -> void;
    auto drawCanvas () -> void;
    auto drawGrid (const Level& level) -> void;
    auto drawBlocks (const Level& level) -> void;
    auto drawParticleEffects (const Level& level) -> void;
    auto getTexture (TextureType type) -> SDL_Texture*;
    };

class Editor : public LevelDraw
    {
public:
    Editor ( const Input& input, const Window& win );
    auto doInput ( Level& level ) -> void override;
    auto tick ( Level& level ) -> bool override;
private:
    bool m_paused;
    Block* m_selectedTile;
    };


#endif