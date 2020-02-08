#include <chrono>
#include <random>
#include <vector>

#include "image.hpp"

struct Pointf
{
  Pointf(float x, float y): x(x), y(y) {}

  float x;
  float y;
};

int main()
{
  int width = 800;
  int height = 1200;
  int len = width*height;
  int* heatmap = new int[len];

  const int maxN = 500000000;

  float x = 0;
  float y = 0;
  float minX = 10e6, minY = 10e6, maxX = 10e-6, maxY = 10e-6;

  std::mt19937 generator(std::chrono::system_clock::now().time_since_epoch().count());
  std::uniform_real_distribution<float> dist;

  std::vector<Pointf> points;
  points.reserve(maxN);

  float r;
  int index;
  int max = 0;
  for(int n = 0; n < maxN; ++n)
  {
    r = dist(generator);
    if(r <= 0.01)//0.01
    {
      x = 0;
      y = 0.16*y;
    }
    else if(r <= 0.08)//0.07
    {
      float x0 = x;
      float y0 = y;
      x = 0.2*x0 - 0.26*y0;
      y = 0.23*x0 + 0.22*y0 + 1.6;
    }
    else if(r <= 0.15)//0.07
    {
      float x0 = x;
      float y0 = y;
      x = -0.15*x0 + 0.28*y0;
      y = 0.26*x0 + 0.24*y0 + 0.44;
    }
    else//0.85
    {
      float x0 = x;
      float y0 = y;
      x = 0.85*x0 + 0.04*y0;
      y = -0.04*x0 + 0.85*y0 + 1.6;
    }
    Pointf p(x, y);
    points.push_back(p);
    if(x < minX) minX = x;
    else if(x > maxX) maxX = x;
    if(y < minY) minY = y;
    else if(y > maxY) maxY = y;
  }


  float invDeltaX = 1.0/(maxX - minX);
  float invDeltaY = 1.0/(maxY - minY);
  for(Pointf& p : points)
  {
    int tx = 0.5 + width*(p.x - minX)*invDeltaX;
    int ty = 0.5 + height*(-p.y + maxY)*invDeltaY;
    if(tx >= 0 && tx < width && ty >= 0 && ty < height)
    {
      index = ty*width + tx;
      heatmap[index]++;
      if(heatmap[index] > max) max = heatmap[index];
    }
  }

  points.clear();

  Image result{(unsigned int)width, (unsigned int)height};
  result.clear(Color::White);

  for(int i = 0; i < len; ++i)
  {
    index = 3*i;
    float v = log(heatmap[i])/log(max);

    unsigned char r = (unsigned char)std::min(255.0*0.4*v + 0.5, 255.0);
    unsigned char g = (unsigned char)std::min(255.0*v + 0.5, 255.0);
    unsigned char b = 0;
    result.setPixel(3*i, Color(r, g, b));
  }

  result.save("output/fern.ppm");

  delete[] heatmap;

  return 0;
}
