#include "bmp.h"
#include <memory> // std::shared_ptr
#include <cstdio> // for file output and deletion
#include <cstring> // std::strlen
#include <stdexcept>

// The string `map` used in this source file is designed to be displayed on white backgrounds.
// If you want to make a black background ascii image, change `map` to " .,:;ox%#@".

void color_bmp_to_grayscale(const char *input,const char *output){
    try{
        bmp image(input);
        auto w=image.get_width(),h=image.get_height();
        auto out =std::shared_ptr<unsigned char>(new unsigned char[w*h],std::default_delete<unsigned char[]>());
        bmp::RGB2gray(image.get_data().get(),w,h,out.get());
        image.save(output,w,h,1,out.get());
    } catch(std::exception &e) {
        throw e;
    }
}

void grayscale_to_ascii(const char *input,const char *output,const char *map="@#%xo;:., "){
    try{
        bmp image(input);
        std::FILE *file=std::fopen(output,"w");
        if(file==NULL)
            throw std::runtime_error("Failed to open file");
        typename bmp::byte_t *ptr=image.get_data().get();;
        int line_width = image.get_width() * image.get_bit_count() / 8;
        for(auto y=0;y<image.get_height();++y){
            for(auto x=0;x<image.get_width();++x){
                int tmp=ptr[y*line_width+x];
                tmp+=ptr[y*line_width+x+1];
                tmp+=ptr[y*line_width+x+2];
                tmp=(tmp*std::strlen(map))/768;
                std::fprintf(file,"%c",map[tmp]);
            }
            std::fputc('\n',file);
        }
        std::fclose(file);
    } catch(std::exception &e) {
        throw e;
    }
}

void color_bmp_to_ascii(const char *input,const char *output,const char *tmp_file="../gray.tmp",const char *map="@#%xo;:., "){
    try{
        color_bmp_to_grayscale(input,tmp_file);
        grayscale_to_ascii(tmp_file,output,map);
        std::remove(tmp_file);
    } catch(std::exception &e) {
        throw e;
    }
}
