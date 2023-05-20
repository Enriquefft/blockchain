#include <concepts>
#include <iostream>

template <typename T>
concept is_int = std::is_integral_v<T>;

template <is_int T> void F(T num) { std::cout << num << '\n'; }

int main() {
  F(1);
  return 0;
}
