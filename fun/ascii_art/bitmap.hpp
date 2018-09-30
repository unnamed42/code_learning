#ifndef ASCIIART_BMP_HPP
#define ASCIIART_BMP_HPP

#include <memory>
#include <string>
#include <iosfwd>
#include <cstdint>

class Bitmap {
  private:
    /**< width of image */
    int32_t m_width;
    /**< height of image, negative if image is bottom-top oriented */
    int32_t m_height;
    /**< bit_count section in info header */
    uint16_t m_bit_count;
    /**< BGR matrix of this image, with padding */
    std::unique_ptr<uint8_t[]> m_pixels;
  public:
    Bitmap()               = default;
    Bitmap(Bitmap &&)      = default;
    Bitmap(const Bitmap &);
    Bitmap(std::istream &is) { read(is); }

    ~Bitmap() = default;

    int32_t        height() const;
    int32_t        width() const { return m_width; }
    uint16_t       bitCount() const { return m_bit_count; }
    uint32_t       imageSize() const { return lineWidth() * height(); }
    const uint8_t *pixels() const { return m_pixels.get(); }

    Bitmap clone() const { return *this; }

    void clear();

    void read(std::istream&);
    void save(std::ostream&) const;

    void swap(Bitmap &);

    /**
     * Convert to grayscale
     */
    void toGrayscale();

    /**
     * Convert image to ASCII art
     * @param path path to file for output
     * @param map character intensity map
     */
    void toASCIIArt(std::ostream &os, std::string map = "@#%xo;:., ");

    /**
     * @return number of used colors in 8-bit image
     */
    unsigned colorCount() const;

    Bitmap &operator=(Bitmap);
  private:
    /**
     * @return bytes of padding for each line
     */
    unsigned padding() const;

    /**
     * @return width (in bytes) of each line, including padding
     */
    unsigned lineWidth() const {
        // equivalent code:
        //   return m_width * (m_bit_count / 8) + padding();
        return (m_width * (m_bit_count >> 3) + 3) & ~0x3;
    }
};

#endif //ASCIIART_BMP_HPP
