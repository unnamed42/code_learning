#include "bmp.hpp"

#include <cstring>
#include <fstream>
#include <stdexcept>

using namespace image;

using std::abs;
using std::swap;
using std::memmove;

using std::ios;
using std::string;
using std::ostream;
using std::ifstream;
using std::ofstream;
using std::invalid_argument;

static constexpr uint16_t file_type = 0x4d42; // "BM", file tag for bitmaps

struct bmp_file_header {
    uint16_t signature;
    uint32_t file_size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t off_bits;
} __attribute__((packed));

struct bmp_info_header {
    uint32_t header_size;
    int32_t  width;
    int32_t  height;
    uint16_t planes;
    uint16_t bit_count;
    uint32_t compression;
    uint32_t image_size;
    int32_t  x_res;
    int32_t  y_res;
    uint32_t colors;
    uint32_t important_colors;
} __attribute__((packed));

// MSDN spec says bmp images are BGR colors
struct rgbquad {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
    //uint8_t reserved;
} __attribute__((packed));

static void write_grayscale_palette(ostream &os) {
    uint32_t palette[256];
    uint32_t data = 0;
    for(auto &pixel: palette) {
        // Little Endian format: [ZERO][RED][GREEN][BLUE]
        pixel = (data << 16) | (data << 8) | data;
        ++data;
    }
    os.write(reinterpret_cast<char*>(palette), sizeof(palette));
}

static uint8_t BGR_to_gray(const rgbquad &pixel) {
    int tmp = pixel.blue;
    tmp += (pixel.green << 2) + pixel.green;
    tmp += pixel.red << 1;
    return tmp >> 3;
}

static uint8_t* decode_RLE8(uint8_t in[], unsigned out_size) {
    auto out = new uint8_t[out_size];
    auto data = out;
    
    if(!in)
        return out;
    uint8_t first, second;
    
    // start decoding, stop when it reaches at the end of decoded data
    for(;;) {
        // grab 2 bytes at the current position
        first = *(in++);
        second = *(in++);
        
        if(first) {  // encoded run mode
            for(int i=0; i < first; ++i)
                *(data++) = second;
        } else {
            if(second == 1)         // reached the end of bitmap
                break;              // must stop decoding
            else if(second == 2)    // delta mark
                in += 2;       // do nothing, but move the cursor 2 more bytes
            else {                  // unencoded run mode (second >= 3)
                for(int i=0; i < second; ++i)
                    *(data++) = *(in++);
                if(second%2)      // if it is odd number, then there is a padding 0. ignore it
                    ++in;
            }
        }
    }
    return out;
}

bmp::bmp()
    : m_width(0), m_height(0), m_bit_count(0), m_pixels(nullptr) {}

bmp::bmp(bmp &&other)
    : m_width(other.m_width), m_height(other.m_height), m_bit_count(other.m_bit_count),
      m_pixels(other.m_pixels) {
    other.m_pixels = nullptr;
}

bmp::bmp(const char *path): m_pixels(nullptr) {
    try {
        read(path);
    } catch(...) {
        this->~bmp();
        throw;
    }
}

bmp::~bmp() {
    delete[] m_pixels;
}

void bmp::clear() {
    this->~bmp();
    new (this) bmp();
}

void bmp::read(const char *path) {
    this->clear();
    
    ifstream file(path, ios::binary);
    
    if(!file.good())
        throw invalid_argument{"cannot open file"};
    
    bmp_file_header file_header;
    bmp_info_header info_header;
    
    file.read(reinterpret_cast<char*>(&file_header), sizeof(file_header));
    file.read(reinterpret_cast<char*>(&info_header), sizeof(info_header));
    
    if(file_header.signature != file_type)
        throw invalid_argument{"file is not a bmp image"};
    
    // supports only 8-bit grayscale, 24-bit BGR or 32-bit BGRA
    if(info_header.bit_count < 8)
        throw invalid_argument{"file format not supported"};
    
    // supports only uncompressed and 8-bit RLE compressed format
    if(info_header.compression > 1)
        throw invalid_argument{"unsupported compression mode"};
    
    // now it is ready to store info and image data
    m_width = info_header.width;
    m_height = info_header.height;
    m_bit_count = info_header.bit_count;
    
    // move cursor to the starting position of data
    file.seekg(file_header.off_bits, ios::beg);
    
    // pixel matrix with paddings
    auto image = new uint8_t[info_header.image_size];
    file.read(reinterpret_cast<char*>(image), info_header.image_size);
    
    if(info_header.compression == 0) { // uncompressed
        m_pixels = image;
    } else if(info_header.compression == 1) { // 8-bit RLE(Run Length Encode) compressed
        m_pixels = decode_RLE8(image, info_header.image_size);
        delete[] image;
    }
}

void bmp::save(const char *path) {
    if(m_width == 0 || m_height == 0)
        throw invalid_argument{"zero width or height"};
    
    bmp_file_header file_header;
    bmp_info_header info_header;
    
    // size of palette block (in bytes), BGRA for each
    constexpr auto palette_size = 256 * 4;
    
    // fill variables for BMP headers
    file_header.signature = file_type;
    file_header.reserved1 = file_header.reserved2 = 0;
    file_header.off_bits = sizeof(file_header) + sizeof(info_header); // fileHeader(14) + infoHeader(40)
    
    info_header.width = m_width;
    info_header.height = m_height;
    info_header.planes = 1;
    info_header.bit_count = m_bit_count;
    info_header.compression = 0;
    info_header.image_size = image_size();
    info_header.x_res = info_header.y_res = 2835; // 72 pixels/inch = 2835 pixels/m
    info_header.colors = 0;
    info_header.important_colors = 0;
    info_header.header_size = sizeof(info_header); // should be 40 bytes
    
    // 8-bit grayscale image need palette
    // correct color_count, off_bits
    if(m_bit_count == 8) {
        // always use max number of colors for 8-bit gray scale
        info_header.colors = 256;
        // add up palette size
        file_header.off_bits += palette_size;
    }
    
    file_header.file_size = info_header.image_size + file_header.off_bits;
    
    // open output file to write data
    ofstream file(path, ios::binary);
    
    // write header
    file.write(reinterpret_cast<char*>(&file_header), sizeof(file_header));
    file.write(reinterpret_cast<char*>(&info_header), sizeof(info_header));
    
    // For 8-bit grayscale, insert palette between header block and data block
    if(m_bit_count == 8)
        write_grayscale_palette(file);
    
    file.write(reinterpret_cast<char*>(m_pixels), info_header.image_size);
}

void bmp::to_grayscale() {
    if(m_bit_count == 8)
        return;
    
    auto line_width = m_width * m_bit_count / 8;
    // add the number of paddings
    line_width += (4 - (m_width * m_bit_count / 8) % 4) % 4;
    
    auto line = m_pixels;
    
    m_bit_count = 8;
    auto new_image = new uint8_t[image_size()];
    auto new_line_width = m_width + padding();
    auto gray = new_image;
    
    for(auto y = 0; y < height(); ++y) {
        auto pixels = reinterpret_cast<rgbquad*>(line);
        for(auto x = 0; x < width(); ++x)
            gray[x] = BGR_to_gray(pixels[x]);
        gray += new_line_width;
        line += line_width;
    }
    
    delete[] m_pixels;
    m_pixels = new_image;
}

void bmp::to_ascii_art(const char *path, const string &map) {
    auto line_width = m_width * m_bit_count / 8 + padding();
    
    auto line = m_pixels + line_width * (height() - 1);
    
    ofstream file(path, ios::binary);
    unsigned len = map.size();
    
    for(auto y = 0; y < height(); ++y) {
        auto pixels = reinterpret_cast<rgbquad*>(line);
        for(auto x = 0; x < width(); ++x) {
            auto gray = m_bit_count == 8 ? line[x] : BGR_to_gray(pixels[x]);
            file << map[gray * len / 256];
        }
        line -= line_width;
        file << '\n';
    }
}

int bmp::width() const {
    return m_width;
}

int bmp::height() const {
    return abs(m_height);
}

uint16_t bmp::bit_count() const {
    return m_bit_count;
}

uint32_t bmp::image_size() const {
    return (m_width * m_bit_count / 8 + padding()) * height();
}

const uint8_t* bmp::pixels() const {
    return m_pixels;
}

int bmp::padding() const {
    // In BMP, each scanline must be divisible evenly by 4
    // If not, add extra paddings in each line, it can be divisible by 4.
    return (4 - ((m_width * m_bit_count / 8) % 4)) % 4;
}
