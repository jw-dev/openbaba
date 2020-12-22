#include "Input.h"

auto Input::moveDirection () const -> Direction
    {
    return m_moveDirection;
    }

auto Input::quit() const -> bool 
    {
    return m_isQuit;
    }

auto Input::pressed() const -> Key 
    {
    return m_key;
    }

auto Input::moving() const -> bool 
    {
    return m_key == Key::LEFT
        || m_key == Key::RIGHT
        || m_key == Key::UP
        || m_key == Key::DOWN;
    }

auto Input::update() -> void 
    {
    m_key = Key::NONE;
    m_isQuit = false;
    m_moveDirection = Direction::NONE;

    SDL_Event e;
    while ( SDL_PollEvent (&e) ) 
        {
        switch (e.type) 
            {
            case SDL_KEYDOWN: 
                {
                switch (e.key.keysym.sym) 
                    {
                    case SDLK_UP:
                        m_key = Key::UP;
                        m_moveDirection = Direction::UP;
                        break;
                    case SDLK_LEFT:
                        m_key = Key::LEFT;
                        m_moveDirection = Direction::LEFT;
                        break;
                    case SDLK_DOWN:
                        m_key = Key::DOWN;
                        m_moveDirection = Direction::DOWN;
                        break;
                    case SDLK_RIGHT:
                        m_key = Key::RIGHT;
                        m_moveDirection = Direction::RIGHT;
                        break;
                    }
                break;
                }
            case SDL_QUIT:
                {
                m_isQuit = true;
                break;
                }
            default:
                break;
            }
        }
    }