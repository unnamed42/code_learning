#include "bmp.hpp"

#include <iostream>
#include <exception>

int main() try {
    image::bmp img("/home/h/toad.bmp");
    img.to_grayscale();
    img.to_ascii_art("/home/h/toad.txt");
    img.save("/home/h/toad_gray.bmp");
} catch(std::exception &e) {
    std::cout << e.what() << '\n';
}
