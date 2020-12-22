#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <stdexcept>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Common.h"

class Window 
    {
    public: 
        Window(const std::string& name, int width, int height);
        ~Window();

        // Draw functions 
        void brush (u8 r, u8 g, u8 b);
        void drawLine (int x1, int y1, int x2, int y2);
        void drawFilledRect (int x, int y, int h, int w);

        // Window functions 
        void clear() const;
        void draw();
        bool is_quit() const;

        // Texture functions 
        SDL_Texture* openTexture (const std::string& p) const;
        void drawTexture (SDL_Texture* t, SDL_Rect* src, SDL_Rect* trg, bool flipped) const;

        // Getters 
        int height () const;
        int width () const;
    private:
        bool m_isQuit = false;
        int m_width, m_height;
        std::string m_name;
        SDL_Window* m_window;
        SDL_Renderer* m_renderer;
    };

#endif 