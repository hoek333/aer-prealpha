#pragma once
#include <cstdint>
#include <utility>
namespace aer {


  /**
   * @class Fraction
   * @brief Numerically stable rational number represented as a whole part and a
   * fractional part. This class exclusively uses 32-bit integers for internal
   * representation. The fractional part is automatically reduced on
   * construction as well as after each operation, so that abs(numerator) <
   * denominator. The denominator is always positive.
   *
   * @note To preserve numerical stability, the only types that can be used to
   * perform operations with a Fraction are either int32_t or Fraction.
   *
   */
  class Fraction {
    int32_t whole;
    int32_t num;
    uint16_t den;

  private:
    /**
     * @brief Reduce the fraction
     */
    void reduce();

  public:
    Fraction(uint32_t whole, uint32_t num, uint32_t den);
    Fraction(uint32_t num, uint32_t den);

    /**
     * @brief Flatten the fraction into a double value.
     * @return Value held by the class, converted to a double.
     */
    double flatten() const { return whole + (double)num / den; }

    /**
     * @brief Get whole part of the fraction
     */
    uint32_t get_whole() const { return whole; }

    /**
     * @brief Get the decimal part as a double
     */
    double get_decimal() const { return (double)num / den; };

    /**
     * @brief Get the decimal part as a numerator-denominator pair
     */
    std::pair<uint32_t, uint32_t> get_frac() const { return {num, den}; }

    /**
     * @brief Calculate the inverse of the function
     */
    Fraction inverse() const;

    Fraction operator-() const;
    Fraction operator+(const Fraction &other) const;
    Fraction operator-(const Fraction &other) const;
    Fraction operator*(const Fraction &other) const;
    Fraction operator/(const Fraction &other) const;

    friend Fraction operator+(const int32_t &a, const Fraction &b);
    friend Fraction operator+(const Fraction &a, const int32_t &b);
    friend Fraction operator-(const int32_t &a, const Fraction &b);
    friend Fraction operator-(const Fraction &a, const int32_t &b);
    friend Fraction operator*(const int32_t &a, const Fraction &b);
    friend Fraction operator*(const Fraction &a, const int32_t &b);
    friend Fraction operator/(const int32_t &a, const Fraction &b);
    friend Fraction operator/(const Fraction &a, const int32_t &b);
  };


} // namespace aer
