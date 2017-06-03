#ifndef ASCIIART_BMP_HPP
#define ASCIIART_BMP_HPP

#include <stdint.h>

namespace image {

class bmp {
    public:
        struct file_header {
            uint16_t signature;
            uint32_t file_size;
            uint16_t reserved1;
            uint16_t reserved2;
            uint32_t off_bits;
        } __attribute__((packed));
        
        struct info_header {
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
        bmp(const bmp &other);
        explicit bmp(const char *path);
        
        ~bmp();
        
        void clear();
        void read(const char *path);
        void save(const char *path);
        
        void swap(bmp &other);
        
        /**
         * Convert to grayscale
         */
        void to_grayscale();
        
        /**
         * Convert image to ASCII art
         * @param path path to file for output
         * @param map character intensity map
         */
        void to_ascii_art(const char *path, const char *map = "@#%xo;:., ");
        
        /**
         * @return number of used colors in 8-bit image
         */
        unsigned color_count() const;
        
        int32_t width() const;
        int32_t height() const;
        uint16_t bit_count() const;
        uint32_t image_size() const;
        const uint8_t* pixels() const;
        
        bmp& operator=(bmp other);
    private:
        /**
         * @return bytes of padding for each line
         */
        unsigned padding() const;
        
        /**
         * @return width (in bytes) of each line, including padding
         */
        unsigned line_width() const;
};

} // namespace image

#endif //ASCIIART_BMP_HPP
