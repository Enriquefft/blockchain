#ifndef VECTOR_HPP
#define VECTOR_HPP

#include "gsl.hpp"
#include <Array/Array.hpp>
#include <bits/iterator_concepts.h>
#include <cstddef>
#include <initializer_list>
#include <stdexcept>

#include <iostream>

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
  constexpr explicit Vector(size_type count);
  constexpr Vector(size_type count, const T &value);

  template <std::input_iterator InputIt>
  constexpr Vector(InputIt first, InputIt last);

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

  // Iterators
  constexpr iterator begin() noexcept;
  constexpr const_iterator begin() const noexcept;
  constexpr const_iterator cbegin() const noexcept;

  constexpr iterator end() noexcept;
  constexpr const_iterator end() const noexcept;
  constexpr const_iterator cend() const noexcept;

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
  template <std::input_iterator InputIt>
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

  constexpr void resize(size_type newSize);
  constexpr void resize(size_type count, const value_type &value);

private:
  size_type m_size = 0;
  size_type m_capacity = 0;
  gsl::owner<pointer> m_data = nullptr;

  [[nodiscard]] constexpr static size_type
  nextCapacity(const size_type &newCap) noexcept;

  inline static constexpr Array<size_type, 24> POWERS = {
      1,     2,      4,      8,      16,      32,      64,      128,
      256,   512,    1024,   2048,   4096,    8192,    16384,   32768,
      65536, 131072, 262144, 524288, 1048576, 2097152, 4194304, 8388608};

  template <bool isConst> class v_iterator {
  public:
    explicit constexpr v_iterator(pointer ptr = nullptr) noexcept
        : m_ptr{ptr} {}

    // Implicit conversion from iterator to const_iterator
    template <bool otherConst>
    constexpr operator v_iterator<otherConst>() const noexcept {
      return v_iterator<otherConst>{m_ptr};
    }

    // Accessors
    constexpr reference operator*() const noexcept { return *m_ptr; }
    constexpr pointer operator->() const noexcept { return m_ptr; }

    // Traversal
    constexpr v_iterator &operator++() noexcept {
      ++m_ptr;
      return *this;
    }
    constexpr v_iterator operator++(int) noexcept {
      v_iterator tmp = *this;
      ++(*this);
      return tmp;
    }

    // Comparison
    constexpr bool operator==(const v_iterator &other) const noexcept {
      return m_ptr == other.m_ptr;
    }
    constexpr bool operator!=(const v_iterator &other) const noexcept {
      return !(*this == other);
    }

  private:
    pointer m_ptr = nullptr;
  };
};

template <typename T>
template <std::input_iterator InputIt>
constexpr auto Vector<T>::insert(const_iterator pos, InputIt first,
                                 InputIt last) -> iterator {
  size_type index = std::distance(cbegin(), pos);
  size_type num_new_elements = std::distance(first, last);

  if (m_size + num_new_elements > m_capacity) {
    reserve(nextCapacity(m_size + num_new_elements));
  }

  // Move the existing elements from the insertion point onwards to the right
  std::move_backward(m_data + index, m_data + m_size,
                     m_data + m_size + num_new_elements);

  std::copy(first, last, m_data + index);

  m_size += num_new_elements;
  return m_data + index;
}

template <typename T>
template <typename... Args>
constexpr auto Vector<T>::emplace(const_iterator pos, Args &&...args)
    -> iterator {

  size_type index = std::distance(cbegin(), pos);

  if (m_size + 1 > m_capacity) {
    reserve(nextCapacity(m_size + 1));
  }

  // If there are elements after position, move them to make room for the new
  // element
  if (index < m_size) {
    std::move_backward(m_data + index, m_data + m_size, m_data + m_size + 1);
  }

  new (m_data + index) T(std::forward<Args>(args)...);

  ++m_size;

  return iterator(m_data + index);
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
constexpr Vector<T>::Vector(size_type count, const T &value)
    : m_size{count}, m_capacity{nextCapacity(m_size)}, m_data{
                                                           new T[m_capacity]} {
  for (size_type i = 0; i < count; ++i) {
    m_data[i] = value;
  }
}

template <typename T>
template <std::input_iterator InputIt>
constexpr Vector<T>::Vector(InputIt first, InputIt last)
    : m_size{std::distance(first, last)},
      m_capacity{nextCapacity(m_size)}, m_data{new T[m_capacity]} {
  for (size_type i = 0; i < m_size; ++i) {
    m_data[i] = *first;
    ++first;
  }
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

template <typename T> constexpr Vector<T>::~Vector() { delete[] m_data; }

template <typename T>
constexpr Vector<T>::Vector(const Vector &other)
    : m_size{other.m_size},
      m_capacity{other.m_capacity}, m_data{new T[m_capacity]} {
  for (size_type i = 0; i < m_size; ++i) {
    m_data[i] = other.m_data[i];
  }
}

template <typename T>
constexpr Vector<T> &Vector<T>::operator=(const Vector &other) {
  if (this == &other) {
    return *this;
  }
  delete[] m_data;
  m_size = other.m_size;
  m_capacity = other.m_capacity;
  m_data = new T[m_capacity];
  for (size_type i = 0; i < m_size; ++i) {
    m_data[i] = other.m_data[i];
  }
  return *this;
}

template <typename T>
constexpr Vector<T>::Vector(Vector &&other) noexcept
    : m_size(other.m_size), m_capacity(other.m_capacity), m_data(other.m_data) {
  other.m_size = 0;
  other.m_capacity = 0;
  other.m_data = nullptr;
}

template <typename T>
constexpr Vector<T> &Vector<T>::operator=(Vector &&other) noexcept {
  m_size = other.m_size;
  m_capacity = other.m_capacity;
  m_data = other.m_data;
  other.m_size = 0;
  other.m_capacity = 0;
  other.m_data = nullptr;
}

// // Element access
//
template <typename T> constexpr auto Vector<T>::at(size_type pos) -> reference {
  if (pos >= m_size) {
    throw std::out_of_range("Vector::at");
  }

  return m_data[pos];
}
template <typename T>
constexpr auto Vector<T>::at(size_type pos) const -> const_reference {
  if (pos >= m_size) {
    throw std::out_of_range("Vector::at");
  }

  return m_data[pos];
}

template <typename T>
constexpr auto Vector<T>::operator[](size_type pos) -> reference {
  return m_data[pos];
}
template <typename T>
constexpr auto Vector<T>::operator[](size_type pos) const -> const_reference {
  return m_data[pos];
}

template <typename T> constexpr auto Vector<T>::front() -> reference {
  return m_data[0];
}
template <typename T>
constexpr auto Vector<T>::front() const -> const_reference {
  return m_data[0];
}

template <typename T> constexpr auto Vector<T>::back() -> reference {
  return m_data[m_size - 1];
}
template <typename T>
constexpr auto Vector<T>::back() const -> const_reference {
  return m_data[m_size - 1];
}

template <typename T> constexpr T *Vector<T>::data() noexcept { return m_data; }
template <typename T> constexpr const T *Vector<T>::data() const noexcept {
  return m_data;
}

// // Capacity
//
template <typename T> constexpr bool Vector<T>::empty() const noexcept {
  return m_size == 0;
}
template <typename T>
constexpr auto Vector<T>::size() const noexcept -> size_type {
  return m_size;
}

template <typename T> constexpr void Vector<T>::reserve(size_type newCap) {

  if (newCap <= m_capacity) {
    return;
  }

  gsl::owner<T *> new_data = new T[newCap];
  std::move(begin(), end(), new_data);
  delete[] m_data;
  m_data = new_data;
  m_capacity = newCap;
}

template <typename T> constexpr void Vector<T>::shrinkToFit() {

  if (m_size == m_capacity) {
    return;
  }

  gsl::owner<T *> new_data = new T[m_size];
  std::move(begin(), end(), new_data);
  delete[] m_data;
  m_data = new_data;
  m_capacity = m_size;
}
template <typename T>
constexpr auto Vector<T>::capacity() const noexcept -> size_type {
  return m_capacity;
}

// // Modifiers
template <typename T> constexpr void Vector<T>::clear() noexcept {

  for (size_type i = 0; i < m_size; ++i) {
    m_data[i].~T();
  }
  m_size = 0;
}

template <typename T>
constexpr auto Vector<T>::insert(const_iterator pos, const T &value)
    -> iterator {
  size_type index = std::distance(cbegin(), pos);

  if (index > size()) {
    throw std::out_of_range("Insert position is out of range");
  }
  if (m_size == m_capacity) {
    reserve(nextCapacity(m_capacity + 1));
  }
  std::move_backward(pos, end(), end() + 1);
  *pos = value;
  m_size++;
  return begin() + index;
}

template <typename T>
constexpr auto Vector<T>::insert(const_iterator pos, T &&value) -> iterator {

  size_type index = std::distance(cbegin(), pos);

  if (index > size()) {
    throw std::out_of_range("Insert position is out of range");
  }
  if (m_size == m_capacity) {
    reserve(nextCapacity(m_capacity + 1));
  }

  std::move_backward(m_data + index, m_data + m_size, m_data + m_size + 1);

  m_data[index] = std::forward<T>(value);

  ++m_size;

  return iterator(m_data + index);
}

template <typename T>
constexpr auto Vector<T>::insert(const_iterator pos, size_type count,
                                 const T &value) -> iterator {

  size_type index = std::distance(cbegin(), pos);

  if (index > size()) {
    throw std::out_of_range("Insert position is out of range");
  }
  if (m_size == m_capacity) {
    reserve(nextCapacity(m_capacity + count));
  }
  std::move_backward(m_data + index, m_data + m_size, m_data + m_size + count);
  std::fill(m_data + index, m_data + index + count, value);

  m_size += count;

  return m_data + index;
}

template <typename T>
constexpr auto Vector<T>::insert(const_iterator pos,
                                 std::initializer_list<T> ilist) -> iterator {
  return insert(pos, ilist.begin(), ilist.end());
}

template <typename T>
constexpr auto Vector<T>::erase(const_iterator pos) -> iterator {
  size_type index = std::distance(cbegin(), pos);

  if (index >= size()) {
    throw std::out_of_range("Erase position is out of range");
  }
  std::move(begin() + index + 1, end(), begin() + index);
  --m_size;

  return begin() + index;
}

template <typename T>
constexpr auto Vector<T>::erase(const_iterator first, const_iterator last)
    -> iterator {
  size_type index_first = std::distance(cbegin(), first);
  size_type index_last = std::distance(cbegin(), last);

  if (index_first > size() || index_last > size()) {
    throw std::out_of_range("Erase range is out of range");
  }

  std::move(m_data + index_last, m_data + m_size, m_data + index_first);

  size_type new_size = m_size - (index_last - index_first);

  // Destroy the leftover elements
  for (size_type i = new_size; i < m_size; ++i) {
    m_data[i].~T();
  }

  m_size = new_size;

  return iterator(m_data + index_first);
}

template <typename T> constexpr void Vector<T>::pushBack(const T &value) {

  if (m_size == m_capacity) {
    reserve(nextCapacity(m_capacity + 1));
  }

  m_data[m_size++] = value;
}
template <typename T> constexpr void Vector<T>::pushBack(T &&value) {
  if (m_size == m_capacity) {
    reserve(nextCapacity(m_capacity + 1));
  }
  m_data[m_size++] = std::move(value);
}

template <typename T> constexpr void Vector<T>::popBack() {
  // Call the destructor for the last element
  if (m_size == 0) {
    throw std::out_of_range("Vector is empty");
  }
  m_data[--m_size].~T();
}

template <typename T>
template <class... Args>
constexpr auto Vector<T>::emplaceBack(Args &&...args) -> reference {
  if (m_size == m_capacity) {
    reserve(nextCapacity(m_capacity + 1));
  }
  m_data[m_size++] = T(std::forward<Args>(args)...);
  return m_data[m_size - 1];
}

template <typename T> constexpr void Vector<T>::resize(size_type newSize) {
  if (newSize < m_size) {
    // If the new size is smaller, destroy the extra elements
    for (size_type i = newSize; i < m_size; ++i) {
      m_data[i].~T();
    }
  } else if (newSize > m_size) {
    // If the new size is larger, check if the vector needs to be resized
    if (newSize > m_capacity) {
      reserve(nextCapacity(newSize));
    }

    for (size_type i = m_size; i < newSize; ++i) {
      m_data[i] = T();
    }
  }

  m_size = newSize;
}

template <typename T> constexpr auto Vector<T>::begin() noexcept -> iterator {
  return iterator(m_data);
}
template <typename T>
constexpr auto Vector<T>::begin() const noexcept -> const_iterator {
  return const_iterator(m_data);
}
template <typename T>
constexpr auto Vector<T>::cbegin() const noexcept -> const_iterator {
  return const_iterator(m_data);
}
template <typename T> constexpr auto Vector<T>::end() noexcept -> iterator {
  return iterator();
}
template <typename T>
constexpr auto Vector<T>::end() const noexcept -> const_iterator {
  return iterator();
}
template <typename T>
constexpr auto Vector<T>::cend() const noexcept -> const_iterator {
  return iterator();
}

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

#endif // !VECTOR_HPP
