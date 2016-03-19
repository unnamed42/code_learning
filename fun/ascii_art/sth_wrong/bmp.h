#ifndef __BMP__
#define __BMP__

#include <memory> // std::shared_ptr
#include <cstdint> // for fixed-length integral types

namespace image{
    
    struct bmp_file_header {
        uint8_t signature[2];
        uint32_t file_size;
        uint16_t reserved1;
        uint16_t reserved2;
        uint32_t off_bits;
    } __attribute__((packed));
    
    struct bmp_info_header{
        uint32_t info_header_size;
        int32_t width;
        int32_t height;
        uint16_t planes;
        uint16_t bit_count;
        uint32_t compression;
        uint32_t image_size;
        int32_t x_res;
        int32_t y_res;
        uint32_t color_used;
        uint32_t color_important;
    }__attribute__((packed));
    
    struct rgbquad{
        uint8_t blue;
        uint8_t green;
        uint8_t red;
        uint8_t reserved;
    }__attribute__((packed));
    
    class bmp{
        protected:
            int32_t m_width;
            int32_t m_height;
            uint16_t m_bit_count;
            uint32_t m_data_size;
            std::shared_ptr<rgbquad> m_data;
        public:
            explicit bmp();
            explicit bmp(const char *filename);
            bmp(const bmp&);
            
            void clear();
            void show_info() const;
            void read(const char *filename);
            void save(const char *filename);
            
            int32_t width() const noexcept;
            int32_t height() const noexcept;
            uint16_t bit_count() const noexcept;
            uint32_t data_size() const noexcept;
            std::shared_ptr<uint8_t> data() const noexcept;
            //std::shared_ptr<uint8_t> data_RGB() const noexcept;
            
            static void save(const char *filename,int32_t width,int32_t height,uint16_t bit_count,const rgbquad data[]);
            
            static std::shared_ptr<uint8_t> decode_RLE8(rgbquad input[],uint32_t size);
            
            static void rotate(rgbquad data[],int32_t width,int32_t height,uint32_t bit_count);
            
            static void swap_RB(rgbquad data[],uint32_t data_size,uint16_t bit_count);
            
            // get the number of colors used in 8-bit grayscale image
            static uint32_t get_color_count(const rgbquad data[],uint32_t data_size);
            
            static void build_grayscale_palette(uint8_t *palette,uint32_t palette_size);
            
            static std::shared_ptr<uint8_t> BGR_to_gray(const rgbquad input[],int32_t width,int32_t height);
            
            static std::shared_ptr<uint8_t> RGB_to_gray(const rgbquad input[],int32_t width,int32_t height);
    };
} // namespace image

#endif // __BMP__
