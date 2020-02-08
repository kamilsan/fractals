#pragma once

#include "color.hpp"
#include "point.hpp"

class Image
{
  public:
    Image(unsigned width, unsigned height);
    Image(const Image& other);
    Image(Image&& other) noexcept;

    ~Image();

    void clear(const Color& color);
    void fillCircle(const Point& center, unsigned int r, const Color& color);
    void fillRect(const Point& pos, unsigned int width, unsigned int height, const Color& color);
    void drawLine(const Point& pt1, const Point& pt2, const Color& color);
    void setPixel(const Point& pos, const Color& color);
    void setPixel(int x, int y, const Color& color);
    void setPixel(unsigned int index, const Color& color);

    void save(const char* filename) const;
    Color getPixel(const Point& pos) const;
    Color getPixel(unsigned int x, unsigned int y) const;
    Color getPixel(unsigned int index) const;
    unsigned int getWidth() const;
    unsigned int getHeight() const;

    Image& operator=(const Image& other);
    Image& operator=(Image&& other) noexcept;
  private:
    unsigned char* pixels_;
    unsigned width_;
    unsigned height_;
    unsigned len_;
};