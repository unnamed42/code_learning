#include "bitmap.hpp"

#include <fstream>
#include <iostream>
#include <stdexcept>

using namespace std;

int main(int argc, char* argv[]) {
    ifstream imageFile(argv[1], ios::binary);
    if(!imageFile.good())
        throw invalid_argument{"cannot open bmp file"};

    ofstream asciiFile(argv[2]);
    if(!asciiFile.good())
        throw invalid_argument{"cannot open ascii art output file"};

    Bitmap img{imageFile};

    img.toGrayscale();
    img.toASCIIArt(asciiFile);

    ofstream outputFile(argv[3], ios::binary);
    if(!outputFile.good())
        throw invalid_argument{"cannot open output file"};

    img.save(outputFile);
}
