#include "bitmap.hpp"

#include <map>
#include <cstring>
#include <istream>
#include <ostream>
#include <stdexcept>

using namespace std;

static constexpr uint16_t magic  = 0x4d42; // "BM", file tag for bitmaps
static constexpr unsigned paddings[] = {0, 3, 2, 1};

struct FileHeader {
    uint16_t signature;
    uint32_t file_size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t off_bits;
} __attribute__((packed));

struct InfoHeader {
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
struct Color {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
    uint8_t reserved;

    uint8_t grayscale() {
        int tmp = blue;
        tmp += (green << 2) + green;
        tmp += red << 1;
        return tmp >> 3;
    }

    void toGrayscale() { red = green = blue = grayscale(); }
};

// Little Endian format: [ALPHA][RED][GREEN][BLUE]
struct PaletteColor {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
    uint8_t alpha;

    PaletteColor() = default;

    PaletteColor(Color c)
        : blue(c.blue), green(c.green), red(c.red), alpha(0) {}

    operator uint32_t() const {
        return *reinterpret_cast<const uint32_t*>(this);
    }

};

// palette buffer
static PaletteColor palette[256];

static unique_ptr<uint8_t[]> decodeRLE8(uint8_t in[], unsigned outSize) {
    auto out = make_unique<uint8_t[]>(outSize);
    auto data = out.get();

    if(!in)
        return out;
    uint8_t first, second;

    // start decoding, stop when it reaches at the end of decoded data
    for(;;) {
        // grab 2 bytes at the current position
        first = *(in++);
        second = *(in++);

        if(first) { // encoded run mode
            for(int i = 0; i < first; ++i)
                *(data++) = second;
        } else {
            if(second == 1)      // reached the end of bitmap
                break;           // must stop decoding
            else if(second == 2) // delta mark
                in += 2;         // do nothing, but move the cursor 2 more bytes
            else {               // unencoded run mode (second >= 3)
                for(int i = 0; i < second; ++i)
                    *(data++) = *(in++);
                if(second % 2) // if it is odd number, then there is a padding
                               // 0. ignore it
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
static unique_ptr<uint8_t[]> fromPalette(istream &is, FileHeader &f, InfoHeader &i) {
    auto paletteSize = f.off_bits - (sizeof(FileHeader) + sizeof(InfoHeader));

    is.seekg(sizeof(FileHeader) + sizeof(InfoHeader), ios::beg);
    is.read(reinterpret_cast<char*>(palette), paletteSize);

    // new line width
    auto lw = i.width * 3;
    // padding bytes for 24-bit matrix
    auto padding = paddings[lw & 0x03];
    lw += padding;

    auto height = abs(i.height);
    // to be returned converted matrix
    auto image = make_unique<uint8_t[]>(lw * height);
    auto line = image.get();

    auto old_lw = (i.width + 3) & ~0x03;
    auto buffer = make_unique<uint8_t[]>(old_lw);

    for(auto y = 0; y < height; ++y) {
        auto pixel = reinterpret_cast<Color*>(line);
        is.read(reinterpret_cast<char*>(buffer.get()), old_lw);
        for(auto x = 0; x < i.width; ++x, ++pixel) {
            auto &&color = palette[buffer[x]];
            pixel->red = color.red;
            pixel->green = color.green;
            pixel->blue = color.blue;
        }
        line = reinterpret_cast<uint8_t*>(--pixel) + padding;
    }

    return image;
}

/**
 * Build palette for 8-bit image
 * @param pixels 24-bit image matrix
 * @param width width of image
 * @param height height of image
 * @return color count of palette
 */
static uint32_t buildPalette(uint8_t *pixels, int32_t width, int32_t height) {
    map<uint32_t, uint8_t> palettes;
    uint32_t index = 0;

    auto bytes = pixels;
    // old line width
    auto lw = ((width * 3) + 3) & ~0x3;
    // new line width
    auto new_lw = (width + 3) & ~0x3;

    for(auto y = 0; y < height; ++y) {
        auto bgr = reinterpret_cast<Color*>(pixels);
        for(auto x = 0; x < width; ++x) {
            PaletteColor color{bgr[x]};
            auto it = palettes.find(color);
            if(it == palettes.end()) {
                palette[index] = color;
                bytes[x] = index;
                palettes.emplace(uint32_t(color), index++);
            } else
                bytes[x] = it->second;
        }
        bytes += new_lw;
        pixels += lw;
    }

    return index;
}

Bitmap::Bitmap(const Bitmap &o)
    : m_width(o.m_width), m_height(o.m_height), m_bit_count(o.m_bit_count) {
    auto size = o.imageSize();
    m_pixels = make_unique<uint8_t[]>(size);
    memcpy(m_pixels.get(), o.m_pixels.get(), size);
}

void Bitmap::clear() {
    this->~Bitmap();
    new (this) Bitmap();
}

void Bitmap::read(istream &is) {
    this->clear();

    FileHeader fheader;
    InfoHeader iheader;

    is.read(reinterpret_cast<char*>(&fheader), sizeof(fheader));
    is.read(reinterpret_cast<char*>(&iheader), sizeof(iheader));

    if(fheader.signature != magic)
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
    decltype(m_pixels) image;

    if(iheader.bit_count == 8) {
        image = fromPalette(is, fheader, iheader);
        m_bit_count = 24;
    } else {
        // normal mode
        image = make_unique<uint8_t[]>(iheader.image_size);
        // move cursor to the starting position of data
        is.seekg(fheader.off_bits, ios::beg);
        is.read(reinterpret_cast<char*>(image.get()), iheader.image_size);
    }

    switch(iheader.compression) {
        // uncompressed
        case 0: m_pixels = move(image);
        default: break;
        // 8-bit RLE(Run Length Encode) compressed
        case 1:
            m_pixels = decodeRLE8(image.get(), iheader.image_size);
    }
}

void Bitmap::save(ostream &os) const {
    if(m_width == 0 || m_height == 0)
        throw invalid_argument{"zero width or height"};

    FileHeader fheader;
    InfoHeader iheader;

    // size of palette block (in bytes), BGRA for each
    constexpr auto palette_size = 256 * 4;

    // fill variables for BMP headers
    fheader.signature = magic;
    fheader.reserved1 = fheader.reserved2 = 0;
    fheader.off_bits = sizeof(fheader) + sizeof(iheader); // fileHeader(14) + infoHeader(40)

    iheader.width = m_width;
    iheader.height = m_height;
    iheader.planes = 1;
    iheader.bit_count = m_bit_count;
    iheader.compression = 0;
    iheader.image_size = imageSize();
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

    // For 8-bit, insert palette between header block and data block
    // and color count fix
    if(m_bit_count == 8)
        iheader.colors = buildPalette(m_pixels.get(), m_width, height());

    // write header
    os.write(reinterpret_cast<char*>(&fheader), sizeof(fheader));
    os.write(reinterpret_cast<char*>(&iheader), sizeof(iheader));
    if(m_bit_count == 8)
        os.write(reinterpret_cast<char*>(palette), iheader.colors * sizeof(uint32_t));
    os.write(reinterpret_cast<char*>(m_pixels.get()), iheader.image_size);
}

void Bitmap::swap(Bitmap &other) {
    std::swap(m_width, other.m_width);
    std::swap(m_height, other.m_height);
    std::swap(m_bit_count, other.m_bit_count);
    std::swap(m_pixels, other.m_pixels);
}

void Bitmap::toGrayscale() {
    auto line_width = this->lineWidth();
    auto line = m_pixels.get();

    for(auto y = 0; y < height(); ++y) {
        auto pixels = reinterpret_cast<Color*>(line);
        for(auto x = 0; x < width(); ++x)
            pixels[x].toGrayscale();
        line += line_width;
    }

    m_bit_count = 8;
}

void Bitmap::toASCIIArt(ostream &os, string map) {
    int line_width = this->lineWidth();
    auto line = m_pixels.get() + line_width * (height() - 1);

    // if height > 0, then pixels are ordered bottom-to-top
    if(m_height > 0)
        line_width = -line_width;

    auto len = map.size();

    for(auto y = 0; y < height(); ++y) {
        auto pixels = reinterpret_cast<Color*>(line);
        for(auto x = 0; x < width(); ++x) {
            auto gray = m_bit_count == 8 ? line[x] : pixels[x].grayscale();
            os << map[gray * len / 256];
        }
        line += line_width;
        os << '\n';
    }
}

unsigned Bitmap::colorCount() const {
    if(!m_pixels || m_bit_count != 8)
        throw invalid_argument{"not an 8-bit image"};

    static constexpr auto max_color = 256;
    // count different indexes
    bool colors[max_color] = {};
    unsigned ret = 0;

    auto line = m_pixels.get();
    auto line_width = this->lineWidth();

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

int32_t Bitmap::height() const {
    return abs(m_height);
}

unsigned Bitmap::padding() const {
    // In BMP, each scanline must be divisible evenly by 4
    // If not, add extra paddings in each line, it can be divisible by 4.

    // equivalent code:
    //   auto padding = (m_width * m_bit_count / 8) % 4;
    //   return padding ? 4 - padding : 0;
    return paddings[(m_width * (m_bit_count >> 3)) & 0x3];
}

Bitmap& Bitmap::operator=(Bitmap other) {
    swap(other);
    return *this;
}
