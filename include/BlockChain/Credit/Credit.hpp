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
  constexpr Credit(uint8_t _amount) : amount(_amount) {} // NOLINT
  // Allow implicit constructor

  auto operator<=>(const Credit &) const = default;

  template <is_number T> Credit &operator+(const T &val) {
    this->amount += val;
    return *this;
  }
  template <is_number T> Credit &operator+=(const T &val) {
    this->amount += val;
    return *this;
  }

  template <is_number T> Credit &operator-=(const T &val) {

    // Handle underflow
    if (static_cast<T>(this->amount) < val) {
      throw std::underflow_error("negative value");
    }
    this->amount -= val;
    return *this;
  }

  friend std::ostream &operator<<(std::ostream &ost, const Credit &credit) {
    ost << credit.amount;
    return ost;
  }

  explicit operator int() const { return this->amount; }

private:
  uint8_t amount;
};

#endif // !CREDIT_HPP
