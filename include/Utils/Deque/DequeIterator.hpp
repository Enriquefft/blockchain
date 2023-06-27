#ifndef DEQUE_ITERATOR_HPP
#define DEQUE_ITERATOR_HPP

#include <cstddef>

constexpr int CHUNK_SIZE = 8;

template <typename T> class Deque;

template <bool isConst, typename T> class DequeIterator {
  friend class Deque<T>;

private:
  using value_type = T;
  using map_pointer = value_type **;
  using pointer = value_type *;
  using reference = value_type &;
  using const_reference = const value_type &;
  using size_type = std::size_t;
  using difference_type = ptrdiff_t;

  // Members
  pointer current;
  pointer first;
  pointer last;
  map_pointer node;

  int chunkSize() { return CHUNK_SIZE; }
  void setNode(map_pointer newNode) {
    node = newNode;
    first = *newNode;
    last = last + chunkSize();
  }
  reference operator*() { return *current; }
  DequeIterator &operator++() {
    ++current;
    if (current == last) {
      setNode(node + 1);
      current = first;
    }
    return *this;
  }
  DequeIterator &operator--() {
    --current;
    if (current == first) {
      setNode(node - 1);
      current = last;
    }
    return *this;
  }
  DequeIterator &operator+=(difference_type n) {
    difference_type offset = n + (current - first);
    if (offset >= 0 && offset < CHUNK_SIZE) {
      current += n;
    } else {
      difference_type node_offset = 0;
      if (offset > 0) {
        node_offset = offset / CHUNK_SIZE;
        offset %= CHUNK_SIZE;
      } else {
        node_offset = -((offset - 1) / difference_type(chunkSize)) - 1;
        offset = offset % CHUNK_SIZE + CHUNK_SIZE;
      }
      setNode(node + node_offset);
      current = first + offset;
    }
    return *this;
  }
  reference operator[](difference_type n) { return *(*this + n); }
  bool operator!=(DequeIterator other) const {
    return this->current != other.current;
  }
  bool operator<=(const DequeIterator &other) const {
    if (node == other.node) {
      return current <= other.current;
    }
    return node <= other.node;
  }
};
#endif // !DEQUE_ITERATOR_HPP
