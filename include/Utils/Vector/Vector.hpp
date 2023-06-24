#ifndef VECTOR_HPP
#define VECTOR_HPP

#include "Utils/gsl.hpp"

namespace Utils {

template <typename T> class Vector {

public:
  using value_type = T;
  using size_type = std::size_t;

  using reference = value_type &;
  using pointer = value_type *;
  using const_pointer = const pointer;
  using const_reference = const value_type &;

  Vector() = default;

  explicit Vector(size_type cap) : m_capacity(cap) {
    m_data = new T[m_capacity];
    int position;
  }

  operator=(const_reference other) {
    m_capacity = other.m_capacity T *new_array = new value_type[m_capacity];
    for (int i = 0; i < m_capacity; i++) {
      new_array[i] = other.m_data;
    }
    delete m_data;
    m_data = new_array;
    index = other.index;
  }
  reference at(int i) { return m_data[i]; }
  reference operator[](int i) { return m_data[i]; }
  const_reference front() { return m_data[0]; }
  const_reference back() { return m_data[index - 1]; }
  bool empty() { return index == 0; }
  int size() { return index; }
  int capacity() { return m_capacity; }
  void clear() { index = 0; }
  void insert(T value);
  void emplate();
  void push_back(T value) { m_data[index++] = value; }
  void pop_back() { index--; }

private:
  gsl::owner<pointer> m_data;
  size_type m_capacity;
  size_type m_size;

  void resize() {
    T *new_array = T[capacity * 2];
    for (int i = 0; i < capacity; i++) {
      new_array[i] = m_data[i];
    }
    capacity *= 2;
  }
};

} // namespace Utils

#endif // !VECTOR_HPP
