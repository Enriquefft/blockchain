#ifndef ARRAY_HPP
#define ARRAY_HPP

#include <array>
#include <initializer_list>
#include <type_traits>

namespace Utils {

template <typename T, std::size_t N> using Array = std::array<T, N>;

template <typename T> class Array_ {
public:
  template <bool IS_CONST> class _iterator;

  // Typedefs
  using value_type = T;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using reference = value_type &;
  using const_reference = const value_type &;
  using pointer = value_type *;
  using const_pointer = const value_type *;
  using iterator = _iterator<false>;
  using const_iterator = _iterator<true>;

  // Constructors
  constexpr Array_();
  constexpr explicit Array_(const size_type &size);
  constexpr Array_(const size_type &size, const T &value);
  constexpr Array_(const std::initializer_list<T> &list);

  constexpr Array_(const Array_ &other);
  constexpr Array_(Array_ &&other) noexcept;
  constexpr Array_ &operator=(const Array_ &other);
  constexpr Array_ &operator=(Array_ &&other) noexcept;

  // Destructor
  ~Array_();

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
  [[nodiscard]] constexpr size_type maxSize() const noexcept;

  // Random access iterator
  template <bool IS_CONST> class _iterator {
  public:
    using value_type = std::conditional_t<IS_CONST, const T, T>;
    using difference_type = std::ptrdiff_t;
    using pointer = std::conditional_t<IS_CONST, const T *, T *>;
    using reference = std::conditional_t<IS_CONST, const T &, T &>;
  };

private:
  T *m_data;
  size_type m_size;
};

// Declaración explicita
extern template class Array_<int>;
extern template class Array_<float>;

} // namespace Utils

#endif // !ARRAY_HPP
