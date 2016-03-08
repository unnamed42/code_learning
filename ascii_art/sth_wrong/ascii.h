#include "bmp.h"
#include <memory> // std::shared_ptr
#include <cstdio> // for file output and deletion
#include <cstring> // std::strlen
#include <stdexcept>

// The string `map` used in this source file is designed to be displayed on white backgrounds.
// If you want to make a black background ascii image, change `map` to " .,:;ox%#@".

// Keep in mind that all these 3 functions would throw exceptions.

void color_bmp_to_grayscale(const char *input,const char *output){
    bmp image(input);
    auto w=image.width(),h=image.height();
    auto out=bmp::RGB_to_gray(image.data().get(),w,h);
    bmp::save(output,image.width(),image.height(),8,out.get()); // set 8 for grayscale image
}

void grayscale_to_ascii(const char *input,const char *output,const char *map="@#%xo;:., "){
    bmp image(input);
    std::FILE *file=std::fopen(output,"w");
    if(file==NULL)
        throw std::runtime_error("Failed to open file");
    auto *ptr=image.data().get();
    int line_width = image.width() * image.bit_count() / 8;
    for(auto y=0;y<image.height();++y){
        for(auto x=0;x<image.width();++x){
            int tmp=ptr[y*line_width+x];
            tmp+=ptr[y*line_width+x+1];
            tmp+=ptr[y*line_width+x+2];
            tmp=(tmp*std::strlen(map))/768;
            std::fprintf(file,"%c",map[tmp]);
        }
        std::fputc('\n',file);
    }
    std::fclose(file);
}

void color_bmp_to_ascii(const char *input,const char *output,const char *map="@#%xo;:., ",const char *tmp_file="../gray.tmp"){
    color_bmp_to_grayscale(input,tmp_file);
    grayscale_to_ascii(tmp_file,output,map);
    std::remove(tmp_file);
}

