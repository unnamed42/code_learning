#ifndef __BMP__
#define __BMP__

#include <memory> // std::shared_ptr
#include <iostream> // std::ostream operator<< overload

class bmp{
    public:
        typedef unsigned char byte_t;
    protected:
        int width;
        int height;
        int bit_count;
        int data_size;
        std::shared_ptr<byte_t> data;
        std::shared_ptr<byte_t> data_RGB;
    private:
        static void decode_RLE8(const byte_t *encoded, byte_t *data);              // decode BMP 8-bit RLE to uncompressed
        static void flip_image(byte_t *data,int width,int height,int channel_count);    // flip the vertical orientation
        static void swap_RB(byte_t *data,int data_size,int channel_count);           // swap the position of red and blue components
        static int get_color_count(const byte_t *data,int data_size);                     // get the number of colors used in 8-bit grayscale image
        static void build_grayscale_palette(byte_t *palette,int palette_size);
    public:
        bmp();
        bmp(const bmp&);
        explicit bmp(const char*);
        void clear();
        void read(const char *filename);
        void save(const char *filename,int width,int height, int channel_count,const byte_t *data);
        
        int get_width() const noexcept;
        int get_height() const noexcept;
        int get_bit_count() const noexcept;
        int get_data_size() const noexcept;
        const std::shared_ptr<byte_t>& get_data() const noexcept;
        const std::shared_ptr<byte_t>& get_data_RGB() const noexcept;
        
        static void BGR2gray(const byte_t *in, int width, int height, byte_t *out);
        static void RGB2gray(const byte_t *in, int width, int height, byte_t *out);
        
    friend std::ostream& operator<<(std::ostream&,const bmp&);
};

#endif
