#pragma once
#include <compare>
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
   * perform operations with a Fraction are either an integer of size smaller
   * than 64 bits or Fraction.
   *
   */
  class Fraction {
    int64_t num;
    int64_t den;

  private:
    /**
     * @brief Reduce the fraction
     */
    void reduce();

  public:
    Fraction();
    Fraction(int64_t whole, int64_t num, int64_t den);
    Fraction(int64_t num, int64_t den);

    /**
     * @brief Flatten the fraction into a double value.
     * @return Value held by the class, converted to a double.
     */
    double flatten() const { return (double)num / den; }

    /**
     * @brief Get whole part of the fraction
     */
    int64_t get_whole() const { return num / den; }

    /**
     * @brief Get the decimal part as a double
     */
    double get_decimal() const {
      return (double)(num - get_whole() * den) / den;
    }

    /**
     * @brief Get the decimal part as a numerator-denominator pair
     */
    std::pair<int64_t, int64_t> get_frac() const {
      return {num - get_whole() * den, den};
    }

    /**
     * @brief Calculate the inverse of the function
     */
    Fraction inverse() const;

    Fraction operator-() const;
    Fraction operator+(const Fraction &other) const;
    Fraction operator-(const Fraction &other) const;
    Fraction operator*(const Fraction &other) const;
    Fraction operator/(const Fraction &other) const;

    std::strong_ordering operator<=>(const Fraction &other) const;

    friend Fraction operator+(const int64_t &a, const Fraction &b);
    friend Fraction operator+(const Fraction &a, const int64_t &b);
    friend Fraction operator-(const int64_t &a, const Fraction &b);
    friend Fraction operator-(const Fraction &a, const int64_t &b);
    friend Fraction operator*(const int64_t &a, const Fraction &b);
    friend Fraction operator*(const Fraction &a, const int64_t &b);
    friend Fraction operator/(const int64_t &a, const Fraction &b);
    friend Fraction operator/(const Fraction &a, const int64_t &b);

    friend std::strong_ordering operator<=>(const int64_t &a,
                                            const Fraction &b);
    friend std::strong_ordering operator<=>(const Fraction &a,
                                            const int64_t &b);
  };


  Fraction operator+(const int64_t &a, const Fraction &b);
  Fraction operator+(const Fraction &a, const int64_t &b);
  Fraction operator-(const int64_t &a, const Fraction &b);
  Fraction operator-(const Fraction &a, const int64_t &b);
  Fraction operator*(const int64_t &a, const Fraction &b);
  Fraction operator*(const Fraction &a, const int64_t &b);
  Fraction operator/(const int64_t &a, const Fraction &b);
  Fraction operator/(const Fraction &a, const int64_t &b);


  std::strong_ordering operator<=>(const int64_t &a, const Fraction &b);
  std::strong_ordering operator<=>(const Fraction &a, const int64_t &b);

} // namespace aer
