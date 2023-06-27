#include "Utils/Vector/Vector.hpp"
#include <cmath>

namespace Utils {

// private:
//   gsl::owner<pointer> m_data = nullptr;
//   size_type m_capacity = 0;
//   size_type m_size = 0;
// };

template <typename T>
constexpr Vector<T>::Vector(size_type count, const T &value) {}
template <typename T> constexpr Vector<T>::Vector(size_type count) {}

template <typename T>
constexpr Vector<T>::Vector(std::initializer_list<T> init) {}

template <typename T> constexpr Vector<T>::~Vector() {}

template <typename T> constexpr Vector<T>::Vector(const Vector &other) {}
template <typename T> constexpr Vector<T>::Vector(Vector &&other) noexcept {}
template <typename T>
constexpr Vector<T> &Vector<T>::operator=(const Vector &other) {}

template <typename T>
constexpr Vector<T> &Vector<T>::operator=(Vector &&other) noexcept {}

// Element access

template <typename T>
constexpr auto Vector<T>::at(size_type pos) -> reference {}
template <typename T>
constexpr auto Vector<T>::at(size_type pos) const -> const_reference {}

template <typename T>
constexpr auto Vector<T>::operator[](size_type pos) -> reference {}
template <typename T>
constexpr auto Vector<T>::operator[](size_type pos) const -> const_reference {}

template <typename T> constexpr auto Vector<T>::front() -> reference {}
template <typename T>
constexpr auto Vector<T>::front() const -> const_reference {}

template <typename T> constexpr auto Vector<T>::back() -> reference {}
template <typename T>
constexpr auto Vector<T>::back() const -> const_reference {}

template <typename T> constexpr T *Vector<T>::data() noexcept {}
template <typename T> constexpr const T *Vector<T>::data() const noexcept {}

// Capacity

template <typename T> constexpr bool Vector<T>::empty() const noexcept {}
template <typename T>
constexpr auto Vector<T>::size() const noexcept -> size_type {}

template <typename T> constexpr void Vector<T>::reserve(size_type newCap) {}
template <typename T> constexpr void Vector<T>::shrinkToFit() {}
template <typename T>
constexpr auto Vector<T>::capacity() const noexcept -> size_type {}

// Modifiers
template <typename T> constexpr void Vector<T>::clear() noexcept {}
template <typename T>
constexpr auto Vector<T>::insert(const_iterator pos, const T &value)
    -> iterator {}
template <typename T>
constexpr auto Vector<T>::insert(const_iterator pos, T &&value) -> iterator {}
template <typename T>
constexpr auto Vector<T>::insert(const_iterator pos, size_type count,
                                 const T &value) -> iterator {}

template <typename T>
constexpr iterator Vector<T>::insert(const_iterator pos,
                                     std::initializer_list<T> ilist) {}

template <typename T> constexpr iterator Vector<T>::erase(const_iterator pos) {}
template <typename T>
constexpr iterator Vector<T>::erase(const_iterator first, const_iterator last) {
}

template <typename T> constexpr void Vector<T>::pushBack(const T &value) {}
template <typename T> constexpr void Vector<T>::pushBack(T &&value) {}

template <typename T> constexpr void Vector<T>::popBack() {}

template <typename T> constexpr void Vector<T>::resize(size_type count) {}
template <typename T>
constexpr void Vector<T>::resize(size_type count, const value_type &value) {}

template <typename T>
constexpr auto Vector<T>::nextCapacity(const size_type &newCap) -> size_type {}

} // namespace Utils
