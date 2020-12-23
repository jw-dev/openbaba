#include <stdlib.h>
#include <stdexcept>
#include <iostream>
#include "Common.h"
#include "Window.h"
#include "Draw.h"
#include "Level.h"
#include "Input.h"

static const char *PROGRAM_NAME = "openbaba";
static const int WINDOW_DEFAULT_WIDTH = 800;
static const int WINDOW_DEFAULT_HEIGHT = 600;

int main ( ) 
    {
    Window window ( PROGRAM_NAME, WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT );
    Input input;
    Level level (1);
    LevelDraw levelDraw(window);

    bool quit = false;
    while (!quit)
        {
        input.update ();
        bool win = level.update (input);
        levelDraw.draw (level);

        if ( win || input.quit() ) 
            quit = true;
        }
    }