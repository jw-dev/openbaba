#include "Draw.h"

Editor::Editor ( const Input& input, const Window& win ) 
  : LevelDraw ( input, win ),
    m_paused ( true ),
    m_selectedTile ( nullptr )
    {
    }

auto Editor::doInput ( Level& level ) -> void 
    {
    Key pressed = m_input.pressed ();

    if ( !m_paused ) 
        {
        if ( pressed == Key::PAUSE ) 
            m_paused = !m_paused;
        LevelDraw::doInput ( level );
        return;
        }
  
    switch (pressed) 
        {
        case Key::EDITOR_HEIGHT_MINUS: 
            if ( level.height > 3 )
                level.height--;
            break;
        case Key::EDITOR_HEIGHT_PLUS:
            level.height++;
            break;
        case Key::EDITOR_WIDTH_MINUS:
            if ( level.width > 3 )
                level.width--;
            break;
        case Key::EDITOR_WIDTH_PLUS:
            level.width++;
            break;
        case Key::PAUSE: 
            m_paused = !m_paused;
            break;
        default:
            break;
        }
    for (size_t i = 0; i < level.blocks.size(); ++i)
        {
        Block& check = level.blocks [i];
        if ( check.x >= level.width || check.y >= level.height) 
            {
            level.blocks.erase ( level.blocks.begin() + i );
            }
        }
    refreshCanvas ( level );
    }

auto Editor::tick ( Level& level ) -> bool 
    {
    if ( !m_paused ) 
        {
        level.tick ();
        level.frames += 1;
        level.flags = 0U;
        }
    return false;
    }