#include <iostream>

#include "complex.hpp"
#include "image.hpp"

typedef unsigned int uint;

Complex pointToComplex(uint x, uint y, uint w, uint h, double rangeRe, double rangeIm, double minRe, double minIm)
{
  return Complex(
    ((double)x / w) * rangeRe + minRe,
    ((double)y / h) * rangeIm + minIm
  );
}

unsigned char falseColorR(uint iter, uint maxIter)
{
  return (unsigned char)round(log(iter)/log(maxIter) * 255.0);
}
unsigned char falseColorG(uint iter, uint maxIter)
{
  return (unsigned char)round(log(iter)/log(maxIter) * 255.0);
}
unsigned char falseColorB(uint iter, uint maxIter)
{
  return (unsigned char)round(log(iter)/log(maxIter) * 255.0);
}

int main()
{
  const uint width = 1920;
  const uint height = 1080;
  const uint len = width * height * 3;
  float ar = (float)width/height;
  float minIm = -1.9;
  float maxIm = 0.8;
  float rangeIm = maxIm - minIm;
  float rangeRe = ar*rangeIm;
  Complex min = Complex(-0.5*rangeRe, minIm);

  Image result{width, height};

  const uint MAX_ITER = 2000;

  Complex z, p;
  uint iter;
  int i;
  for(uint y = 0; y < height; ++y)
  {
    for(uint x = 0; x < width; ++x)
    {
      i = 3*(y*width + x);
      p = pointToComplex(x, y, width, height, rangeRe, rangeIm, min.re, min.im);
      iter = 0;
      z = Complex();
      while(z.magnitudeSq() < 4 && iter++ < MAX_ITER)
      {
        z = Complex(std::abs(z.re), std::abs(z.im));
        z = z*z + p;
      }
      
      unsigned char r = falseColorR(iter, MAX_ITER);
      unsigned char g = falseColorG(iter, MAX_ITER);
      unsigned char b = falseColorB(iter, MAX_ITER);

      result.setPixel(i, Color(r, g, b));
    }
    std::cout << 100.0*i/len << "%\n";
  }

  result.save("output/burning_ship.ppm");
  std::cout << "Done!\n";

  return 0;
}
