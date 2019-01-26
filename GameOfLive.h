//
// Created by oliver on 10/09/18.
//

#ifndef GAMEOFLIVE_GAMEOFLIVE_H
#define GAMEOFLIVE_GAMEOFLIVE_H

#include <cmath>
#include <fstream>
#include <iostream>
#include <random>
#include<SFML/Graphics.hpp>

enum CellularAutomaton {
    CONWAY,
    COPY,
};

enum State {
    ALIVE,
    DEAD,
};

class GameOfLive {
private:
    // Window constant
    static constexpr const char* WINDOW_TITLE = "SFML";
    static const int WINDOW_WIDTH = 500;
    static const int WINDOW_HEIGHT = 500;
    static const int FRAMERATE_ALIVE = 10;
    static const int FRAMERATE_DEAD = 60;
    // Cell size and the number of cells
    static const int CELL_SIZE = 50;
    static const int WIDTH = WINDOW_WIDTH / CELL_SIZE;
    static const int HEIGHT = WINDOW_HEIGHT / CELL_SIZE;
    // Help message
//    static constexpr const char* FONT_PATH = "/usr/share/fonts/truetype/inconsolata/Inconsolata.otf";
//    static constexpr const char* FONT_PATH = "/usr/share/fonts/truetype/font-awesome/fontawesome-webfont.ttf";
    static constexpr const char* HELP_STRING = "Click or Click and Drag to Toggle the Cells\nPress Space to Start and Stop the Simulation\nPress s to Save the Configuration\nPress r to Reset the Configuration";
//    static const unsigned int CHARACTER_SIZE = WINDOW_WIDTH / 72;

    // Automata options
    static const bool RANDOM_DISTRIBUTION = false;
    static const CellularAutomaton m_automata = CellularAutomaton::CONWAY;
    // Simulation steps
    int m_evolution;

    // To save configuration to
    static constexpr const char* FILENAME = "/home/oliver/Dropbox/CLionProjects/GameOfLive/configurations/CHANGE_ME.live";
    // To save the command line argument of which file to load
    const char* m_filename;

    // For the random distribution
    std::default_random_engine m_generator;
    std::uniform_int_distribution<int> m_distribution;

    // SFML Window
    sf::RenderWindow m_window;
    // Pixels to draw
    sf::VertexArray m_pixels;
    // Saving the cell states
    std::vector<State> m_cells;

    // Running or stopped simulation
    State m_state;

    // Click and drag to mark cells
    bool m_drag;
    std::vector<std::pair<int, int>> m_changedCells;
    bool m_notMoved;
    int m_lastChangedIndex;

    // Help message
//    sf::Font m_font;
//    sf::Text m_text;

    void addPixel( int x, int y );

    int getCellIndex( int x, int y );
    int countNeighbors( int x, int y );
    void setColor( int x, int y, State state );

    bool alreadyChanged( int x, int y );
    void swap( int& x, int& y );
    void bresenhamLow( int x1, int y1, int x2, int y2 );
    void bresenhamHigh( int x1, int y1, int x2, int y2 );
    void bresenham( int index1, int index2 );

    void handleDeadEvents( sf::Event event );
    void handleAliveEvents( sf::Event event );
    void handleEvents( sf::Event event );

    void conwayUpdate( std::vector<State> newCells );
    void copyUpdate( std::vector<State> newCells );
    void update();

    void render();

    void save();
    void reset();

public:
    GameOfLive();

    void load();
    void setFilename( const char* filename );

    void run();
};


#endif //GAMEOFLIVE_GAMEOFLIVE_H
