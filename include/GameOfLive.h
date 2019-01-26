#include <iostream>
#include <SFML/Graphics.hpp>

class GameOfLive {
    private:
        const char* WINDOW_TITLE = "SFML";
        static const int NUM_CELLS = 5;
        static const int CELL_SIZE = 40;
        static const int WINDOW_WIDTH = NUM_CELLS * CELL_SIZE;
        static const int FRAMERATE = 1;

        static const bool DEAD = false;
        static const bool ALIVE = true;

        sf::RenderWindow m_window;
        sf::Image m_image;
        sf::Texture m_texture;
        sf::Sprite m_sprite;

        bool m_pixels[ NUM_CELLS * NUM_CELLS ];

        int countNeighbors( int cell );

        void init();
        void update();
        void render();

    public:
        GameOfLive();

        void run();
};

