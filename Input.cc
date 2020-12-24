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
                    case SDLK_o:
                        m_key = Key::EDITOR_HEIGHT_MINUS;
                        break;
                    case SDLK_p:
                        m_key = Key::EDITOR_HEIGHT_PLUS;
                        break;
                    case SDLK_k:
                        m_key = Key::EDITOR_WIDTH_MINUS;
                        break;
                    case SDLK_l:
                        m_key = Key::EDITOR_WIDTH_PLUS;
                        break;
                    case SDLK_SPACE:
                        m_key = Key::PAUSE;
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