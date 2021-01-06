#include "Input.h"

namespace Level 
    {

    auto Input::update () -> void
        {
        u32 pressed = 0U;
        SDL_Event e; 

        auto pressKey = [&pressed] (u32 key) 
            {
            if ( sdlkToBabaMap.find (key) != sdlkToBabaMap.end() )
                pressed |= sdlkToBabaMap.at ( key );
            };

        while ( SDL_PollEvent (&e) )
            {
            switch (e.type) 
                {
                case SDL_KEYDOWN: 
                    pressKey ( e.key.keysym.sym );
                    break;
                case SDL_MOUSEWHEEL:
                    if ( e.wheel.y > 0 ) 
                        pressed |= PRESSED_SCROLLUP;
                    else if ( e.wheel.y < 0 )
                        pressed |= PRESSED_SCROLLDOWN;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    switch ( e.button.button ) 
                        {
                        case SDL_BUTTON_LEFT: 
                            pressed |= PRESSED_LEFTCLICK;
                            break;
                        case SDL_BUTTON_RIGHT: 
                            pressed |= PRESSED_RIGHTCLICK;
                            break;
                        default:
                            break;
                        }
                    break;
                case SDL_MOUSEMOTION:
                    // m_x = e.motion.x;
                    // m_y = e.motion.y;
                    break;
                case SDL_QUIT:
                    pressed |= PRESSED_QUIT;
                }
            }
        if ( pressed != m_pressed )
            {
            m_pressed = pressed;
            }
        }

    auto Input::pressed ( u32 press ) const -> bool 
        {
        return press & m_pressed;
        }

    auto Input::dir() const -> u8 
        {
        if      ( pressed (PRESSED_LEFT) )      return DIRECTION_LEFT;
        else if ( pressed ( PRESSED_RIGHT ) )   return DIRECTION_RIGHT;
        else if ( pressed ( PRESSED_UP ) )      return DIRECTION_UP;
        else if ( pressed ( PRESSED_DOWN ) )    return DIRECTION_DOWN;
        else                                    return DIRECTION_NONE;
        }
    }

// auto Input::update () -> void 
//     {
//     
//     }