#include "snake_game.hpp"
#include <cstdlib>
#include <ctime>
#include <unistd.h>

#include <stdio.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <stropts.h>

extern "C" {static int kbhit();}

// Check if keyboard is pressed, implemented by UNIX APIs
static int kbhit() {
    static const int STDIN = 0;
    static bool initialized = false;

    if (! initialized) {
        // Use termios to turn off line buffering
        termios term;
        tcgetattr(STDIN, &term);
        term.c_lflag &= ~ICANON;
        tcsetattr(STDIN, TCSANOW, &term);
        setbuf(stdin, NULL);
        initialized = true;
    }

    int bytesWaiting;
    ioctl(STDIN, FIONREAD, &bytesWaiting);
    return bytesWaiting;
}

// Take a move
static coordinate forward(const coordinate &crd,direction direct){
    coordinate res=crd;
    switch(direct){
        case LEFT:res.y-=1;break;
        case RIGHT:res.y+=1;break;
        case UP:res.x-=1;break;
        case DOWN:res.x+=1;break;
    }
    return res;
}

// Empty, wall, snake, food
enum _material:unsigned char {EMPTY=0,WALL,SNAKE,FOOD};
const std::vector<char> snake_game::material{' ','*','#','@'};

snake_game::snake::snake(const coordinate &crd):m_body(1,crd),m_direct(LEFT) {}

snake_game::snake_game(unsigned int row,unsigned int column):m_map(row,column),m_snake({row/2,column/2}),m_food({0,0}),m_end(false) {
    // build walls
    for(auto i=0U;i<row;++i){
        m_map[i][0]=WALL;
        m_map[0][i]=WALL;
        m_map[i][column-1]=WALL;
        m_map[column-1][i]=WALL;
    }
    m_map.set(m_snake.m_body.front(),SNAKE);
    std::srand(std::time(nullptr));
    generate_food();
}

bool snake_game::is_end() const {return m_end;}

void snake_game::generate_food(){
    do{
        m_food=coordinate(rand()%m_map.row(),rand()%m_map.column());
    } while(m_map.value(m_food));
    m_map.set(m_food,FOOD);
}

bool snake_game::invalid_move(const coordinate &crd,direction direct){
    switch(direct){
        case LEFT:return crd.y==1;
        case RIGHT:return crd.y==m_map.row()-2;
        case UP:return crd.x==1;
        case DOWN:return crd.x==m_map.column()-2;
    }
}

void snake_game::update(){
    // Snake fills the map, you win
    if(m_snake.m_body.size()==m_map.row()*m_map.column()){
        m_end=true;
        return;
    }
    // Get out of map or eat yourself, you lose
    auto &&head=m_snake.m_body.front();
    if(invalid_move(head,m_snake.m_direct)){
        m_end=true;
        return;
    }
    
    auto &&new_head=forward(head,m_snake.m_direct);
    if(new_head==m_food){ // eat food
        m_snake.m_body.push_front(m_food);
        m_map.set(m_food,SNAKE);
        generate_food();
    }
    else if(m_map.value(new_head)){ // eat yourself, you lose
        m_end=true;
        return;
    } else { // just move forward
        m_map.set(new_head,SNAKE);
        m_snake.m_body.push_front(new_head);
        
        auto &&tail=m_snake.m_body.back();
        m_map.set(tail,EMPTY);
        m_snake.m_body.pop_back();
    }
}

void snake_game::restart(){
    auto row=m_map.row(),column=m_map.column();
    m_map.clear();
    m_snake.m_body.clear();
    m_snake.m_direct=LEFT;
    m_map=map(row,column);
}

void snake_game::run(){
    while(!is_end()){
        std::system("clear");
        m_map.draw(material);
        if(kbhit()){
            unsigned int tmp=0;
            switch(getchar()){
                case 'w':tmp=UP;break;
                case 'a':tmp=LEFT;break;
                case 's':tmp=DOWN;break;
                case 'd':tmp=RIGHT;
                default:break;
            }
            if(m_snake.m_body.size()>1 && tmp==3-m_snake.m_direct)
                ; // You cannot go backwards when bigger-than-1-block long. In this case, the input is ignored.
            else
                m_snake.m_direct=static_cast<direction>(tmp);
        }
        update();
        sleep(1);
    }
}
