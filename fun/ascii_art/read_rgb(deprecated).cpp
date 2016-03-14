#include <string>
#include <fstream>
#include <iostream>

typedef unsigned char byte_t;

struct bmp_file{
    int width;
    int height;
    byte_t *pixels;
    
    explicit bmp_file(int w=0,int h=0,byte_t *p=nullptr):width(w),height(h),pixels(p){}
    ~bmp_file() {delete[] pixels;}
    byte_t& red(int i,int j) {return pixels[j*width+i];}
    byte_t& green(int i,int j) {return pixels[j*width+i+1];}
    byte_t& blue(int i,int j) {return pixels[j*width+i+2];}
};

// read 24bit bmp image, return an array, delete required.
// data should contain the (R, G, B) values of the pixels.
// The color of pixel (i, j) is stored at data[j * width + i], data[j * width + i + 1] and data[j * width + i + 2].
bmp_file* read_bmp(const char *file_loc){
    // read file
    std::fstream file(file_loc,std::ios::in|std::ios::binary);
    if(!file.is_open())
        return nullptr;
    
    // read header from bmp
    byte_t info[54];
    file.read(reinterpret_cast<char*>(info),54);
    
    int width=*reinterpret_cast<int*>(&info[18]),\
        height=*reinterpret_cast<int*>(&info[22]),\
        size=3*width*height;
    auto data=new byte_t[size]; // allocates 3 bytes per pixel
    file.read(reinterpret_cast<char*>(data),size); // extract rest of the data
    file.close();
    
    // change data format from (B,G,R) to (R,G,B)
    for(auto i=0;i<size;i+=3)
        data[i]^=data[i+2]^=data[i]^=data[i+2];
    
    // assemble data
    return new bmp_file(width,height,data);
}

void grayscale(){
    //Luminace = 0.299 * Red + 0.587 * Green + 0.114 * Blue
}

int main(){
    auto ptr=read_bmp("/home/h/parrots.bmp");
    if(ptr==nullptr)
        return -1;
    const int &width=ptr->width,&height=ptr->height;
    std::fstream out("/home/h/pixels",std::ios::out);
    for(int i=0;i<width;++i){
        for(int j=0;j<height;++j)
            out<<"R: "<<int(ptr->red(i,j))<<std::endl\
               <<"G: "<<int(ptr->green(i,j))<<std::endl\
               <<"B: "<<int(ptr->blue(i,j))<<std::endl;
    }
    out.close();
    std::cout<<"Over";
    return 0;
}
