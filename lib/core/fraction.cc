#pragma once
#include "core/fraction.hh"
#include <cassert>
#include <numeric>


void aer::Fraction::reduce() {
  uint32_t virtual_whole = num / den;
  whole += virtual_whole;
  num -= virtual_whole * den;

  uint32_t gcd = std::gcd(num, den);
  if (gcd > 1) {
    num /= gcd;
    den /= gcd;
  }
}


aer::Fraction::Fraction(uint32_t whole, uint32_t num, uint32_t den)
    : whole(whole)
    , num(num)
    , den(den) {
  assert(den != 0);
  reduce();
}


aer::Fraction::Fraction(uint32_t num, uint32_t den)
    : whole(0)
    , num(num)
    , den(den) {
  assert(den != 0);
  reduce();
}


aer::Fraction aer::Fraction::inverse() const {
  assert(num + whole != 0);
  return Fraction(den, whole * den + num);
}


// Fraction/Fraction operators


aer::Fraction aer::Fraction::operator-() const {
  return Fraction(-whole, -num, den);
}


aer::Fraction aer::Fraction::operator+(const Fraction &other) const {
  return Fraction(whole + other.whole, num * other.den + other.num * den,
                  den * other.den);
}


aer::Fraction aer::Fraction::operator-(const Fraction &other) const {
  return Fraction(whole - other.whole, num * other.den - other.num * den,
                  den * other.den);
}


aer::Fraction aer::Fraction::operator*(const Fraction &other) const {
  uint32_t true_num = whole * den + num;
  uint32_t other_true_num = other.whole * other.den + other.num;
  return Fraction(true_num * other_true_num, den, other.den);
}


aer::Fraction aer::Fraction::operator/(const Fraction &other) const {
  assert(other.num + other.whole != 0);
  return *this * other.inverse();
}


// uint32_t/Fraction operators


aer::Fraction aer::operator+(const int32_t &a, const aer::Fraction &b) {
  return Fraction(b.whole + a, b.num, b.den);
}


aer::Fraction aer::operator+(const aer::Fraction &a, const int32_t &b) {
  return Fraction(a.whole + b, a.num, a.den);
};


aer::Fraction aer::operator-(const int32_t &a, const aer::Fraction &b) {
  return Fraction(b.whole - a, b.num, b.den);
}


aer::Fraction aer::operator-(const aer::Fraction &a, const int32_t &b) {
  return Fraction(a.whole - b, a.num, a.den);
};


aer::Fraction aer::operator*(const int32_t &a, const aer::Fraction &b) {
  return Fraction(a * b.whole, a * b.num, b.den);
};


aer::Fraction aer::operator*(const aer::Fraction &a, const int32_t &b) {
  return Fraction(b * a.whole, b * a.num, a.den);
}


aer::Fraction aer::operator/(const int32_t &a, const aer::Fraction &b) {
  assert(b.num + b.whole != 0);
  return a * b.inverse();
}


aer::Fraction aer::operator/(const aer::Fraction &a, const int32_t &b) {
  assert(b != 0);
  return a * Fraction(1, b);
}
