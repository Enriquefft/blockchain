#include "Utils/Vector/Vector.hpp"
#include <cmath>

// DEBUG
#include <iostream>

namespace Utils {

// private:
//   gsl::owner<pointer> m_data = nullptr;
//   size_type m_capacity = 0;
//   size_type m_size = 0;
// };

template <typename T>
constexpr Vector<T>::Vector(size_type count, const T &value)
    : m_size{count}, m_capacity{nextCapacity(m_size)}, m_data{
                                                           new T[m_capacity]} {
  for (size_type i = 0; i < count; ++i) {
    m_data[i] = value;
  }
}

template <typename T>
constexpr Vector<T>::Vector(size_type count)
    : m_size{count}, m_capacity{nextCapacity(m_size)}, m_data{
                                                           new T[m_capacity]} {
  std::cout << "Vector(size_type count)\n";
  for (size_type i = 0; i < count; ++i) {
    std::cout << "i: " << i << "\n";
  }
  std::cout << "Vector(size_type count) end\n\n";
}

template <typename T>
constexpr Vector<T>::Vector(std::initializer_list<T> init)
    : m_size{init.size()},
      m_capacity{nextCapacity(m_size)}, m_data{new T[m_capacity]} {
  std::cout << "Vector(std::initializer_list<T> init)\n";
  size_type v_idx = 0;
  for (const auto &elem : init) {
    m_data[v_idx] = elem;
    std::cout << "i: " << v_idx << "\n";
    ++v_idx;
  }
  std::cout << "Vector(std::initializer_list<T> init) end\n\n";
}

template <typename T> constexpr Vector<T>::~Vector() {}

// template <typename T> constexpr Vector<T>::Vector(const Vector &other) {}
// template <typename T> constexpr Vector<T>::Vector(Vector &&other) noexcept {}
// template <typename T>
// constexpr Vector<T> &Vector<T>::operator=(const Vector &other) {}
//
// template <typename T>
// constexpr Vector<T> &Vector<T>::operator=(Vector &&other) noexcept {}
//
// // Element access
//
// template <typename T>
// constexpr auto Vector<T>::at(size_type pos) -> reference {}
// template <typename T>
// constexpr auto Vector<T>::at(size_type pos) const -> const_reference {}
//
// template <typename T>
// constexpr auto Vector<T>::operator[](size_type pos) -> reference {}
// template <typename T>
// constexpr auto Vector<T>::operator[](size_type pos) const -> const_reference
// {}
//
// template <typename T> constexpr auto Vector<T>::front() -> reference {}
// template <typename T>
// constexpr auto Vector<T>::front() const -> const_reference {}
//
// template <typename T> constexpr auto Vector<T>::back() -> reference {}
// template <typename T>
// constexpr auto Vector<T>::back() const -> const_reference {}
//
// template <typename T> constexpr T *Vector<T>::data() noexcept {}
// template <typename T> constexpr const T *Vector<T>::data() const noexcept {}
//
// // Capacity
//
// template <typename T> constexpr bool Vector<T>::empty() const noexcept {}
// template <typename T>
// constexpr auto Vector<T>::size() const noexcept -> size_type {}
//
// template <typename T> constexpr void Vector<T>::reserve(size_type newCap) {}
// template <typename T> constexpr void Vector<T>::shrinkToFit() {}
// template <typename T>
// constexpr auto Vector<T>::capacity() const noexcept -> size_type {}
//
// // Modifiers
// template <typename T> constexpr void Vector<T>::clear() noexcept {}
// template <typename T>
// constexpr auto Vector<T>::insert(const_iterator pos, const T &value)
//     -> iterator {}
// template <typename T>
// constexpr auto Vector<T>::insert(const_iterator pos, T &&value) -> iterator
// {} template <typename T> constexpr auto Vector<T>::insert(const_iterator pos,
// size_type count,
//                                  const T &value) -> iterator {}
//
// template <typename T>
// constexpr iterator Vector<T>::insert(const_iterator pos,
//                                      std::initializer_list<T> ilist) {}
//
// template <typename T> constexpr iterator Vector<T>::erase(const_iterator pos)
// {} template <typename T> constexpr iterator Vector<T>::erase(const_iterator
// first, const_iterator last) {
// }
//
// template <typename T> constexpr void Vector<T>::pushBack(const T &value) {}
// template <typename T> constexpr void Vector<T>::pushBack(T &&value) {}
//
// template <typename T> constexpr void Vector<T>::popBack() {}
//
// template <typename T> constexpr void Vector<T>::resize(size_type count) {}
// template <typename T>
// constexpr void Vector<T>::resize(size_type count, const value_type &value) {}

template <typename T>
constexpr auto Vector<T>::nextCapacity(const size_type &newCap) noexcept
    -> size_type {

  size_t low = 0;
  size_t high = POWERS.size();

  while (low < high) {
    size_t mid = low + ((high - low) / 2);

    if (POWERS.at(mid) <= newCap) {
      low = mid + 1;
    } else {
      high = mid;
    }
  }

  std::cout << "nextCapacity: " << POWERS.at(low) << "\n";

  return POWERS.at(low);
}

} // namespace Utils
