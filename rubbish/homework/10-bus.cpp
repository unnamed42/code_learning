#include <queue>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <iostream>

#define TIME_LIMIT 600

typedef enum{A=0,B,C,D} bus_id;
typedef unsigned int uint_t;

struct bus{
    uint_t start,interval;
    const bus_id id;
    std::queue<uint_t> psgr;//abbr. for passenger
    bus():start(0),interval(0),id(A),psgr(){}
    bus(uint_t _start,uint_t _inter,bus_id _id):start(_start),interval(_inter),id(_id),psgr(){}
    void get_on(uint_t t){psgr.push(t);}
    uint_t calculate_time(){
        if(psgr.size()==0)
            return 0;
        uint_t wait=0,_inter=start,curr;
        while(!psgr.empty()){
            curr=psgr.front();
            if(_inter>TIME_LIMIT)
                break;
            if(curr<=_inter)
                wait+=_inter-curr;
            else
                _inter+=interval;
            psgr.pop();
        }
        return wait;
    }
};

/*
 * 4 kinds of bus in total.
 * Bus A: starting at 0 min, interval 20min.
 * Bus B: starting at 10min, interval 30min.
 * Bus C: starting at 20min, interval 10min.
 * Bus D: starting at 30min, interval 50min.
 * Assuming that each kind of bus only one arrives at a time.
 * All buses can take all the waiting passengers.
 * Time ranges from 0min to 600min.
 */

void get_on_bus(std::vector<bus> &v){
    uint_t t=0;
    while(t<=TIME_LIMIT){
        v[rand()%4].get_on(t);    //rand()%4 generates the wanted bus id
        t+=rand()%3;
    }
}

int main(){
    std::vector<bus> Bus;
    Bus.push_back(bus(0,20,A));
    Bus.push_back(bus(10,30,B));
    Bus.push_back(bus(20,10,C));
    Bus.push_back(bus(30,50,D));
    srand(time(NULL));
    get_on_bus(Bus);
    for(int i=0;i<4;i++)
        std::cout<<"Bus "<<static_cast<char>('A'+i)<<": "<<Bus[i].calculate_time()<<"min"<<std::endl;
    return 0;
}