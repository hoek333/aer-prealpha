#include "core/fraction.hh"
#include <cassert>
#include <numeric>


void aer::Fraction::reduce() {
  // reduce fractional part
  int32_t gcd = std::gcd(num, den);
  num /= gcd;
  den /= gcd;
}


aer::Fraction::Fraction()
    : num(0)
    , den(1) {}


aer::Fraction::Fraction(int64_t whole, int64_t num, int64_t den)
    : num(whole * den + num)
    , den(den) {
  assert(den != 0);
  if (den < 0) {
    den = -den;
    num = -num;
  }
  reduce();
}


aer::Fraction::Fraction(int64_t num, int64_t den)
    : num(num)
    , den(den) {
  assert(den != 0);
  if (den < 0) {
    den = -den;
    num = -num;
  }
  reduce();
}


aer::Fraction aer::Fraction::inverse() const {
  assert(num != 0);
  return Fraction(den, num);
}


/* MEMBER OPERATORS ***********************************************************/


aer::Fraction aer::Fraction::operator-() const { return Fraction(-num, den); }


aer::Fraction aer::Fraction::operator+(const Fraction &other) const {
  return Fraction(num * other.den + other.num * den, den * other.den);
}


aer::Fraction aer::Fraction::operator-(const Fraction &other) const {
  return Fraction(num * other.den - other.num * den, den * other.den);
}


aer::Fraction aer::Fraction::operator*(const Fraction &other) const {
  return Fraction(num * other.num, den * other.den);
}


aer::Fraction aer::Fraction::operator/(const Fraction &other) const {
  assert(other.num != 0);
  return Fraction(num * other.den, den * other.num);
}


std::strong_ordering aer::Fraction::operator<=>(const Fraction &other) const {
  return num * other.den <=> other.num * den;
}


/* FRIEND OPERATORS ***********************************************************/


aer::Fraction aer::operator+(const int64_t &a, const Fraction &b) {
  return Fraction(b.num + a * b.den, b.den);
}


aer::Fraction aer::operator+(const Fraction &a, const int64_t &b) {
  return Fraction(a.num + b * a.den, a.den);
}

aer::Fraction aer::operator-(const int64_t &a, const Fraction &b) {
  return Fraction(a * b.den - b.num, b.den);
}

aer::Fraction aer::operator-(const Fraction &a, const int64_t &b) {
  return Fraction(a.num - b * a.den, a.den);
}


aer::Fraction aer::operator*(const int64_t &a, const Fraction &b) {
  return Fraction(a * b.num, b.den);
}


aer::Fraction aer::operator*(const Fraction &a, const int64_t &b) {
  return Fraction(a.num * b, a.den);
}


aer::Fraction aer::operator/(const int64_t &a, const Fraction &b) {
  return Fraction(a, 1) / b;
}


aer::Fraction aer::operator/(const Fraction &a, const int64_t &b) {
  return a / Fraction(b, 1);
}


std::strong_ordering aer::operator<=>(const int64_t &a, const Fraction &b) {
  return Fraction(a, 1) <=> b;
}

std::strong_ordering aer::operator<=>(const Fraction &a, const int64_t &b) {
  return a <=> Fraction(b, 1);
}
