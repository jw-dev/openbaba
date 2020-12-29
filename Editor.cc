// #include "Draw.h"

#include "Draw.h"


Editor::Editor ( const Input& input, const Window& win )
  : LevelDraw ( input, win )
    {
    m_paused = true;
    }

auto Editor::handleResize ( Level& level ) -> void
    {
    if      ( m_input.pressed ( PRESSED_HEIGHT_PLUS ))  level.height++;
    else if ( m_input.pressed ( PRESSED_HEIGHT_MINUS )) level.height--;
    else if ( m_input.pressed ( PRESSED_WIDTH_PLUS ))   level.width++;
    else if ( m_input.pressed ( PRESSED_WIDTH_MINUS ))  level.width--;
    else 
        return;
    // Resized editor; remove any blocks 
    for ( size_t i = level.blocks.size() - 1; i; --i )
        {
        const auto& block = level.blocks.at (i);
        if ( block.x >= level.width || block.y >= level.height )
            {
            level.blocks.erase ( level.blocks.begin() + i );
            }
        }
    // Fix canvas
    refreshCanvas ( level );
    }

auto Editor::handleChangeSprite ( Level& level ) -> void
    {
    if ( m_input.pressed ( PRESSED_SCROLLUP ))
        {
        if ( currentBlock < countBlocks() - 1 )
            currentBlock++;
        }
    else if ( m_input.pressed ( PRESSED_SCROLLDOWN ))
        {
        if ( currentBlock > 0 ) 
            currentBlock--;
        }
    }

auto Editor::handleSave (Level& level) -> void 
    {
    if ( m_input.pressed ( PRESSED_SAVE ))
        {
        std::ostringstream oss; 
        oss << "Data/"
            << level.id 
            << ".txt";
        level.save ( oss.str() );
        std::cout << "Saved: " << oss.str () << std::endl;
        }
    }

auto Editor::doInput ( Level& level ) -> bool
    {
    if ( m_input.pressed ( PRESSED_PAUSE )) 
        {
        m_paused = !m_paused;
        level.tick();
        }

    if ( !m_paused )
        {
        return LevelDraw::doInput ( level );
        }

    handleResize (level);
    handleChangeSprite (level);
    handleSave (level);
    return false;
    }

auto Editor::drawExtra ( Level& level ) -> void
    {
    if ( !m_paused ) 
        return; 

    const int mouseX = m_input.x();
    const int mouseY = m_input.y();
    if ( isWithinCanvas ( mouseX, mouseY ) )
        {
        const u8 gridX = (mouseX - m_canvas.x) / m_canvas.cellSize;
        const u8 gridY = (mouseY - m_canvas.y) / m_canvas.cellSize;

        Block block = getBlock ( currentBlock );
        block.x = gridX;
        block.y = gridY;
        drawBlock ( block );

        if ( m_input.pressed ( PRESSED_LEFTCLICK )) 
            {
            // place new block down 
            level.blocks.push_back ( block );
            level.flags |= LEVELFLAG_PARSEWORDS;
            }
        else if ( m_input.pressed ( PRESSED_RIGHTCLICK )) 
            {
            for ( int i = level.blocks.size() - 1; i >= 0; --i) 
                {
                Block& erase = level.blocks.at (i);
                if ( erase.x == gridX && erase.y == gridY )
                    {
                    level.blocks.erase ( level.blocks.begin() + i );
                    }
                }
            }
        }
    }

auto Editor::isWithinCanvas ( int x, int y ) const -> bool
    {
    return x > m_canvas.x
        && x < m_canvas.x + m_canvas.width 
        && y > m_canvas.y 
        && y < m_canvas.y + m_canvas.height;
    }