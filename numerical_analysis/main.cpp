#include "linear_equation.hpp"

#include <iostream>

using namespace std;

int main() {
    LinearEquation eq {
        {1,1,1,3},
        {2,4,5,0},
        {5,7,9,8},
    };

    cout << eq.solve();
}
