#include "Window.h"

Window::Window(const std::string& name, int width, int height)
  : m_name (name),
    m_width (width),
    m_height (height)
    {
    SDL_Init(SDL_INIT_VIDEO);

    m_window = SDL_CreateWindow(
        name.c_str(),
        0,
        0,
        width,
        height,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    m_renderer = SDL_CreateRenderer(
        m_window,
        -1,
        SDL_RENDERER_ACCELERATED);
    }

Window::~Window()
    {
    SDL_DestroyWindow(m_window);
    SDL_DestroyRenderer(m_renderer);
    SDL_Quit();
    }

void Window::clear() const
    {
    SDL_RenderClear(m_renderer);
    }

int Window::height () const
    {
    return m_height;
    }

int Window::width () const
    {
    return m_width;
    }

void Window::brush (u8 r, u8 g, u8 b)
    {
    SDL_SetRenderDrawColor(
        m_renderer,
        r,
        g,
        b,
        SDL_ALPHA_OPAQUE);
    }

void Window::drawLine (int x1, int y1, int x2, int y2)
    {
    SDL_RenderDrawLine(
        m_renderer,
        x1,
        y1,
        x2,
        y2);
    }

void Window::drawFilledRect (int x, int y, int w, int h)
    {
    SDL_Rect rect; 
    rect.x = x;
    rect.y = y;
    rect.h = h;
    rect.w = w;
    SDL_RenderFillRect(m_renderer, &rect);
    }

void Window::draw()
    {
    SDL_RenderPresent(m_renderer);
    }

SDL_Texture* Window::openTexture(const std::string& p) const
    {
    SDL_Texture *texture = nullptr;
    SDL_Surface *surface = IMG_Load(p.c_str());
    if (surface) 
        {
        texture = SDL_CreateTextureFromSurface (m_renderer, surface);
        SDL_FreeSurface (surface);
        }
    return texture;
    }

void Window::drawTexture (SDL_Texture* t, SDL_Rect* src, SDL_Rect* trg, bool flipped) const
    {
    if (!t)
        throw std::runtime_error ("null texture");
    SDL_RenderCopyEx (
        m_renderer,
        t,
        src,
        trg,
        0.0,
        NULL,
        flipped? SDL_FLIP_HORIZONTAL: SDL_FLIP_NONE);
    }

bool Window::is_quit() const 
    {
    return m_isQuit;
    }