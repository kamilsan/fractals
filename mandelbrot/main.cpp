#include <cmath>

#include "complex.hpp"
#include "image.hpp"

typedef unsigned int uint;

Complex pointToComplex(uint x, uint y, double hw, double hh, Complex center, double rangeRe, double rangeIm)
{
  return Complex(
    ((x - hw) / hw) * rangeRe + center.re,
    ((y - hh) / hh) * rangeIm + center.im);
}

float interpolate(int a, int b, float t)
{
  return (1.0-t)*a + t*b;
}

void sRGBEncode(float& c)
{
  float y = 1.0f/2.4f;

  if(c <= 0.0031308f) c *= 12.92f;
  else c = 1.055f * powf(c, y) - 0.055f;

}
void sRGBDecode(float& c)
{
  if(c <= 0.04045f) c /= 12.92f;
  else c = powf((c+0.055f)/1.055f, 2.4f);
}

/*
Position = 0.0     Color = (0,   7,   100)
Position = 0.16    Color = (32,  107, 203)
Position = 0.42    Color = (237, 255, 255)
Position = 0.6425  Color = (255, 170, 0)
Position = 0.8575  Color = (0,   2,   0)
Position = 1.0     Color = (0,   7,   100)
*/

unsigned char falseColorR(float v, uint max)
{
  float t = log(v)/log(max);
  float c;
  if (t <= 0.16)
  {
    t /= 0.16;
    c = interpolate(0, 32, t);
  }
  else if(t <= 0.42)
  {
    t = (t-0.16)/(0.42-0.16);
    c = interpolate(32, 237, t);
  }
  else if(t <= 0.6425)
  {
    t = (t-0.42)/(0.6425-0.42);
    c = interpolate(237, 255, t);
  }
  else if(t <= 0.8575)
  {
    t = (t-0.6425)/(0.8575-0.6425);
    c = interpolate(255, 0, t);
  }
  else
  {
    t = (t-0.8575)/(1.0-0.8575);
    c = interpolate(0, 0, t);
  }
  return (unsigned char)(c + 0.5);
}
unsigned char falseColorG(float v, uint max)
{
  float t = log(v)/log(max);
  float c;
  if (t <= 0.16)
  {
    t /= 0.16;
    c = interpolate(7, 107, t);
  }
  else if(t <= 0.42)
  {
    t = (t-0.16)/(0.42-0.16);
    c = interpolate(107, 255, t);
  }
  else if(t <= 0.6425)
  {
    t = (t-0.42)/(0.6425-0.42);
    c = interpolate(255, 170, t);
  }
  else if(t <= 0.8575)
  {
    t = (t-0.6425)/(0.8575-0.6425);
    c = interpolate(170, 2, t);
  }
  else
  {
    t = (t-0.8575)/(1.0-0.8575);
    c = interpolate(2, 7, t);
  }
  return (unsigned char)(c + 0.5);
}
unsigned char falseColorB(float v, uint max)
{
  float t = log(v)/log(max);
  float c;
  if (t <= 0.16)
  {
    t /= 0.16;
    c = interpolate(100, 203, t);
  }
  else if(t <= 0.42)
  {
    t = (t-0.16)/(0.42-0.16);
    c = interpolate(203, 255, t);
  }
  else if(t <= 0.6425)
  {
    t = (t-0.42)/(0.6425-0.42);
    c = interpolate(255, 0, t);
  }
  else if(t <= 0.8575)
  {
    t = (t-0.6425)/(0.8575-0.6425);
    c = interpolate(0, 0, t);
  }
  else
  {
    t = (t-0.8575)/(1.0-0.8575);
    c = interpolate(0, 100, t);
  }
  return (unsigned char)(c + 0.5);
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

  const uint MAX_ITER = 5000;

  Complex z, p;
  uint iter;
  float smooth;
  int escapeRadiusSq = 100;

  float invLogEx = 1.0/log(2);
  float logTerm = log(log(escapeRadiusSq))*invLogEx;

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
      z = z*z + p;
      z = z*z + p;
      iter += 2;
      if(iter < MAX_ITER)
      {
        smooth = float(iter) + logTerm - log(log(z.magnitudeSq()))*invLogEx;
        smooth = std::fmin(smooth, (float)MAX_ITER);

        unsigned char r = falseColorR(smooth, MAX_ITER);
        unsigned char g = falseColorG(smooth, MAX_ITER);
        unsigned char b = falseColorB(smooth, MAX_ITER);

        result.setPixel(x, y, Color(r, g, b));
      }
      else
        result.setPixel(x, y, Color::Black);
    }
  }

  result.save("output/mandelbrot.ppm");

  return 0;
}
