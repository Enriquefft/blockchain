#include <iostream>
#include <memory>
#include <type_traits>

// int chunksize = 3;

template <typename T, bool IsConst> struct DequeIterator {

  using value_type = std::conditional_t<IsConst, const T, T>;
  using pointer = value_type *;
  using reference = value_type &;
  using difference_type = ptrdiff_t;

  using map_pointer = pointer *;
  using iterator = DequeIterator;

  const difference_type CHUNK_SIZE = 512 / sizeof(T);

  pointer current;  // pointer to the chunk
  pointer first;    // begin of the chunk
  pointer last;     // end of the chunk
  map_pointer node; // pointer to the map

  void setNode(map_pointer newNode) { // salta a otro chunk
    node = newNode;
    first = *newNode;
    last = first + CHUNK_SIZE;
  }

  reference operator*() { return *current; }

  // ++iterator
  iterator &operator++() {
    ++current;
    if (current == last) {
      setNode(node + 1);
      current = first;
    }
    return *this;
  }

  // iterator++
  iterator operator++(int) {
    iterator tmp = *this;
    ++*this;
    return tmp;
  }

  // --iterator
  iterator &operator--() {
    if (current == first) {
      setNode(node - 1);
      current = last;
    }
    --current;
    return *this;
  }

  // iterator--
  iterator operator--(int) {
    iterator tmp = *this;
    --*this;
    return tmp;
  }

  iterator &operator+=(difference_type steps) {
    difference_type offset = steps + (current - first);
    if (offset >= 0 && offset < CHUNK_SIZE) {
      // same chunk
      current += steps;
      return *this;
    }

    difference_type node_offset = 0;

    // Compute chunks to move
    if (offset > 0) {
      node_offset = offset / CHUNK_SIZE;
    } else {
      node_offset = -((offset - 1) / CHUNK_SIZE) - 1;
    }
    setNode(node + node_offset); // goto new chunk

    current = first + (offset - node_offset * CHUNK_SIZE);

    return *this;
  }

  iterator operator+(difference_type steps) const {
    iterator temp = *this;
    return temp += steps;
  }
  iterator &operator-=(difference_type steps) const {
    iterator tmp = *this;
    return tmp += steps;
  }
  iterator operator-(difference_type steps) const {
    iterator tmp = *this;
    return tmp -= steps;
  }

  reference operator[](difference_type n) { return *(*this + n); }

  bool operator!=(const iterator &other) {
    return this->current != other.current;
  }
};

template <typename T> class Deque {

public:
  // typedefs
  using value_type = T;
  using size_type = size_t;
  using reference = T &;
  using const_reference = const T &;
  using pointer = T *;
  using const_pointer = const T *;
  using iterator = DequeIterator<T, false>;
  using const_iterator = DequeIterator<T, true>;
  using map_pointer = pointer *;

  // Constructors
  constexpr Deque();

  explicit Deque(size_t numElements) { fillInitialize(numElements, T()); }

  Deque(size_t num_elements, const T &value) {
    fillInitialize(num_elements, value);
  }
  T &front() { return *start; }
  T &back() {
    iterator tmp = finish;
    --tmp;
    return *tmp;
  }
  T &operator[](size_t n) { return start[n]; }
  void push_back(const T &value) {
    *(finish.current) = value;
    ++finish.current;
  }
  void push_front(const T &value) {
    *(start.current) = value;
    --start.current;
  }
  T &set_front() {
    iterator tmp = start;
    ++tmp;
    return *tmp;
  }
  void test() { --start.current; }
  iterator begin() { return start; }
  iterator end() { return finish; }

private:
  map_pointer map;
  size_type map_size;

  iterator start;
  iterator finish;

  void createMapAndNodes(size_t numElements) {

    size_t num_nodes = numElements / CHUNK_SIZE;
    if (numElements % CHUNK_SIZE != 0) {
      ++num_nodes;
    }
    map_size = std::max(8, (num_nodes + 2));
    mapAllocator alloc;
    map = alloc.allocate(map_size);

    T **tmp_start = map + (map_size - num_nodes) / 2;
    T **tmp_finish = tmp_start + num_nodes - 1;
    T **cur;
    dataAllocator alloc1;
    for (T **current = tmp_start; current <= tmp_finish; ++current) {
      *current = alloc1.allocate(CHUNK_SIZE);
    }

    start.set_node(tmp_start);
    start.current = start.first;

    finish.set_node(tmp_finish);
    finish.current = finish.first + numElements % CHUNK_SIZE;
  }

  void fillInitialize(size_type size, const_reference value);
  //   createMapAndNodes(n);
  //
  //   T *cur;
  //   for (cur = start.current; cur != finish.current; ++cur) {
  //     *cur = value;
  //   }
  // }
};
