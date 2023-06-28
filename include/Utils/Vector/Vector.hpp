#ifndef VECTOR_HPP
#define VECTOR_HPP

#include "Utils/gsl.hpp"
#include <cstddef>
#include <initializer_list>
#include <stdexcept>

// #include <Utils/Array/Array.hpp>
#include <array>

namespace Utils {

template <typename T> class Vector {

  static_assert(std::is_copy_constructible_v<T>,
                "T must be copy constructible");
  template <bool isConst> class v_iterator;
  template <bool isConst> friend class v_iterator;

public:
  // Typedefs
  using value_type = T;
  using size_type = size_t;
  using reference = value_type &;
  using pointer = value_type *;
  using const_pointer = const pointer;
  using const_reference = const value_type &;

  using iterator = v_iterator<false>;
  using const_iterator = v_iterator<true>;

  // Constructors
  constexpr Vector() noexcept = default;
  constexpr Vector(size_type count, const T &value);
  constexpr explicit Vector(size_type count);

  template <class InputIt> constexpr Vector(InputIt first, InputIt last);
  constexpr Vector(std::initializer_list<T> init);

  constexpr ~Vector();

  constexpr Vector(const Vector &other);
  constexpr Vector(Vector &&other) noexcept;
  constexpr Vector &operator=(const Vector &other);
  constexpr Vector &operator=(Vector &&other) noexcept;

  // Element access
  constexpr reference at(size_type pos);
  constexpr const_reference at(size_type pos) const;

  constexpr reference operator[](size_type pos);
  constexpr const_reference operator[](size_type pos) const;

  constexpr reference front();
  constexpr const_reference front() const;

  constexpr reference back();
  constexpr const_reference back() const;

  constexpr T *data() noexcept;
  constexpr const T *data() const noexcept;

  // Capacity
  [[nodiscard]] constexpr bool empty() const noexcept;
  [[nodiscard]] constexpr size_type size() const noexcept;
  constexpr void reserve(size_type newCap);
  constexpr void shrinkToFit();
  [[nodiscard]] constexpr size_type capacity() const noexcept;

  // Modifiers
  constexpr void clear() noexcept;
  constexpr iterator insert(const_iterator pos, const T &value);
  constexpr iterator insert(const_iterator pos, T &&value);
  constexpr iterator insert(const_iterator pos, size_type count,
                            const T &value);
  template <class InputIt>
  constexpr iterator insert(const_iterator pos, InputIt first, InputIt last);

  constexpr iterator insert(const_iterator pos, std::initializer_list<T> ilist);

  template <class... Args>
  constexpr iterator emplace(const_iterator pos, Args &&...args);
  constexpr iterator erase(const_iterator pos);
  constexpr iterator erase(const_iterator first, const_iterator last);

  constexpr void pushBack(const T &value);
  constexpr void pushBack(T &&value);

  template <class... Args> constexpr reference emplaceBack(Args &&...args);

  constexpr void popBack();

  constexpr void resize(size_type count);
  constexpr void resize(size_type count, const value_type &value);

private:
  size_type m_size = 0;
  size_type m_capacity = 0;
  gsl::owner<pointer> m_data = nullptr;

  [[nodiscard]] constexpr static size_type
  nextCapacity(const size_type &newCap) noexcept;

  inline static constexpr std::array<size_type, 24> POWERS = {
      1,     2,      4,      8,      16,      32,      64,      128,
      256,   512,    1024,   2048,   4096,    8192,    16384,   32768,
      65536, 131072, 262144, 524288, 1048576, 2097152, 4194304, 8388608};
};

template <typename T>
template <class InputIt>
constexpr Vector<T>::Vector(InputIt first, InputIt last) {}

template <typename T>
template <class InputIt>
constexpr auto Vector<T>::insert(const_iterator pos, InputIt first,
                                 InputIt last) -> iterator {}
template <typename T>
template <class... Args>
constexpr auto Vector<T>::emplace(const_iterator pos, Args &&...args)
    -> iterator {}
template <typename T>
template <class... Args>
constexpr auto Vector<T>::emplaceBack(Args &&...args) -> reference {}

} // namespace Utils

#endif // !VECTOR_HPP
