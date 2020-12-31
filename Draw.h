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
    virtual auto draw (Level& level) -> bool;
protected: 
    Random m_random;

    // Level data
    int m_levelId;
    unsigned m_animationFrame;
    bool m_paused;

    // Drawing
    std::vector <Particle> m_particles;
    std::map <TextureType, SDL_Texture*> m_textures;
    const Window& m_win;
    const Input& m_input;
    Canvas m_canvas;

    // Functions 
    virtual auto doInput (Level& level) -> bool;
    virtual auto drawExtra (Level& level) -> void;

    auto createParticleEffect (const Block& block, ParticleType type, unsigned frames, unsigned ticksPerFrame) -> void;
    auto getDirectionInput () const -> u8;
    auto refreshCanvas (const Level& level) -> void;
    auto drawBackground () -> void;
    auto drawCanvas () -> void;
    auto drawGrid (const Level& level) -> void;
    auto drawBlock ( const Block& block ) -> void;
    auto drawBlocks (const Level& level) -> void;
    auto drawParticleEffects (const Level& level) -> void;
    auto getTexture (TextureType type) -> SDL_Texture*;
    };

class Editor: public LevelDraw 
    {
public:
    Editor ( const Input& input, const Window& win );
    auto doInput ( Level& level ) -> bool override;
    auto drawExtra ( Level& level ) -> void override;
private: 
    auto handleResize ( Level& level ) -> void;
    auto handleChangeSprite ( Level& level ) -> void;
    auto handleSave ( Level& level ) -> void;
    
    auto isWithinCanvas ( int x, int y ) const -> bool;
    size_t currentBlock = 0;
    };


#endif