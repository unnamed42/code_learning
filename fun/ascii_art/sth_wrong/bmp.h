#include <memory>
#include <cstdint>

class bmp{
    public:
        struct file_header {
            uint8_t signature[2];
            uint32_t file_size;
            uint16_t reserved1;
            uint16_t reserved2;
            uint32_t off_bits;
        } __attribute__((packed));
        struct info_header{
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
    protected:
        int32_t _width;
        int32_t _height;
        uint16_t _bit_count;
        uint32_t _data_size;
        std::shared_ptr<uint8_t> _data;
        //std::shared_ptr<uint8_t> _data_RGB;
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
        
        static void save(const char *filename,int32_t width,int32_t height,uint16_t bit_count,const uint8_t *data);
        static std::shared_ptr<uint8_t> decode_RLE8(uint8_t *input,uint32_t size);
        static void rotate(uint8_t *data,int32_t width,int32_t height,uint32_t bit_count);
        static void swap_RB(uint8_t *data,uint32_t data_size,uint16_t bit_count);
        static uint32_t get_color_count(const uint8_t *data,uint32_t data_size);                     // get the number of colors used in 8-bit grayscale image
        static void build_grayscale_palette(uint8_t *palette,uint32_t palette_size);
        static std::shared_ptr<uint8_t> BGR_to_gray(const uint8_t *input,int32_t width,int32_t height);
        static std::shared_ptr<uint8_t> RGB_to_gray(const uint8_t *input,int32_t width,int32_t height);
};
