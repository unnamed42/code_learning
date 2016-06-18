#ifndef __SNAKE_GAME__
#define __SNAKE_GAME__

#include "map.hpp"
#include <deque>

enum direction:unsigned int {UP=0,DOWN,LEFT,RIGHT};

class snake_game{
    public:
        struct snake{
            std::deque<coordinate> m_body;
            direction m_direct;
            
            snake(const coordinate&);
        };
        
    private:
        map m_map;
        snake m_snake;
        coordinate m_food;
        bool m_end;
    public:
        snake_game(unsigned int,unsigned int);    
    protected:
        // Randomly generate a food
        void generate_food();
        
        // Let the snake go forward, then refresh the map
        void update();
    public:
        // Check if game ends
        bool is_end() const;
        
        // Run the game
        void run();
        
        // Restart this game
        void restart();
};

#endif // __SNAKE_GAME__
