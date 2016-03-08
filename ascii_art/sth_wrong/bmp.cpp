#include "bmp.h"
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>

typedef typename bmp::file_header file_header;
typedef typename bmp::info_header info_header;

template <class T> static inline constexpr std::shared_ptr<T> create_array(T *arr) {return std::shared_ptr<T>(arr,std::default_delete<T[]>());}

bmp::bmp():_width(0),_height(0),_bit_count(0),_data_size(0),_data(create_array<uint8_t>(nullptr))/*,_data_RGB(create_array<uint8_t>(nullptr))*/{
    /*_data=std::shared_ptr<uint8_t>(nullptr,std::default_delete<uint8_t[]>());
    _data_RGB=std::shared_ptr<uint8_t>(nullptr,std::default_delete<uint8_t[]>());*/
}

bmp::bmp(const bmp &other):_width(other._width),_height(other._height),_bit_count(other._bit_count),_data_size(other._data_size),_data(other._data)/*,_data_RGB(other._data_RGB)*/{}

bmp::bmp(const char *filename){read(filename);}

void bmp::clear(){
    _width=_height=_bit_count=_data_size=0;
    _data.reset();
    //_data_RGB.reset();
}

void bmp::show_info() const{
    std::cout << "===== bmp =====" << std::endl
           << "Width: " << _width << " pixels" << std::endl
           << "Height: " << _height << " pixels" << std::endl
           << "Bit Count: " << _bit_count << " bits" << std::endl
           << "Data Size: " << _data_size  << " bytes" << std::endl;
}

void bmp::read(const char *filename){
    std::fstream file(filename,std::ios::in|std::ios::binary);
    if(!file.is_open()||file.bad())
        throw std::runtime_error("bmp::read: cannot open or read file");
    file_header f;
    file.read(reinterpret_cast<char*>(&f),sizeof(file_header));
    if(f.signature[0]!='B'||f.signature[1]!='M'){
        file.close();
        throw std::runtime_error("bmp::read: Wrong image file type");
    }
    info_header i;
    file.read(reinterpret_cast<char*>(&i),sizeof(info_header));
    
    if(i.bit_count<8){
        file.close();
        throw std::runtime_error("Unsupported format.");
    }
    if(i.compression > 1){
        file.close();
        throw std::runtime_error("Unsupported compression mode.");
    }
    file.seekg(0,std::ios::end);
    f.file_size=file.tellg();
    
    uint32_t paddings = (4 - ((i.width * i.bit_count / 8) % 4)) % 4,\
             data_size = i.width * i.height * i.bit_count / 8;
    
    uint32_t image_size=f.file_size - f.off_bits;
    
    // write vars
    _width=i.width;
    _height=i.height;
    _bit_count=i.bit_count;
    _data_size=data_size;
    //_data_RGB=create_array(new uint8_t[image_size]);
    
    // see if decoding is required
    if(i.compression==0){
        file.seekg(f.off_bits,std::ios::beg);
        _data=create_array(new uint8_t[image_size]);
        file.read(reinterpret_cast<char*>(_data.get()),image_size);
    } else if(i.compression==1) {
        auto size=f.file_size-f.off_bits;
        auto encoded=create_array(new uint8_t[size]);
        
        file.seekg(f.off_bits,std::ios::beg);
        file.read(reinterpret_cast<char*>(encoded.get()),size);
        
        _data=decode_RLE8(encoded.get(),size);
    }
    file.close();
    
    // remove paddings
    if(i.compression==0&&paddings>0){
        int32_t line_width=i.width*i.bit_count/8;
        for(int x=1;x<i.height;++x)
            std::memcpy(&_data.get()[x*line_width],&_data.get()[x*(line_width+paddings)],line_width);
    }
    
    if(i.height>0)
        rotate(_data.get(),i.width,i.height,i.bit_count);
}

void bmp::save(const char *filename,int32_t w,int32_t h, uint16_t bit_count,const uint8_t *data){
    if(w == 0 || h == 0)
        throw std::runtime_error("Zero width or height");

    constexpr uint32_t RES=2835; // 72 pixels/inch = 2835 pixels/m
    
    constexpr uint32_t MAX_COLOR=256; // max color number
    
    const auto channel_count=bit_count/8;
    
    file_header f{{'B','M'},0,0,0,54};
    info_header i{40,w,h,1,bit_count,0,0,RES,RES,0,0};
    
    int palette_size=0;        // size of palette block in bytes

    // compute paddings per each line
    // In BMP, each scanline must be divisible evenly by 4
    // If not, add extra paddings in each line, it can be divisible by 4.
    auto paddings = (4 - ((w * channel_count) % 4)) % 4;

    // compute data size without paddings
    auto data_size = w * h * channel_count;
    
    auto image_size = data_size + (h * paddings);
    
    f.file_size = image_size + f.off_bits;

    // 8-bit grayscale image need palette
    // correct color_count, data_offset and file_size
    if(channel_count == 1){
        i.color_used = MAX_COLOR;                   // always use max number of colors for 8-bit gray scale
        palette_size = MAX_COLOR * 4;       // BGRA for each
        f.off_bits = 54 + palette_size;      // add up palette size
        f.file_size = image_size + f.off_bits;   // reset file size
    }

    // allocate output data array
    auto tmp_data = create_array(new uint8_t[data_size]);

    // copy image data
    std::memcpy(tmp_data.get(), data, data_size);

    // flip the image upside down
    // If height is  negative, then it is bottom-top orientation (no need to flip)
    if(h > 0)
        rotate(tmp_data.get(), w, h, bit_count);

    // convert RGB to BGR order
    //if(channel_count == 3 || channel_count == 4)
    //    swap_RB(tmp_data.get(), data_size, channel_count);

    // add paddings(0s) if the width of image is not divisible by 4
    auto data_with_paddings=create_array<uint8_t>(nullptr);
    if(paddings > 0){
        // allocate an array
        // add extra bytes for paddings in case the width is not divisible by 4
        data_with_paddings = create_array(new uint8_t[image_size]);

        int line_width = w * channel_count;       // line _width in bytes

        // copy single line at a time
        for(int i = 0; i < h; ++i){
            // restore data by adding paddings
            std::memcpy(&data_with_paddings.get()[i*(line_width+paddings)], &tmp_data.get()[i*line_width], line_width);

            // insert 0 for paddings after copying the current line
            for(int j = 1; j <= paddings; ++j)
                data_with_paddings.get()[(i+1)*(line_width+paddings) - j] = uint8_t(0);
        }
    }

    // open output file to write data
    std::fstream file(filename,std::ios::out|std::ios::binary);
    if(!file.is_open()||!file.good())
        throw std::runtime_error("Failed to open an output file.");

    // write header
    file.write(reinterpret_cast<char*>(&f),sizeof(file_header));
    file.write(reinterpret_cast<char*>(&i),sizeof(info_header));

    // For 8-bit grayscale, insert palette between header block and data block
    if(bit_count == 8){
        auto palette=create_array(new uint8_t[palette_size]);// each entry has 4 bytes(B,G,R,A)
        build_grayscale_palette(palette.get(), palette_size);

        // write palette to the file
        file.write(reinterpret_cast<char*>(palette.get()), palette_size);
    }

    // write image data
    if(paddings == 0)
        file.write(reinterpret_cast<char*>(tmp_data.get()), data_size);                        // without padding
    else
        file.write(reinterpret_cast<char*>(data_with_paddings.get()), image_size);   // with paddings

    // close the opened file
    file.close();
}

void bmp::save(const char *filename) {save(filename,_width,_height,_bit_count,_data.get());}

std::shared_ptr<uint8_t> bmp::decode_RLE8(uint8_t *input,uint32_t size){
    if(input==nullptr)
        return create_array<uint8_t>(nullptr);
    
    auto data=create_array(new uint8_t[size]);auto output=data.get();
    
    uint8_t first, second;

    // start decoding, stop when it reaches at the end of decoded data
    for(;;){
        // grab 2 bytes at the current position
        first = *(input++);
        second = *(input++);

        if(first!=0){  // input run mode
            for(int i=0; i < first; ++i)
                *(output++) = second;
        } else {
            if(second == 1)         // reached the end of bitmap
                break;              // must stop decoding

            else if(second == 2)    // delta mark
                input += 2;       // do nothing, but move the cursor 2 more bytes

            else {                  // uninput run mode (second >= 3)
                for(int i=0; i < second; ++i)
                    *(output++) = *(input++);

                if(second%2!=0)      // if it is odd number, then there is a padding 0. ignore it
                    input++;
            }
        }
    }
    return data;
}

void bmp::rotate(uint8_t* data, int32_t width, int32_t height, uint32_t bit_count){
    if(data==nullptr) 
        return;
    int32_t line_size = width * bit_count/8;
    auto tmp = create_array(new uint8_t[line_size]);
    int32_t half = height / 2;

    int32_t line1 = 0;                          // first line
    int32_t line2 = (height - 1) * line_size;    // last line

    // scan only half of height
    for(int i = 0; i < half; ++i){
        // copy line by line
        std::memcpy(tmp.get(), &data[line1], line_size);
        std::memcpy(&data[line1], &data[line2], line_size);
        std::memcpy(&data[line2], tmp.get(), line_size);

        // move to next line
        line1 += line_size;
        line2 -= line_size;
    }
}

void bmp::swap_RB(uint8_t *data,uint32_t data_size,uint16_t bit_count){
    auto channel_count=bit_count/8;
    if(data==nullptr || channel_count < 3 || data_size%channel_count!=0)  // channel_count must be 3 or 4, data_size must be divisible by the number of channels
        return;

    // swap the position of red and blue components
    for(auto i=0U; i < data_size; i+=channel_count)
        data[i]^=data[i+2]^=data[i]^=data[i+2];
}

uint32_t bmp::get_color_count(const uint8_t *data,uint32_t data_size){
    if(data==nullptr) 
        return 0;
    constexpr int MAX_COLOR = 256;  // max number of colors in 8-bit grayscale
    uint32_t color_count = 0;
    unsigned int colors[MAX_COLOR]={};

    // increment at the same index
    for(auto i = 0U; i < data_size; ++i)
        colors[data[i]]++;

    // count backward the number of color used in this data
    color_count = MAX_COLOR;
    for(int i = 0; i < MAX_COLOR; ++i){
        if(colors[i] == 0)
            color_count--;
    }

    return color_count;
}

void bmp::build_grayscale_palette(uint8_t *palette,uint32_t palette_size){
    if(palette==nullptr)
        return;

    // fill B, G, R, with same value and A is 0
    for(auto i = 0U, j = 0U; i < palette_size; i+=4, ++j){
        palette[i] = palette[i+1] = palette[i+2] = uint8_t(j);
        palette[i+3] = uint8_t(0);
    }
}

std::shared_ptr<uint8_t> bmp::BGR_to_gray(const uint8_t *input,int32_t width,int32_t height){
    if(input==nullptr)
        return create_array<uint8_t>(nullptr);

    auto size = width * height;  // number of pixels

    auto output=create_array(new uint8_t[size]);
    
    /* slow method
    for(int i = 0, j = 0; i < size; i++, j+=3)
        out[i] = (uint8_t)(0.114f * input[j] + 0.587f * input[j+1] + 0.299f * input[j+2] + 0.5f);
    */
    
    // fast estimation ********************************
    auto *out = output.get();

    for(int i=0;i<size;i++){
        auto tmp  = *(input++);                       // add blue (1 * blue)
        tmp += (*input << 2) + *input;          // add green (5 * green)
        input++;
        tmp += *(input++) << 1;                  // add red (2 * red)
        *out++ = uint8_t(tmp >> 3); // divide by 8
    }
    
    return output;
}

std::shared_ptr<uint8_t> bmp::RGB_to_gray(const uint8_t *input,int32_t width,int32_t height){
    // check NULL pointer
    if(input==nullptr)
        return create_array<uint8_t>(nullptr);

    int size = width * height;  // number of pixels

    /* slow methode
    for(int i = 0, j = 0; i < size; i++, j+=3)
        out[i] = uint8_t(0.229f * input[j] + 0.587f * input[j+1] + 0.114f * input[j+2] + 0.5f);
    */
    
    auto output=create_array(new uint8_t[size]);
    
    // fast estimation ********************************
    // backup pointers
    // We are going to use parameters (input, out) because they are faster.
    uint8_t *out = output.get();

    for(int i=0;i<size;++i){
        auto tmp  = *(input++) << 1;                  // add red (2 * red)
        tmp += ((*input) << 2) + *input;          // add green (5 * green)
        input++;
        tmp += *(input++);                       // add blue (1 * blue)
        *(out++) = uint8_t(tmp >> 3); // divide by 8
    }

    return output;
}

int32_t bmp::width() const noexcept {return _width;}

int32_t bmp::height() const noexcept {return _height;}

uint16_t bmp::bit_count() const noexcept {return _bit_count;}

uint32_t bmp::data_size() const noexcept {return _data_size;}

std::shared_ptr<uint8_t> bmp::data() const noexcept {return _data;}
