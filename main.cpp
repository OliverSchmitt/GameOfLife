#include <iostream>

#include "GameOfLive.h"

int main( int argc, char** argv ) {
    GameOfLive game;
    game.setFilename( argv[1] );
    game.load();
    game.run();
    return EXIT_SUCCESS;
}