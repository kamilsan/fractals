#include <cmath>
#include <algorithm>

#include "complex.hpp"
#include "image.hpp"

typedef unsigned int uint;

Complex pointToComplex(uint x, uint y, double hw, double hh, Complex center, double rangeRe, double rangeIm)
{
  return Complex(
    ((x - hw) / hw) * rangeRe + center.re,
    ((y - hh) / hh) * rangeIm + center.im);
}

Color getColor(const Complex& z, uint iter) {
  // Coloring scheme taken from Taichi example:
  // https://github.com/taichi-dev/taichi/blob/master/python/taichi/examples/simulation/mandelbrot_zoom.py
  
  const auto v = std::log2(iter + 1 - std::log2(std::log2(z.magnitude()))) / 5.0;
  
  unsigned char r = 0;
  unsigned char g = 0;
  unsigned char b = 0;

  if (v < 1.0f) {
    r = (unsigned char)std::min(255.0, std::max(0.0, 255 * std::pow(v, 4)));
    g = (unsigned char)std::min(255.0, std::max(0.0, 255 * std::pow(v, 2.5f)));
    b = (unsigned char)std::min(255.0, std::max(0.0, 255 * v)); 
  } 
  else {
    const auto v2 = std::max(0.0, 2.0 - v);

    r = (unsigned char)std::min(255.0, std::max(0.0, 255 * v2));
    g = (unsigned char)std::min(255.0, std::max(0.0, 255 * std::pow(v2, 1.5f)));
    b = (unsigned char)std::min(255.0, std::max(0.0, 255 * std::pow(v2, 3))); 
  }

  return Color(r, g, b);
}

int main()
{
  const uint width = 1920;
  const uint height = 1080;
  double halfWidth = 0.5*width;
  double halfHeight = 0.5*height;
  double ar = halfHeight/halfWidth;
  double rangeRe = 2.5;
  double rangeIm = rangeRe*ar;
  Complex center = Complex(0, 0);

  Image result{width, height};

  const uint MAX_ITER = 100;

  Complex z, p;
  uint iter;
  int escapeRadiusSq = 50;

  for(uint y = 0; y < height; ++y)
  {
    for(uint x = 0; x < width; ++x)
    {
      p = pointToComplex(x, y, halfWidth, halfHeight, center, rangeRe, rangeIm);
      iter = 0;
      z = Complex();
      while(z.magnitudeSq() < escapeRadiusSq && iter++ < MAX_ITER)
      {
        z = z*z + p;
      }

      if(iter < MAX_ITER)
      {
        const auto color = getColor(z, iter);
        result.setPixel(x, y, color);
      }
      else {
        result.setPixel(x, y, Color::Black);
      }
    }
  }

  result.save("output/mandelbrot.ppm");

  return 0;
}
