#ifndef ASCIIART_BMP_HPP
#define ASCIIART_BMP_HPP

#include <string>
#include <cstdint>

namespace image {

namespace {
using std::uint8_t;
using std::string;
}

class bmp {
    private:
        /**< width of image */
        int32_t m_width;
        /**< height of image, negative if image is bottom-top oriented */
        int32_t m_height;
        /**< bit_count section in info header */
        uint16_t m_bit_count;
        /**< BGR matrix of this image, with padding */
        uint8_t *m_pixels;
    public:
        bmp();
        bmp(bmp &&other);
        bmp(const bmp &other) = delete;
        explicit bmp(const char *path);
        
        ~bmp();
        
        void clear();
        void read(const char *path);
        void save(const char *path);
        
        /**
         * Convert 24-bit true color image to grayscale
         */
        void to_grayscale();
        
        /**
         * Convert image to ASCII art
         * @param path path to file for output
         * @param map character intensity map
         */
        void to_ascii_art(const char *path, const string &map = "@#%xo;:., ");
        
        int32_t width() const;
        int32_t height() const;
        uint16_t bit_count() const;
        uint32_t image_size() const;
        const uint8_t* pixels() const;
    private:
        /**
         * @return bytes of padding for each line
         */
        int padding() const;
};

} // namespace image
#endif //ASCIIART_BMP_HPP
