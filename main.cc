#include <stdlib.h>
#include <stdexcept>
#include <iostream>

#include "Window.h"
#include "IO.h"
#include "Parser.h"
#include "Control.h"
#include "Input.h"

static const char *PROGRAM_NAME = "openbaba";
static const int WINDOW_DEFAULT_WIDTH = 800;
static const int WINDOW_DEFAULT_HEIGHT = 600;

int main ( int argc, char** argv ) 
    { 
    Window window ( PROGRAM_NAME, WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT );

    Level::IO io {}; 
    Level::Parser parser {};
    Level::Input input {}; 
    Level::Draw draw { window };
    Level::Control control 
        { 
        &parser, 
        &draw, 
        &input, 
        &io 
        };
    
    control.gameLoop ();
    }