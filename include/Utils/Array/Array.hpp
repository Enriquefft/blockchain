#ifndef ARRAY_HPP
#define ARRAY_HPP

#include <initializer_list>
#include <stdexcept>
#include <type_traits>
#include <utility>

namespace Utils {

template <typename T, std::size_t N> class Array {
  static_assert(N > 0, "Array size must be greater than 0");

public:
  // Typedefs
  using value_type = T;

  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;

  using reference = value_type &;
  using const_reference = const value_type &;
  using pointer = value_type *;
  using const_pointer = const value_type *;

  using iterator = pointer;
  using const_iterator = const_pointer;

  // Constructors
  // constexpr Array() = default;
  // constexpr explicit Array(const T &value);
  // constexpr Array(const std::initializer_list<T> &list);
  //
  // template <typename... Tps>
  //   requires(sizeof...(Tps) == N && (std::is_same_v<T, Tps> && ...))
  // constexpr explicit Array(Tps &&...values) : m_data{std::move(values)...} {}
  //
  // template <typename... Tps>
  //   requires(sizeof...(Tps) == N && (std::is_same_v<T, Tps> && ...))
  // constexpr explicit Array(const Tps &...values) : m_data{values...} {}
  //
  // template <std::input_iterator InputIt>
  // constexpr Array(InputIt first, InputIt last);

  // Element access
  constexpr reference at(const size_type &pos);
  [[nodiscard]] constexpr const_reference at(const size_type &pos) const;
  constexpr reference operator[](const size_type &pos);
  constexpr const_reference operator[](const size_type &pos) const;

  constexpr reference front();
  [[nodiscard]] constexpr const_reference front() const;

  constexpr reference back();
  [[nodiscard]] constexpr const_reference back() const;

  constexpr pointer data() noexcept;
  [[nodiscard]] constexpr const_pointer data() const noexcept;

  // Iterators
  iterator begin() noexcept;
  iterator end() noexcept;
  [[nodiscard]] const_iterator begin() const noexcept;
  [[nodiscard]] const_iterator end() const noexcept;

  // Capacity
  [[nodiscard]] constexpr bool empty() const noexcept;
  [[nodiscard]] constexpr size_type size() const noexcept;

  T m_data[N];
};

// template <typename T, std::size_t N>
// constexpr Array<T, N>::Array(const T &value) {
//   for (auto &item : m_data) {
//     item = value;
//   }
// }
//
// template <typename T, std::size_t N>
// constexpr Array<T, N>::Array(const std::initializer_list<T> &list) {
//   if (list.size() > N) {
//     throw std::runtime_error(
//         "initializer_list size is bigger than array  size");
//   }
//   for (auto data_it = 0; const T &elem : list) {
//     m_data[data_it++] = elem;
//   }
// }
//
// template <typename T, std::size_t N>
// template <std::input_iterator InputIt>
// constexpr Array<T, N>::Array(InputIt first, InputIt last) {
//   static_assert(
//       std::is_same_v<typename std::iterator_traits<InputIt>::value_type, T>,
//       "InputIt value type must be the same as Array value type");
//   static_assert(std::distance(first, last) <= N,
//                 "InputIt distance must be the same as Array size");
//
//   auto data_it = 0;
//   for (auto it = first; it != last; ++it) {
//     m_data[data_it++] = *it;
//   }
// }

// Element access
template <typename T, std::size_t N>
constexpr auto Array<T, N>::at(const size_type &pos) -> reference {
  if constexpr (pos >= N) {
    throw std::out_of_range("Array index out of range");
  }

  return m_data[pos];
}

template <typename T, std::size_t N>
[[nodiscard]] constexpr auto Array<T, N>::at(const size_type &pos) const
    -> const_reference {
  if constexpr (pos >= N) {
    throw std::out_of_range("Array index out of range");
  }

  return m_data[pos];
}

template <typename T, std::size_t N>
constexpr auto Array<T, N>::operator[](const size_type &pos) -> reference {
  return m_data[pos];
}

template <typename T, std::size_t N>
constexpr auto Array<T, N>::operator[](const size_type &pos) const
    -> const_reference {
  return m_data[pos];
}

template <typename T, std::size_t N>
constexpr auto Array<T, N>::front() -> reference {
  return m_data[0];
}
template <typename T, std::size_t N>
[[nodiscard]] constexpr auto Array<T, N>::front() const -> const_reference {
  return m_data[0];
}

template <typename T, std::size_t N>
constexpr auto Array<T, N>::back() -> reference {
  return m_data[N - 1];
}
template <typename T, std::size_t N>
[[nodiscard]] constexpr auto Array<T, N>::back() const -> const_reference {
  return m_data[N - 1];
}

template <typename T, std::size_t N>
constexpr auto Array<T, N>::data() noexcept -> pointer {
  return m_data;
}
template <typename T, std::size_t N>
constexpr auto Array<T, N>::data() const noexcept -> const_pointer {
  return m_data;
}

// Iterators
template <typename T, std::size_t N>
auto Array<T, N>::begin() noexcept -> iterator {
  return m_data;
}
template <typename T, std::size_t N>
auto Array<T, N>::end() noexcept -> iterator {
  return m_data + N;
}
template <typename T, std::size_t N>
auto Array<T, N>::begin() const noexcept -> const_iterator {
  return m_data;
}
template <typename T, std::size_t N>
auto Array<T, N>::end() const noexcept -> const_iterator {
  return m_data + N;
}

// Capacity
template <typename T, std::size_t N>
constexpr bool Array<T, N>::empty() const noexcept {
  return N == 0;
}
template <typename T, std::size_t N>
constexpr auto Array<T, N>::size() const noexcept -> size_type {
  return N;
}
template <class T, class... U> Array(T, U...) -> Array<T, 1 + sizeof...(U)>;

} // namespace Utils

#endif // !ARRAY_HPP
