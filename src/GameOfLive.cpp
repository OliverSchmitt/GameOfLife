#include "GameOfLive.h"

GameOfLive::GameOfLive() {
    for( int cell = 0; cell < NUM_CELLS * NUM_CELLS; cell++ )
        m_pixels[ cell ] = DEAD;

    m_pixels[ 12 ] = ALIVE;
}

int GameOfLive::countNeighbors( int cell ) {
    int left = -1;
    int right = 2;
    int top = -1;
    int bottom = 2;
    if( cell % NUM_CELLS == 0 )
        left = 0;
    if( cell % NUM_CELLS == NUM_CELLS - 1 )
        right = 1;
    if( cell / NUM_CELLS == 0 )
        top = 0;
    if( cell / NUM_CELLS == NUM_CELLS - 1 )
        bottom = 1;

    int neighbors = 0;
    for( int y = top; y < bottom; y++ ) {
        for( int x = left; x < right; x++ ) {
            if( ( x != 0 || y != 0 ) && m_pixels[ cell + x + ( y * NUM_CELLS ) ] == ALIVE )
                neighbors++;
        }
    }
    return neighbors;
}

void GameOfLive::init() {
    m_window.create( sf::VideoMode( WINDOW_WIDTH, WINDOW_WIDTH ), WINDOW_TITLE );
    m_window.setFramerateLimit( FRAMERATE );
}

void GameOfLive::update() {
    for( int cell = 0; cell < NUM_CELLS * NUM_CELLS; cell++ ) {
        int neighbors = countNeighbors( cell );
        std::cout << "cell: " << cell << " - " << neighbors << std::endl;
        if( m_pixels[ cell ] == DEAD && neighbors == 3 )
            m_pixels[ cell ] = ALIVE;
        if( m_pixels[ cell ] == ALIVE && ( neighbors < 2 || neighbors > 3 ) )
            m_pixels[ cell ] = DEAD;
    }
}

void GameOfLive::render() {
    m_window.clear();

    sf::Uint8 pixels[ ( NUM_CELLS * CELL_SIZE * 4 ) * ( NUM_CELLS * CELL_SIZE * 4 ) ];
    for( int cell = 0; cell < NUM_CELLS * NUM_CELLS; cell++ ) {
        sf::Uint8 color;
        if( m_pixels[cell] == DEAD)
            color = 255;
        else
            color = 0;

        int yOffset = (int) ( cell / NUM_CELLS ) * ( CELL_SIZE * NUM_CELLS * CELL_SIZE * 4 );
        int xOffset = ( cell % NUM_CELLS ) * ( CELL_SIZE * 4 );
        int offset = xOffset + yOffset;
        for( int y = 0; y < CELL_SIZE; y++ ) {
            for( int x = 0; x < CELL_SIZE; x++ ) {
                for( int j = 0; j < 3; j++ ) {
                    pixels[ offset + ( y * 4 * NUM_CELLS * CELL_SIZE ) + ( x * 4 ) + j] = color;
                }
                pixels[ offset + ( y * 4 * NUM_CELLS * CELL_SIZE ) + ( x * 4 ) + 3] = 255;
            }
        }
    }

    m_image.create( NUM_CELLS * CELL_SIZE, NUM_CELLS * CELL_SIZE, pixels );
    m_texture.loadFromImage( m_image );
    m_sprite.setTexture( m_texture );
    m_window.draw( m_sprite );

    m_window.display();
}

void GameOfLive::run() {
    init();

    while( m_window.isOpen() ) {
        sf::Event event;
        while( m_window.pollEvent( event ) )
            if( event.type == sf::Event::Closed )
                m_window.close();

        update();
        render();
    }
}

