#include "ascii.h"
#include <iostream>

int main(){
    try{
        color_bmp_to_ascii("/home/h/1.bmp","/home/h/gray.txt");
    } catch(std::runtime_error &e){
        std::cout<<e.what();
    }
    std::cout<<"Over";
    return 0;
}
