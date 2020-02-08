#pragma once

struct Color
{
  Color(unsigned char rr, unsigned char gg, unsigned char bb): 
    r(rr), g(gg), b(bb) 
  {}

  unsigned char r;
  unsigned char g;
  unsigned char b;

  static const Color White, Black, Red, Green, Blue, Yellow, Magenta, Cyan;
};