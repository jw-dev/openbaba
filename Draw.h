#ifndef DRAW_H
#define DRAW_H

#include "Common.h"
#include "Window.h"
#include "Level.h"
#include "Particle.h"
#include "Random.h"
#include "Input.h"

namespace Level 
    {
    enum class TextureType
        {
        SPRITES,
        WORDS,
        PARTICLES
        };

    enum class DrawMode 
        {
        GAME,
        EDIT,
        };

    struct Canvas 
        {
        int x, y, width, height;
        int cellSize;
        };

    class Draw 
        {
    public: 
        Draw (Window& window);
        ~Draw ();

        virtual auto toggleMode () -> void;
        virtual auto draw ( const Input* input, Level* level ) -> void;
        virtual auto createParticles ( u8 x, u8 y, ParticleType type, unsigned frames, unsigned ticksPerFrame ) -> void;
    private:
         // Level data
        unsigned m_animationFrame;
        bool m_paused;
        DrawMode m_mode = DrawMode::GAME;

        // Drawing
        std::vector <Particle> m_particles;
        std::map <TextureType, SDL_Texture*> m_textures;
        Canvas m_canvas;
        Window& m_win;

        // Functions 
        auto refreshCanvas ( u8 width, u8 height ) -> void;
        auto getDirectionInput () const -> u8;
        auto getTexture (TextureType type) -> SDL_Texture*;
        
        auto drawBlock ( const Block& block ) -> void;
        auto drawBlocks ( const std::vector<Block>& blocks ) -> void;
        auto drawParticleEffects () -> void;
        auto drawBackground () -> void;
        auto drawCanvas () -> void;
        auto drawGrid (u8 width, u8 height) -> void;
        auto drawEditorControls (const Input* input, Level* level) -> void;
        };

    }
#endif