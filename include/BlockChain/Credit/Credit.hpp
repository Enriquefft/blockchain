#ifndef CREDIT_HPP
#define CREDIT_HPP

#include <compare>
#include <concepts>
#include <cstdint>
#include <ostream>
#include <stdexcept>

template <typename T>
concept is_number = std::is_arithmetic_v<T>;

struct Credit {
  // We are working under the asumption that the credit amount wont overflow

public:
  constexpr Credit() = default;
  constexpr Credit(uint8_t _amount) : amount(_amount) {} // NOLINT
  // Allow implicit constructor

  constexpr auto operator<=>(const Credit &) const = default;

  template <is_number T> constexpr Credit &operator+(const T &val) {
    this->amount += val;
    return *this;
  }
  template <is_number T> constexpr Credit &operator+=(const T &val) {
    this->amount += val;
    return *this;
  }

  template <is_number T> constexpr Credit &operator-=(const T &val) {

    // Handle underflow
    if (static_cast<T>(this->amount) < val) {
      throw std::underflow_error("negative value");
    }
    this->amount -= val;
    return *this;
  }

  friend std::ostream &operator<<(std::ostream &ost, const Credit &credit) {
    ost << static_cast<int>(credit.amount);
    return ost;
  }

  constexpr explicit operator int() const { return this->amount; }

private:
  uint8_t amount = 0;
};

#endif // !CREDIT_HPP
