#ifndef FORWARD_LIST_HPP
#define FORWARD_LIST_HPP

#include <cstddef>

namespace Utils {

template <class T> class ForwardList {

public:
  // Typedefs
  using value_type = T;
  using size_type = size_t;
  using reference = T &;
  using const_reference = const T &;

  using pointer = T *;
  using const_pointer = const T *;

private:
  struct Node {
    value_type data;
    Node *next;
  };
  Node *head;
  Node *tail;

public:
  // Constructors
  ForwardList();
  explicit ForwardList(size_type count);
  ForwardList(size_type count, const T &value);

  ForwardList(const ForwardList &other);
  ForwardList(ForwardList &&other) noexcept;

  ForwardList &operator=(const ForwardList &other);
  ForwardList &operator=(ForwardList &&other) noexcept;

  // Accessors
  reference front();
  const_reference front() const;

  // Capacity
  [[nodiscard]] bool empty() const;
  [[nodiscard]] size_type size() const;

  // Modifiers
  void clear() noexcept;
  void pushFront(const_reference value);
  void pushFront(T &&value);
  template <class... Args> reference emplaceFront(Args &&...args);
  void popFront();
  void resize(const size_type &count);
  void resize(size_type count, const_reference value);
};
} // namespace Utils

#endif // !FORWARD_LIST_HPP
