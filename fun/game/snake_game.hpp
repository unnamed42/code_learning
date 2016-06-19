#ifndef __SNAKE_GAME__
#define __SNAKE_GAME__

#include "map.hpp"
#include <deque>
#include <vector>

// Let `x` be a direction variable, then (3-x) is the opposite direction
enum direction:unsigned int {UP=0,LEFT,RIGHT,DOWN};

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
        
        const static std::vector<char> material;
    public:
        snake_game(unsigned int,unsigned int);
    protected:
        // Randomly generate a food
        void generate_food();
        
        // Check if one move causes an overflow
        bool invalid_move(const coordinate&,direction);
        
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
