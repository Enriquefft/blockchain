#ifndef DEQUE_HPP
#define DEQUE_HPP

#include <cstddef>

template <typename T> class Deque {
public:
  template <bool IsConst> class Iterator;

  using value_type = T;
  using size_type = std::size_t;
  using reference = T &;
  using const_reference = const T &;
  using pointer = T *;
  using const_pointer = const T *;

  using iterator = Iterator<false>;
  using const_iterator = Iterator<true>;

  // Constructors
  Deque() noexcept = default;
  explicit Deque(size_type count, const_reference value = T());

  // Accessors
  reference at(size_type pos);
  const_reference at(size_type pos) const;

  reference operator[](size_type pos);
  const_reference operator[](size_type pos) const;

  reference front();
  const_reference front() const;

  reference back();
  const_reference back() const;

  // Iterators
  iterator begin() noexcept;
  const_iterator begin() const noexcept;
  iterator end() noexcept;
  const_iterator end() const noexcept;

  // Capacity
  [[nodiscard]] bool empty() const noexcept;
  [[nodiscard]] size_type size() const noexcept;
  [[nodiscard]] size_type max_size() const noexcept;
  void shrink_to_fit();

  // Modifiers
  void clear() noexcept;
  /*insert*/
  template <typename... Args> reference emplace_front(Args &&...args);
};

#endif // !DEQUE_HPP
