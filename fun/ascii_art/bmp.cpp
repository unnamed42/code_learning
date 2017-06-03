#include "bmp.hpp"

#include <string.h>

#include <map>
#include <fstream>
#include <stdexcept>

using namespace image;

using std::abs;
using std::swap;

using std::map;
using std::ios;
using std::istream;
using std::ostream;
using std::ifstream;
using std::ofstream;
using std::invalid_argument;

using file_header = bmp::file_header;
using info_header = bmp::info_header;

static constexpr uint16_t file_type  = 0x4d42; // "BM", file tag for bitmaps
static constexpr unsigned paddings[] = {0, 3, 2, 1};

// MSDN spec says bmp images are BGR colors
struct color_t {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
    //uint8_t reserved;
} __attribute__((packed));

// Little Endian format: [ALPHA][RED][GREEN][BLUE]
struct palette_color_t {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
    uint8_t alpha;
    
    static palette_color_t from_BGR(const color_t &c) {
        return palette_color_t{c.blue, c.green, c.red, 0};
    }
    
    operator uint32_t() const {
        return *reinterpret_cast<const uint32_t*>(this);
    }
    
} __attribute__((packed));

// palette buffer
static palette_color_t palette[256];

/**
 * @param pixel an rgb pixel
 * @return computed grayscale value of pixel
 */
static uint8_t grayscale(const color_t &pixel) {
    int tmp = pixel.blue;
    tmp += (pixel.green << 2) + pixel.green;
    tmp += pixel.red << 1;
    return tmp >> 3;
}

/**
 * Convert an rgb pixel to grayscale
 * @param pixel an rgb pixel
 */
static void to_grayscale(color_t &pixel) {
    pixel.red = pixel.green = pixel.blue = grayscale(pixel);
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

/**
 * Convert 8-bit palette and index array into rgb matrix
 * @param is source data stream
 * @param f file header
 * @param i info header
 * @return converted matrix
 */
static uint8_t* from_palette(istream &is, file_header &f, info_header &i) {
    auto palette_size = f.off_bits - (sizeof(file_header) + sizeof(info_header));
    
    is.seekg(sizeof(file_header) + sizeof(info_header), ios::beg);
    is.read(reinterpret_cast<char*>(palette), palette_size);
    
    // new line width
    auto lw = i.width * 3;
    // padding bytes for 24-bit matrix
    auto padding = paddings[lw & 0x03];
    lw += padding;
    
    auto height = abs(i.height);
    // to be returned converted matrix
    auto image = new uint8_t[lw * height];
    auto line = image;
    
    auto old_lw = (i.width + 3) & ~0x03;
    auto buffer = new uint8_t[old_lw];
    
    for(auto y = 0; y < height; ++y) {
        auto pixel = reinterpret_cast<color_t*>(line);
        is.read(reinterpret_cast<char*>(buffer), old_lw);
        for(auto x = 0; x < i.width; ++x, ++pixel) {
            auto &&color = palette[buffer[x]];
            pixel->red = color.red;
            pixel->green = color.green;
            pixel->blue = color.blue;
        }
        line = reinterpret_cast<uint8_t*>(--pixel) + padding;
    }
    
    delete[] buffer;
    return image;
}

/**
 * Build palette for 8-bit image
 * @param pixels 24-bit image matrix
 * @param width width of image
 * @param height height of image
 * @return color count of palette
 */
static uint32_t build_palette(uint8_t *pixels, int32_t width, int32_t height) {
    map<uint32_t, uint8_t> palette_map;
    uint32_t index = 0;
    
    auto bytes = pixels;
    // old line width
    auto lw = ((width * 3) + 3) & ~0x3;
    // new line width
    auto new_lw = (width + 3) & ~0x3;
    
    for(auto y = 0; y < height; ++y) {
        auto bgr = reinterpret_cast<color_t*>(pixels);
        for(auto x = 0; x < width; ++x) {
            auto color = palette_color_t::from_BGR(bgr[x]);
            auto it = palette_map.find(color);
            if(it == palette_map.end()) {
                palette[index] = color;
                bytes[x] = index;
                palette_map.emplace(uint32_t(color), index++);
            } else
                bytes[x] = it->second;
        }
        bytes += new_lw;
        pixels += lw;
    }
    
    return index;
}

bmp::bmp()
    : m_width(0), m_height(0), m_bit_count(0), m_pixels(nullptr) {}

bmp::bmp(bmp &&other)
    : m_width(other.m_width), m_height(other.m_height), m_bit_count(other.m_bit_count),
      m_pixels(other.m_pixels) {
    other.m_pixels = nullptr;
}

bmp::bmp(const char *path) try: m_pixels(nullptr) {
    read(path);
} catch(...) {
    delete[] m_pixels;
    throw;
}

bmp::bmp(const bmp &other)
    : m_width(other.m_width), m_height(other.m_height),
      m_bit_count(other.m_bit_count) {
    auto size = other.image_size();
    m_pixels = new uint8_t[size];
    memcpy(m_pixels, other.m_pixels, size);
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
    
    file_header fheader;
    info_header iheader;
    
    file.read(reinterpret_cast<char*>(&fheader), sizeof(fheader));
    file.read(reinterpret_cast<char*>(&iheader), sizeof(iheader));
    
    if(fheader.signature != file_type)
        throw invalid_argument{"file is not a bmp image"};
    
    // supports only 8-bit, 24-bit BGR or 32-bit BGRA
    if(iheader.bit_count & 0x07)
        throw invalid_argument{"file format not supported"};
    
    // supports only uncompressed and 8-bit RLE compressed format
    if(iheader.compression > 1)
        throw invalid_argument{"unsupported compression mode"};
    
    // now it is ready to store info and image data
    m_width = iheader.width;
    m_height = iheader.height;
    m_bit_count = iheader.bit_count;
    
    // pixel matrix with paddings
    uint8_t *image;
    
    if(iheader.bit_count == 8) {
        image = from_palette(file, fheader, iheader);
        m_bit_count = 24;
    } else {
        // normal mode
        image = new uint8_t[iheader.image_size];
        // move cursor to the starting position of data
        file.seekg(fheader.off_bits, ios::beg);
        file.read(reinterpret_cast<char*>(image), iheader.image_size);
    }
    
    switch(iheader.compression) {
        // uncompressed
        case 0: m_pixels = image;
        default: break;
        // 8-bit RLE(Run Length Encode) compressed
        case 1:
            m_pixels = decode_RLE8(image, iheader.image_size);
            delete[] image;
    }
}

void bmp::save(const char *path) {
    if(m_width == 0 || m_height == 0)
        throw invalid_argument{"zero width or height"};
    
    file_header fheader;
    info_header iheader;
    
    // size of palette block (in bytes), BGRA for each
    constexpr auto palette_size = 256 * 4;
    
    // fill variables for BMP headers
    fheader.signature = file_type;
    fheader.reserved1 = fheader.reserved2 = 0;
    fheader.off_bits = sizeof(fheader) + sizeof(iheader); // fileHeader(14) + infoHeader(40)
    
    iheader.width = m_width;
    iheader.height = m_height;
    iheader.planes = 1;
    iheader.bit_count = m_bit_count;
    iheader.compression = 0;
    iheader.image_size = image_size();
    iheader.x_res = iheader.y_res = 2835; // 72 pixels/inch = 2835 pixels/m
    iheader.colors = 0;
    iheader.important_colors = 0;
    iheader.header_size = sizeof(iheader); // should be 40 bytes
    
    // 8-bit image need palette
    // correct color_count, off_bits
    if(m_bit_count == 8) {
        // always use max number of colors for 8-bit gray scale
        iheader.colors = 256;
        // add up palette size
        fheader.off_bits += palette_size;
    }
    
    fheader.file_size = iheader.image_size + fheader.off_bits;
    
    // open output file to write data
    ofstream file(path, ios::binary);
    
    // For 8-bit, insert palette between header block and data block
    // and color count fix
    if(m_bit_count == 8)
        iheader.colors = build_palette(m_pixels, m_width, height());
    
    // write header
    file.write(reinterpret_cast<char*>(&fheader), sizeof(fheader));
    file.write(reinterpret_cast<char*>(&iheader), sizeof(iheader));
    if(m_bit_count == 8)
        file.write(reinterpret_cast<char*>(palette), iheader.colors * sizeof(uint32_t));
    file.write(reinterpret_cast<char*>(m_pixels), iheader.image_size);
}

void bmp::swap(bmp &other) {
    ::swap(m_width, other.m_width);
    ::swap(m_height, other.m_height);
    ::swap(m_bit_count, other.m_bit_count);
    ::swap(m_pixels, other.m_pixels);
}

void bmp::to_grayscale() {
    auto line_width = this->line_width();
    auto line = m_pixels;
    
    for(auto y = 0; y < height(); ++y) {
        auto pixels = reinterpret_cast<color_t*>(line);
        for(auto x = 0; x < width(); ++x)
            ::to_grayscale(pixels[x]);
        line += line_width;
    }
    
    m_bit_count = 8;
}

void bmp::to_ascii_art(const char *path, const char *map) {
    int line_width = this->line_width();
    auto line = m_pixels + line_width * (height() - 1);
    
    // if height > 0, then pixels are ordered bottom-to-top
    if(m_height > 0)
        line_width = -line_width;
    
    ofstream file(path, ios::binary);
    unsigned len = strlen(map);
    
    for(auto y = 0; y < height(); ++y) {
        auto pixels = reinterpret_cast<color_t*>(line);
        for(auto x = 0; x < width(); ++x) {
            auto gray = m_bit_count == 8 ? line[x] : grayscale(pixels[x]);
            file << map[gray * len / 256];
        }
        line += line_width;
        file << '\n';
    }
}

unsigned bmp::color_count() const {
    if(!m_pixels || m_bit_count != 8)
        throw invalid_argument{"not an 8-bit image"};
    
    static constexpr auto max_color = 256;
    // count different indexes
    bool colors[max_color] = {};
    unsigned ret = 0;
    
    auto line = m_pixels;
    auto line_width = this->line_width();
    
    for(auto y = 0; y < height(); ++y) {
        for(auto x = 0; x < width(); ++x) {
            if(!colors[line[x]]) {
                colors[line[x]] = true;
                ++ret;
            }
        }
        line += line_width;
    }
    
    return ret;
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
    return line_width() * height();
}

const uint8_t* bmp::pixels() const {
    return m_pixels;
}

unsigned bmp::padding() const {
    // In BMP, each scanline must be divisible evenly by 4
    // If not, add extra paddings in each line, it can be divisible by 4.
    
    // equivalent code:
    //   auto padding = (m_width * m_bit_count / 8) % 4;
    //   return padding ? 4 - padding : 0;
    return paddings[(m_width * (m_bit_count >> 3)) & 0x3];
}

unsigned bmp::line_width() const {
    // equivalent code:
    //   return m_width * (m_bit_count / 8) + padding();
    return (m_width * (m_bit_count >> 3) + 3) & ~0x3;
}

bmp& bmp::operator=(bmp other) {
    swap(other);
    return *this;
}