#include "utm.hpp"
#include <iostream>

int main(){
    universal_tm tm;
    tm.readfile("./actions");
    tm.mark_halt("q5");
    auto &&str=string("000001111122222");
    std::cout<<std::boolalpha<<tm.check(str)<<std::endl;
    std::cout<<str<<std::endl;
}
