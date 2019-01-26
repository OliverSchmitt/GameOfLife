//
// Created by oliver on 10/09/18.
//

#include "GameOfLive.h"

GameOfLive::GameOfLive() : m_state( State::DEAD ), m_distribution( 0, 1 ), m_drag( false ), m_notMoved( true ), m_lastChangedIndex( -1 ), m_evolution( 0 ) {
    m_window.create( sf::VideoMode( WINDOW_WIDTH, WINDOW_HEIGHT ), WINDOW_TITLE );
    m_window.setFramerateLimit( FRAMERATE_DEAD );

    m_pixels.setPrimitiveType( sf::PrimitiveType::Quads );

    for( int y = 0; y < HEIGHT; y++ )
        for( int x = 0; x < WIDTH; x++ )
            addPixel( x, y );

//    if( !m_font.loadFromFile( FONT_PATH ) )
//        std::cout << "Font could not be loaded"<< std::endl;

//    m_text.setString( HELP_STRING );
//    m_text.setFont( m_font );
//    m_text.setFillColor(( sf::Color::White ) );
//    m_text.setOutlineColor( sf::Color::Black );
//    m_text.setOutlineThickness( 1.0f );
//    m_text.setPosition( 0.01f * WINDOW_WIDTH, 0.01f * WINDOW_HEIGHT );
//    m_text.setCharacterSize( CHARACTER_SIZE );
//    std::cout << (std::string) m_text.getString() << std::endl;
    std::cout << HELP_STRING << std::endl;
}

void GameOfLive::addPixel(int x, int y) {
    sf::Vertex topLeft;
    sf::Vertex bottomLeft;
    sf::Vertex bottomRight;
    sf::Vertex topRight;

    topLeft.position = sf::Vector2f    ( x * CELL_SIZE + 1    , y * CELL_SIZE + 1 );
    bottomLeft.position = sf::Vector2f ( x * CELL_SIZE + 1    , ( y + 1 ) * CELL_SIZE );
    bottomRight.position = sf::Vector2f( ( x + 1 ) * CELL_SIZE, ( y + 1 ) * CELL_SIZE );
    topRight.position = sf::Vector2f   ( ( x + 1 ) * CELL_SIZE, y * CELL_SIZE + 1 );

    sf::Color color;
    int c = m_distribution( m_generator );
    color = ( c == 0 && RANDOM_DISTRIBUTION ) ? sf::Color::Black : sf::Color::White;
    topLeft.color = color;
    bottomLeft.color = color;
    bottomRight.color = color;
    topRight.color = color;

    m_pixels.append( topLeft );
    m_pixels.append( bottomLeft );
    m_pixels.append( bottomRight );
    m_pixels.append( topRight );

    m_cells.push_back( (c == 0 && RANDOM_DISTRIBUTION ) ? State::ALIVE : State::DEAD );
}

int GameOfLive::getCellIndex(int x, int y) {
    return y * WIDTH + x;
}

int GameOfLive::countNeighbors( int x, int y ) {
    int left = (x == 0) ? 0 : -1;
    int right = (x == WIDTH - 1) ? 1 : 2;
    int top = (y == 0) ? 0 : -1;
    int bottom = (y == HEIGHT - 1) ? 1 : 2;


    int neighbors = 0;
    for (int dy = top; dy < bottom; dy++) {
        for (int dx = left; dx < right; dx++) {
            if( dx == 0 && dy == 0 )
                continue;
            if( m_cells[getCellIndex(x + dx, y + dy)] == State::ALIVE)
                neighbors++;
        }
    }
    return neighbors;
}

void GameOfLive::setColor( int x, int y, State state ) {
    int index = getCellIndex( x, y ) * 4;
    sf::Color color = ( state == State::ALIVE ) ? sf::Color::Black : sf::Color:: White;
    m_pixels[ index     ].color = color;
    m_pixels[ index + 1 ].color = color;
    m_pixels[ index + 2 ].color = color;
    m_pixels[ index + 3 ].color = color;
}

bool GameOfLive::alreadyChanged( int x, int y ) {
    int n = (int) m_changedCells.size();
    for( int i = 0; i < n; i++ )
        if( m_changedCells[i].first == x && m_changedCells[i].second == y )
            return true;
    return false;
}

void GameOfLive::swap( int& x, int& y ) {
    int temp = x;
    x = y;
    y = temp;
}

void GameOfLive::bresenhamLow( int x1, int y1, int x2, int y2 ) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    int yi = 1;
    if( dy < 0 ) {
        yi = -1;
        dy = -dy;
    }
    int d = 2 * dy - dx;
    int y = y1;

    for( int x = x1; x < x2; x++ ) {
        m_cells[ getCellIndex( x, y ) ] = State::ALIVE;
        if( d > 0 ) {
            y = y + yi;
            d = d - 2 * dx;
        }
        d = d + 2 * dy;
    }
}

void GameOfLive::bresenhamHigh( int x1, int y1, int x2, int y2 ) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    int xi = 1;
    if( dx < 0 ) {
        xi = -1;
        dx = -dx;
    }
    int d = 2 * dx - dy;
    int x = x1;

    for( int y = y1; y < y2; y++ ) {
        m_cells[ getCellIndex( x, y ) ] = State::ALIVE;
        if( d > 0 ) {
            x = x + xi;
            d = d - 2 * dy;
        }
        d = d + 2 * dx;
    }
}

void GameOfLive::bresenham(int index1, int index2) {
    std::cout << "bresenham" << std::endl;
    std::cout << "index1: " << index1 << std::endl;
    std::cout << "index2: " << index2 << std::endl;
    if( m_lastChangedIndex == -1 )
        return;
    int x1 = ( index1 % WIDTH );
    int x2 = ( index2 % WIDTH );
    int y1 = ( index1 / HEIGHT );
    int y2 = ( index2 / HEIGHT );
    std::cout << "x1: " << x1 << std::endl;
    std::cout << "y1: " << y1 << std::endl;
    std::cout << "x2: " << x2 << std::endl;
    std::cout << "y2: " << y2 << std::endl;

    if( std::abs( y2 - y1 ) < std::abs( x2 - x1 ) ){
        if( x1 > x2 ) {
            bresenhamLow(x2, y2, x1, y1);
            std::cout << "1" << std::endl;
            m_cells[ index2 ] = State::ALIVE;
        }
        else {
            bresenhamLow(x1, y1, x2, y2);
            std::cout << "2" << std::endl;
        }
    }
    else {
        if( y1 > y2 ) {
            bresenhamHigh(x2, y2, x1, y1);
            std::cout << "3" << std::endl;
        }
        else {
            bresenhamHigh(x1, y1, x2, y2);
            std::cout << "4" << std::endl;
        }
    }
}

void GameOfLive::handleDeadEvents( sf::Event event ){
    switch( event.type ) {
        case sf::Event::MouseButtonPressed: {
            if (event.mouseButton.button == sf::Mouse::Button::Left) {
                if( sf::Keyboard::isKeyPressed( sf::Keyboard::LShift ) ) {
                    int x = event.mouseButton.x / CELL_SIZE;
                    int y = event.mouseButton.y / CELL_SIZE;
                    bresenham( m_lastChangedIndex, getCellIndex( x, y ) );
                }
                m_drag = true;
            }
            break;
        }
        case sf::Event::MouseButtonReleased: {
            if( event.mouseButton.button == sf::Mouse::Button::Left ) {
                if( m_notMoved ) {
                    int x = event.mouseButton.x / CELL_SIZE;
                    int y = event.mouseButton.y / CELL_SIZE;
                    int index = getCellIndex( x, y );
                    m_cells[ index ] = ( m_cells[ index ] == State::ALIVE ) ? State::DEAD : State::ALIVE;
                    m_lastChangedIndex = index;
                }
                m_drag = false;
                m_changedCells.clear();
                m_notMoved = true;
            }
            break;
        }
        case sf::Event::MouseMoved: {
            if( m_drag ) {
                m_notMoved = false;
                int x = event.mouseMove.x / CELL_SIZE;
                int y = event.mouseMove.y / CELL_SIZE;
                if (!alreadyChanged(x, y)) {
                    int index = getCellIndex( x, y );
                    m_cells[ index ] = ( m_cells[ index ] == State::ALIVE ) ? State::DEAD : State::ALIVE;
                    m_changedCells.emplace_back(x, y);
                    m_lastChangedIndex = index;
                }
            }
            break;
        }
        case sf::Event::KeyPressed: {
            if( event.key.code == sf::Keyboard::Space ) {
                m_state = State::ALIVE;
                m_window.setFramerateLimit( FRAMERATE_ALIVE );
            }
            if( event.key.code == sf::Keyboard::S ) {
                save();
            }
            if( event.key.code == sf::Keyboard::R ) {
                reset();
            }
            break;
        }
        default: {
            break;
        }
    }
}

void GameOfLive::handleAliveEvents( sf::Event event ) {
    if( event.type == sf::Event::KeyPressed )
        if( event.key.code == sf::Keyboard::Space ) {
            m_state = State::DEAD;
            m_window.setFramerateLimit( FRAMERATE_DEAD );
        }
}

void GameOfLive::handleEvents(sf::Event event) {
    if( m_state == State::DEAD ) {
        handleDeadEvents( event );
    }
    else if( m_state == State::ALIVE ) {
        handleAliveEvents( event );
    }
}

void GameOfLive::conwayUpdate( std::vector<State> newCells ) {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            int index = getCellIndex(x, y);
            int neighbors = countNeighbors(x, y);
            if (m_cells[index] == State::DEAD && neighbors == 3)
                newCells[index] = State::ALIVE;
            else if (m_cells[index] == State::ALIVE && (neighbors < 2 || neighbors > 3))
                newCells[index] = State::DEAD;
            else
                newCells[index] = m_cells[index];
        }
    }
    m_cells = newCells;
}

void GameOfLive::copyUpdate( std::vector<State> newCells ) {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            int index = getCellIndex(x, y);
            int neighbors = countNeighbors(x, y);
            newCells[ index ] = ( neighbors % 2 == 0 ) ? State::DEAD : State::ALIVE;
        }
    }
    m_cells = newCells;
    m_evolution++;
}

void GameOfLive::update() {
    if( m_state == State::ALIVE ) {
        std::vector<State> newCells( (unsigned long ) ( WIDTH * HEIGHT ) );
        switch( m_automata ) {
            case CONWAY:
                conwayUpdate( newCells );
                break;
            case COPY:
                copyUpdate( newCells );
                break;
        }
    }
}

void GameOfLive::render() {
    m_window.clear( sf::Color( 127, 127, 127, 255 ) );

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            setColor( x, y, ( m_cells[ getCellIndex( x, y ) ] ) );
        }
    }
    m_window.draw( m_pixels );

//    if( m_state == State::DEAD && WINDOW_WIDTH > 640 ) {
//        m_window.draw( m_text );
//    }

    m_window.display();

    if( m_evolution % 4 == 0 && m_automata == CellularAutomaton::COPY && m_state == State::ALIVE )
        sf::sleep( sf::milliseconds( 1000 ) );
}

void GameOfLive::save() {
    std::ofstream fs( FILENAME );

    fs << 991 << std::endl;
    fs << WIDTH << " " << HEIGHT << std::endl;
    for( int y = 0; y < HEIGHT; y++ ) {
        for( int x = 0; x < WIDTH; x++ ) {
            fs << ( ( m_cells [ getCellIndex( x, y ) ] == State::DEAD ) ? "0 " : "1 " );
        }
        fs << std::endl;
    }

    std::cout << "Saved" << std::endl;
    fs.close();
}

void GameOfLive::reset() {
    int n = (int) m_cells.size();
    for( int i = 0; i < n; i++ ) {
        m_cells[i] = State::DEAD;
    }
    std::cout << "Reset" << std::endl;
}

void GameOfLive::load() {
    std::ifstream fs( m_filename );

    int header;
    fs >> header;
    if( header != 991 ) {
        std::cout << "The magic number of the filename is wrong." << std::endl;
        fs.close();
        return;
    }
    int width;
    int height;
    fs >> width >> height;
    int smallHeight = ( height < HEIGHT ) ? height : HEIGHT;
    int bigWidth = width;
    int smallWidth = width;
    if( width < WIDTH )
        bigWidth = WIDTH;
    else
        smallWidth = WIDTH;
    for( int y = 0; y < smallHeight; y++ ) {
        for( int x = 0; x < smallWidth; x++ ) {
            int value;
            fs >> value;
            m_cells[ getCellIndex( x, y ) ] = ( value == 0 ) ? State::DEAD : State::ALIVE;
        }
        fs.ignore( 2 * bigWidth, '\n' );
    }

    fs.close();
}

void GameOfLive::setFilename( const char* filename ) {
    m_filename = filename;
}

void GameOfLive::run() {
    while( m_window.isOpen() ) {
        sf::Event event;
        while( m_window.pollEvent( event ) ) {
            if( event.type == sf::Event::Closed )
                m_window.close();
            handleEvents( event );
        }

        render();
        update();
    }
}
