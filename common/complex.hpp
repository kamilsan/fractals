#pragma once

#include <cmath>

class Complex
{
public:
  Complex(double re = 0, double im = 0): re(re), im(im) {}

  double magnitudeSq() const { return re * re + im * im; }
  double magnitude() const { return std::sqrt(re * re + im * im); }

  Complex power(float exponent) const
  {
    float r = sqrt(re*re + im*im);
    float angle = atan2(im, re);
    float newR = pow(r, exponent);
    float newAngle = angle * exponent;
    return Complex(newR*cos(newAngle), newR*sin(newAngle));
  }

  Complex inverse() const
  {
    double den = re*re + im*im;
    return Complex(re/den, -im/den);
  }

  Complex operator+(const Complex &other) const
  {
    return Complex(re + other.re, im + other.im);
  }
  Complex operator*(const Complex &other) const
  {
    return Complex(re * other.re - im * other.im, re * other.im + im * other.re);
  }
  Complex operator*(float o) const
  {
    return Complex(re * o , im * o);
  }
  
  double re;
  double im;
};