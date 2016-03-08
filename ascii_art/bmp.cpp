#include "bmp.h"
#include <cstring> // for memory manipulation functions
#include <fstream> // std::fstream
#include <cstdint> // for fixed width integral types
#include <stdexcept> // std::exception

bmp::bmp():width(0),height(0),bit_count(0),data_size(0){
    data=std::shared_ptr<byte_t>(nullptr,std::default_delete<byte_t[]>());
    data_RGB=std::shared_ptr<byte_t>(nullptr,std::default_delete<byte_t[]>());
}

bmp::bmp(const bmp &other):width(other.width),height(other.height),bit_count(other.bit_count),data_size(other.data_size),data(other.data),data_RGB(other.data_RGB){}

bmp::bmp(const char *filename){read(filename);}

void bmp::clear(){
    width=height=bit_count=data_size=0;
    data.reset();
    data_RGB.reset();
}

void bmp::read(const char *filename){
    this->clear();
    std::fstream file(filename,std::ios::in|std::ios::binary);
    if(!file.is_open()||!file.good())
        throw std::runtime_error("Opening file failed.");
    
    // list of entries in BMP header
    byte_t id[2];             // magic identifier "BM" (2 bytes)
    int32_t file_size;           // file size in bytes (4)
    int16_t reserved1;        // reserved 1 (2)
    int16_t reserved2;        // reserved 2 (2)
    int32_t data_offset;         // starting offset of bitmap data (4)
    int32_t info_header_size;     // info header size (4)
    int32_t width;              // image width (4)
    int32_t height;             // image height (4)
    int16_t plane_count;       // # of planes (2)
    int16_t bit_count;         // # of bits per pixel (2)
    int32_t compression;        // compression mode (4)
    int32_t data_size_with_paddings; // bitmap data size with paddings in bytes (4)
    //int32_t x_resolution;        // horizontal pixels per metre (4)
    //int32_t y_resolution;        // vertical pixels per metre (4)
    //int32_t color_count;         // # of colours used (4)
    //int32_t important_color_count;// # of important colours (4)
    
    // read BMP header infos
    file.read(reinterpret_cast<char*>(id), 2);                  // should be "BM"
    file.read(reinterpret_cast<char*>(&file_size), 4);           // should be same as file size
    file.read(reinterpret_cast<char*>(&reserved1), 2);          // should be 0
    file.read(reinterpret_cast<char*>(&reserved2), 2);          // should be 0
    file.read(reinterpret_cast<char*>(&data_offset), 4);
    file.read(reinterpret_cast<char*>(&info_header_size), 4);     // should be 40
    file.read(reinterpret_cast<char*>(&width), 4);
    file.read(reinterpret_cast<char*>(&height), 4);
    file.read(reinterpret_cast<char*>(&plane_count), 2);         // should be 1
    file.read(reinterpret_cast<char*>(&bit_count), 2);           // 1, 4, 8, 24, or 32
    file.read(reinterpret_cast<char*>(&compression), 4);        // 0(uncompressed), 1(8-bit RLE), 2(4-bit RLE), 3(RGB with mask)
    file.read(reinterpret_cast<char*>(&data_size_with_paddings), 4);
    //file.read(reinterpret_cast<char*>(&x_resolution), 4);
    //file.read(reinterpret_cast<char*>(&y_resolution), 4);
    //file.read(reinterpret_cast<char*>(&color_count), 4);
    //file.read(reinterpret_cast<char*>(&important_color_count), 4);
    
    // check magic ID, "BM"
    if(id[0] != 'B' && id[1] != 'M'){
        // it is not BMP file, close the opened file and exit
        file.close();
        throw std::runtime_error("Magic ID is invalid.");
    }

    // it supports only 8-bit grayscale, 24-bit BGR or 32-bit BGRA
    if(bit_count < 8){
        file.close();
        throw std::runtime_error("Unsupported format.");
    }

    // it supports only uncompressed and 8-bit RLE compressed format
    if(compression > 1){
        file.close();
        throw std::runtime_error("Unsupported compression mode.");
    }
    
    // do not trust the file size in header, recalculate it
    file.seekg(0,std::ios::end);
    file_size = file.tellg();
    
    // compute the number of paddings
    // In BMP, each scanline must be divisible evenly by 4.
    // If not divisible by 4, then each line adds
    // extra paddings. So it can be divided evenly by 4.
    int paddings = (4 - ((width * bit_count / 8) % 4)) % 4;

    // compute data size without paddings
    int data_size = width * height * bit_count / 8;

    // recompute data size with paddings (do not trust the data size in header)
    data_size_with_paddings = file_size - data_offset;   // it maybe greater than "data_size+(height*paddings)" because 4-byte boundary for file size

    // now it is ready to store info and image data
    this->width = width;
    this->height = height;
    this->bit_count = bit_count;
    this->data_size = data_size;

    // allocate data arrays
    // add extra bytes for paddings if width is not divisible by 4
    this->data=std::shared_ptr<byte_t>(new byte_t[data_size_with_paddings],std::default_delete<byte_t[]>());
    this->data_RGB=std::shared_ptr<byte_t>(new byte_t[data_size],std::default_delete<byte_t[]>());

/*@@ we don't use palette for 8-bit indexed grayscale mode. Instead, we use the index value as the intensity of the pixel.
    // for loading palette
    byte_t* palette = 0; // for palette for indexed mode
    int palette_size = 0;

    // if bit count is 8 (256 grayscale), then it uses palette (indexed mode)
    // build palette lookup table = (4 * color_count) bytes
    if(bit_count == 8)
    {
        // count palette size
        // palette is placed between BMP header and data
        palette_size = data_offset - 54;              // BMP header size is 54 bytes total

        // allocate palette array
        palette = new byte_t[palette_size];

        // get number of colors used
        int color_count = palette_size / 4;       // each palette has 4 entries(B,G,R,A)

        // copy palette data
        file.seekg(54, ios::beg);         // palette starts right after BMP header block (54 bytes)
        file.read(reinterpret_cast<char*>(palette), palette_size);
    }
*/

    if(compression == 0){       // uncompressed
        file.seekg(data_offset,std::ios::beg); // move cursor to the starting position of data
        file.read(reinterpret_cast<char*>(data.get()), data_size_with_paddings);
    } else if(compression == 1) {  // 8-bit RLE(Run Length Encode) compressed
        // get length of encoded data
        int size = file_size - data_offset;

        // allocate tmp array to store the encoded data
        auto encoded_data =std::shared_ptr<byte_t>(new byte_t[size],std::default_delete<byte_t[]>());

        // read data from file
        file.seekg(data_offset,std::ios::beg);
        file.read(reinterpret_cast<char*>(encoded_data.get()), size);

        // decode RLE into image data buffer
        decode_RLE8(encoded_data.get(), data.get());
    }

    // close it after reading
    file.close();

    // we don't need paddings, trim paddings from each line
    // Note that there is no padding in RLE compressed data
    if(compression == 0 && paddings > 0){
        int line_width = width * bit_count / 8;

        // copy line by line
        for(int i = 1; i < height; ++i)
            std::memcpy(&data.get()[i*line_width], &data.get()[i*(line_width+paddings)], line_width);
    }

    // BMP is bottom-to-top orientation by default, flip image vertically
    // But if the height is negative value, then it is top-to-bottom orientation.
    if(height > 0)
        flip_image(data.get(), width, height, bit_count/8);

    // the colour components order of BMP image is BGR
    // convert image data to RGB order for convenience
    std::memcpy(data_RGB.get(),data.get(),data_size);    // copy data to dataRGB first
    if(bit_count == 24 || bit_count == 32)
        swap_RB(data_RGB.get(), data_size, bit_count/8);
}

void bmp::save(const char *filename,int w,int h, int channel_count,const byte_t *data){
    if(w == 0 || h == 0)
        throw std::runtime_error("Zero width or height");

    // list of entries in BMP header
    byte_t id[2];             // magic identifier "BM" (2 bytes)
    int32_t file_size;           // file size in bytes (4)
    int16_t reserved1;        // reserved 1 (2)
    int16_t reserved2;        // reserved 2 (2)
    int32_t data_offset;         // starting offset of bitmap data (4)
    int32_t info_header_size;     // info header size (4)
    int32_t width;              // image width (4)
    int32_t height;             // image height (4)
    int16_t plane_count;       // # of planes (2)
    int16_t bit_count;         // # of bits per pixel (2)
    int32_t compression;        // compression mode (4)
    int32_t data_size_with_paddings; // bitmap data size in bytes with padding (4)
    int32_t x_resolution;        // horizontal pixels per metre (4)
    int32_t y_resolution;        // vertical pixels per metre (4)
    int32_t color_count;         // # of colours used (4)
    int32_t important_color_count;// # of important colours (4)
    
    int palette_size;        // size of palette block in bytes

    // compute paddings per each line
    // In BMP, each scanline must be divisible evenly by 4
    // If not, add extra paddings in each line, it can be divisible by 4.
    int paddings = (4 - ((w * channel_count) % 4)) % 4;

    // compute data size without paddings
    int data_size = w * h * channel_count;

    // fill vars for BMP header infos
    id[0] = 'B';
    id[1] = 'M';
    reserved1 = reserved2 = 0;
    width = w;
    height = h;
    plane_count = 1;
    bit_count = channel_count * 8;
    compression = 0;
    data_size_with_paddings = data_size + (h * paddings);
    x_resolution = y_resolution = 2835;   // 72 pixels/inch = 2835 pixels/m
    color_count = 0;
    important_color_count = 0;
    info_header_size = 40;                // should be 40 bytes
    data_offset = 54;                    // fileHeader(14) + infoHeader(40)
    file_size = data_size_with_paddings + data_offset;

    // 8-bit grayscale image need palette
    // correct color_count, data_offset and file_size
    if(channel_count == 1){
        color_count = 256;                   // always use max number of colors for 8-bit gray scale
        palette_size = color_count * 4;       // BGRA for each
        data_offset = 54 + palette_size;      // add up palette size
        file_size = data_size_with_paddings + data_offset;   // reset file size
    }

    // allocate output data array
    auto tmp_data = std::shared_ptr<byte_t>(new byte_t[data_size],std::default_delete<byte_t[]>());

    // copy image data
    std::memcpy(tmp_data.get(), data, data_size);

    // flip the image upside down
    // If height is  negative, then it is bottom-top orientation (no need to flip)
    if(height > 0)
        flip_image(tmp_data.get(), width, height, channel_count);

    // convert RGB to BGR order
    if(channel_count == 3 || channel_count == 4)
        swap_RB(tmp_data.get(), data_size, channel_count);

    // add paddings(0s) if the width of image is not divisible by 4
    std::shared_ptr<byte_t> data_with_paddings(nullptr,std::default_delete<byte_t[]>());
    if(paddings > 0){
        // allocate an array
        // add extra bytes for paddings in case the width is not divisible by 4
        data_with_paddings = std::shared_ptr<byte_t>(new byte_t[data_size_with_paddings],std::default_delete<byte_t[]>());

        int line_width = width * channel_count;       // line width in bytes

        // copy single line at a time
        for(int i = 0; i < height; ++i){
            // restore data by adding paddings
            std::memcpy(&data_with_paddings.get()[i*(line_width+paddings)], &tmp_data.get()[i*line_width], line_width);

            // insert 0s for paddings after copying the current line
            for(int j = 1; j <= paddings; ++j)
                data_with_paddings.get()[(i+1)*(line_width+paddings) - j] = byte_t(0);
        }
    }

    // open output file to write data
    std::fstream file(filename,std::ios::out|std::ios::binary);
    if(!file.is_open()||!file.good())
        throw std::runtime_error("Failed to open an output file.");

    // write header
    file.put(id[0]);
    file.put(id[1]);
    file.write(reinterpret_cast<char*>(&file_size), 4);
    file.write(reinterpret_cast<char*>(&reserved1), 2);
    file.write(reinterpret_cast<char*>(&reserved2), 2);
    file.write(reinterpret_cast<char*>(&data_offset), 4);
    file.write(reinterpret_cast<char*>(&info_header_size), 4);
    file.write(reinterpret_cast<char*>(&width), 4);
    file.write(reinterpret_cast<char*>(&height), 4);
    file.write(reinterpret_cast<char*>(&plane_count), 2);
    file.write(reinterpret_cast<char*>(&bit_count), 2);
    file.write(reinterpret_cast<char*>(&compression), 4);
    file.write(reinterpret_cast<char*>(&data_size_with_paddings), 4);
    file.write(reinterpret_cast<char*>(&x_resolution), 4);
    file.write(reinterpret_cast<char*>(&y_resolution), 4);
    file.write(reinterpret_cast<char*>(&color_count), 4);
    file.write(reinterpret_cast<char*>(&important_color_count), 4);

    // For 8-bit grayscale, insert palette between header block and data block
    if(bit_count == 8){
        auto palette=std::shared_ptr<byte_t>(new byte_t[palette_size],std::default_delete<byte_t[]>());// each entry has 4 bytes(B,G,R,A)
        build_grayscale_palette(palette.get(), palette_size);

        // write palette to the file
        file.write(reinterpret_cast<char*>(palette.get()), palette_size);
    }

    // write image data
    if(paddings == 0)
        file.write(reinterpret_cast<char*>(tmp_data.get()), data_size);                        // without padding
    else
        file.write(reinterpret_cast<char*>(data_with_paddings.get()), data_size_with_paddings);   // with paddings

    // close the opened file
    file.close();
}

///////////////////////////////////////////////////////////////////////////////
// decode 8-bit RLE data into uncompressed data
// This routine need 2 pointers: the pointer to the encoded input data and
// the pointer to the decoded output data. The length of input array is not
// necessary because the last 2 bytes of input data must be 00 and 01, which
// tells the end of data. So it can stop decoding process.
//
// BMP uses 2-value RLE scheme: the first value contains a count of the number
// of pixels in the run, and the second value contains the value of the pixel
// repeated. For example, 0x3 0xFF means 0xFF 0xFF 0xFF.
//
// If the first value is 0x00, then it is unencoded run mode and a pixel is not
// repeated any more. In unencode run mode, the second value is the the number
// of unencoded pixel values that follow. If the number of pixels is odd, then
// a 0x00 padding value also follows.
// 1st  2nd  EncodedValue  DecodedValue
// ===  ===  ============  ============
//  00   03  FF FE FD 00   FF FE FD
//  00   04  11 12 13 14   11 12 13 14
//
// The second value of unencoded run mode must be greater than and equal to 3.
// If the second value is less than 3, then it specifies special positioning
// operations and does not decode any data themselves.
// 1st  2nd  Meaning
// ===  ===  ==============================================
//  00   00  End of Scanline, Decode new data at the next line
//  00   01  End of Bitmap data, Stop decoding data here
//  00   02  Delta Offset, Move the cursor hori and vert direction
//
// Delta Offset operation requires 4-byte in size: the first and second should
// be 00 and 02, and the third byte is the number of pixels forward in the
// same scanline and the fourth byte is the number of rows to move. For
// example, 00 02 03 04 means move the cursor 3 pixels right, and 4 pixels
// upward. (Note that BMP is bottom-to-top orientation.)
///////////////////////////////////////////////////////////////////////////////
void bmp::decode_RLE8(const byte_t *encoded, byte_t *data){
    if(encoded==nullptr||data==nullptr)
        return;
    byte_t first, second;

    // start decoding, stop when it reaches at the end of decoded data
    for(;;){
        // grab 2 bytes at the current position
        first = *(encoded++);
        second = *(encoded++);

        if(first!=0){  // encoded run mode
            for(int i=0; i < first; ++i)
                *(data++) = second;
        } else {
            if(second == 1)         // reached the end of bitmap
                break;              // must stop decoding

            else if(second == 2)    // delta mark
                encoded += 2;       // do nothing, but move the cursor 2 more bytes

            else {                  // unencoded run mode (second >= 3)
                for(int i=0; i < second; ++i)
                    *(data++) = *(encoded++);

                if(second%2!=0)      // if it is odd number, then there is a padding 0. ignore it
                    encoded++;
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// BMP is bottom-to-top orientation. Flip the image vertically, so the image
// can be rendered from top to bottom orientation
///////////////////////////////////////////////////////////////////////////////
void bmp::flip_image(byte_t *data, int width, int height, int channel_count){
    if(data==nullptr) 
        return;
    int line_size = width * channel_count;
    auto tmp = std::shared_ptr<byte_t>(new byte_t[line_size],std::default_delete<byte_t[]>());
    int half = height / 2;

    int line1 = 0;                          // first line
    int line2 = (height - 1) * line_size;    // last line

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

///////////////////////////////////////////////////////////////////////////////
// swap the position of the 1st and 3rd color components (RGB <-> BGR)
///////////////////////////////////////////////////////////////////////////////
void bmp::swap_RB(byte_t *data,int data_size,int channel_count){
    if(data==nullptr || channel_count < 3 || data_size%channel_count!=0)  // channel_count must be 3 or 4, data_size must be divisible by the number of channels
        return;

    // swap the position of red and blue components
    for(int i=0; i < data_size; i+=channel_count)
        data[i]^=data[i+2]^=data[i]^=data[i+2];
}

///////////////////////////////////////////////////////////////////////////////
// compute the number of used colors in the 8-bit grayscale image
///////////////////////////////////////////////////////////////////////////////
int bmp::get_color_count(const byte_t *data, int data_size){
    if(data==nullptr) 
        return 0;
    constexpr int MAX_COLOR = 256;  // max number of colors in 8-bit grayscale
    int color_count = 0;
    unsigned int colors[MAX_COLOR]={};

    // increment at the same index
    for(int i = 0; i < data_size; ++i)
        colors[data[i]]++;

    // count backward the number of color used in this data
    color_count = MAX_COLOR;
    for(int i = 0; i < MAX_COLOR; ++i){
        if(colors[i] == 0)
            color_count--;
    }

    return color_count;
}

///////////////////////////////////////////////////////////////////////////////
// build palette for 8-bit grayscale image
// Each component(B,G,R,A) of palette will have the same value as data value
// because it is grayscale.
///////////////////////////////////////////////////////////////////////////////
void bmp::build_grayscale_palette(byte_t* palette,int palette_size){
    if(palette==nullptr)
        return;

    // fill B, G, R, with same value and A is 0
    for(int i = 0, j = 0; i < palette_size; i+=4, j++){
        palette[i] = palette[i+1] = palette[i+2] = byte_t(j);
        palette[i+3] = byte_t(0);
    }
}

int bmp::get_width() const noexcept {return width;}

int bmp::get_height() const noexcept {return height;}

int bmp::get_bit_count() const noexcept {return bit_count;}

int bmp::get_data_size() const noexcept {return data_size;}

const std::shared_ptr<typename bmp::byte_t>& bmp::get_data() const noexcept {return data;}

const std::shared_ptr<typename bmp::byte_t>& bmp::get_data_RGB() const noexcept {return data_RGB;}

std::ostream& operator<<(std::ostream &stream,const bmp &image){
    stream << "===== bmp =====" << std::endl
           << "Width: " << image.width << " pixels" << std::endl
           << "Height: " << image.height << " pixels" << std::endl
           << "Bit Count: " << image.bit_count << " bits" << std::endl
           << "Data Size: " << image.data_size  << " bytes" << std::endl;
    return stream;
}

void bmp::BGR2gray(const byte_t *in, int width, int height, byte_t *out){
    if(in==nullptr || out==nullptr)
        return;

    int size = width * height;  // number of pixels

    
    /* slow methode
    for(int i = 0, j = 0; i < size; i++, j+=3)
        out[i] = (byte_t)(0.114f * in[j] + 0.587f * in[j+1] + 0.299f * in[j+2] + 0.5f);
    */
    
    // fast estimation ********************************
    // backup pointers
    // We are going to use parameters (in, out) because they are faster.
    const byte_t *in_ptr = in;
    byte_t *out_ptr = out;

    int tmp = 0;
    for(int i=0;i<size;i++){
        tmp  = *in++;                       // add blue (1 * blue)
        tmp += (*in << 2) + *in;          // add green (5 * green)
        in++;
        tmp += *in++ << 1;                  // add red (2 * red)
        *out++ = (byte_t)(tmp >> 3); // divide by 8
    }

    // restore the pointers to the original addresses
    in = in_ptr;
    out = out_ptr;
}

///////////////////////////////////////////////////////////////////////////////
// convert 24 bit RGB colour image to 8 bit grayscale
///////////////////////////////////////////////////////////////////////////////
void bmp::RGB2gray(const byte_t *in, int width, int height, byte_t *out){
    // check NULL pointer
    if(in==nullptr || out==nullptr)
        return;

    int size = width * height;  // number of pixels

    /* slow methode
    for(int i = 0, j = 0; i < size; i++, j+=3)
        out[i] = byte_t(0.229f * in[j] + 0.587f * in[j+1] + 0.114f * in[j+2] + 0.5f);
    */
    
    // fast estimation ********************************
    // backup pointers
    // We are going to use parameters (in, out) because they are faster.
    const byte_t *in_ptr = in;
    byte_t *out_ptr = out;

    int tmp = 0;
    for(int i=0;i<size;++i){
        tmp  = *(in++) << 1;                  // add red (2 * red)
        tmp += ((*in) << 2) + *in;          // add green (5 * green)
        in++;
        tmp += *(in++);                       // add blue (1 * blue)
        *(out++) = byte_t(tmp >> 3); // divide by 8
    }

    // restore the pointers to the original addresses
    in = in_ptr;
    out = out_ptr;
}
