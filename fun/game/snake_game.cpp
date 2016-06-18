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

// Check if one move causes an overflow
static bool invalid_move(const coordinate &crd,direction direct,unsigned int max_x,unsigned int max_y){
    bool res=false;
    switch(direct){
        case LEFT:res=(crd.y==0);break;
        case RIGHT:res=(crd.y==max_y);break;
        case UP:res=(crd.x==0);break;
        case DOWN:res=(crd.x==max_x);break;
    }
    return res;
}

// Move a point
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

snake_game::snake::snake(const coordinate &crd):m_body(1,crd),m_direct(LEFT) {}

snake_game::snake_game(unsigned int row,unsigned int column):m_map(row,column),m_snake({row/2,column/2}),m_food({0,0}),m_end(false) {
    m_map.set(m_snake.m_body.front());
    std::srand(std::time(nullptr));
    generate_food();
}

bool snake_game::is_end() const {return m_end;}

void snake_game::generate_food(){
    do{
        m_food=coordinate(rand()%m_map.row(),rand()%m_map.column());
    } while(m_map.is_set(m_food));
    m_map.set(m_food);
}

void snake_game::update(){
    // Snake fills the map, you win
    if(m_snake.m_body.size()==m_map.row()*m_map.column()){
        m_end=true;
        return;
    }
    // Get out of map or eat yourself, you lose
    auto &&head=m_snake.m_body.front();
    if(invalid_move(head,m_snake.m_direct,m_map.column()-1,m_map.row()-1)){
        m_end=true;
        return;
    }
    
    auto &&new_head=forward(head,m_snake.m_direct);
    if(new_head==m_food){ // eat food
        m_snake.m_body.push_front(m_food);
        generate_food();
    }
    else if(m_map.is_set(new_head)){ // eat yourself, you lose
        m_end=true;
        return;
    } else { // just move forward
        m_map.set(new_head);
        m_snake.m_body.push_front(new_head);
        
        auto &&tail=m_snake.m_body.back();
        m_map.unset(tail);
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
        m_map.draw(' ','#');
        if(kbhit()){
            switch(getchar()){
                case 'w':m_snake.m_direct=UP;break;
                case 'a':m_snake.m_direct=LEFT;break;
                case 's':m_snake.m_direct=DOWN;break;
                case 'd':m_snake.m_direct=RIGHT;
                default:break;
            }
        }
        update();
        sleep(1);
    }
}
