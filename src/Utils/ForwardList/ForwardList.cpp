#include "Utils/ForwardList/ForwardList.hpp"
#include <cstdint>
#include <utility>

// Explicit Declaration only
#include "Utils/RadixTrie/RadixTrie.hpp"

namespace Utils {

// Constructors
template <typename T> ForwardList<T>::ForwardList(const size_type &count) {
  for (uint64_t i = 0; i < count; ++i) {
    pushFront(T());
  }
}

template <typename T>
ForwardList<T>::ForwardList(const size_type &count, const_reference value) {

  for (uint64_t i = 0; i < count; ++i) {
    pushFront(value);
  }
}

template <typename T>
ForwardList<T>::ForwardList(std::initializer_list<T> init) {
  for (auto &value : init) {
    emplaceFront(value);
  }
}

template <typename T> ForwardList<T>::ForwardList(const ForwardList &other) {
  for (auto &value : other) {
    emplaceFront(value);
  }
}
template <typename T>
ForwardList<T>::ForwardList(ForwardList &&other) noexcept
    : m_head(other.m_head), m_tail(other.m_tail), m_size(other.m_size) {
  other.m_head = nullptr;
  other.m_tail = nullptr;
  other.m_size = 0;
}

template <typename T>
ForwardList<T> &ForwardList<T>::operator=(const ForwardList &other) {

  // Forward list copy assignment operator
  if (this != &other) {
    clear();

    for (auto &value : other) {
      emplaceFront(value);
    }
  }

  return *this;
}

template <typename T>
ForwardList<T> &ForwardList<T>::operator=(ForwardList &&other) noexcept {

  if (this != &other) {
    clear();

    m_head = other.m_head;
    m_tail = other.m_tail;
    m_size = other.m_size;

    other.m_head = nullptr;
    other.m_tail = nullptr;
    other.m_size = 0;
  }

  return *this;
}

// Accessors
template <typename T> auto ForwardList<T>::front() -> reference {
  return m_tail->data;
}
template <typename T> auto ForwardList<T>::front() const -> const_reference {
  return m_tail->data;
}

template <typename T> T *ForwardList<T>::find(const_reference value) {
  auto curr = m_head;
  auto halve = m_head;

  bool advance_halve = false;

  while (curr != nullptr && curr->data != value) {
    curr = curr->next;

    if (advance_halve) {
      halve = halve->next;
    }

    advance_halve = !advance_halve;
  }

  if (curr == nullptr) {
    return nullptr;
  }

  std::swap(curr->data, halve->data);
  return &halve->data;
}

// Capacity
template <typename T> bool ForwardList<T>::empty() const { return m_size == 0; }

template <typename T> auto ForwardList<T>::size() const -> size_type {
  return m_size;
}

// Modifiers
template <typename T> void ForwardList<T>::clear() noexcept {
  delete m_head;
  m_head = static_cast<gsl::owner<Node *>>(nullptr);
  m_tail = nullptr;
  m_size = 0;
}
template <typename T> void ForwardList<T>::pushFront(value_type &&value) {
  m_size++;

  if (m_tail == nullptr) {
    m_head = new Node(value);
    m_tail = m_head;
    return;
  }

  m_tail->next = new Node(std::move(value));
  m_tail = m_tail->next;
}

template <typename T> void ForwardList<T>::pushFront(const_reference value) {
  m_size++;

  if (m_tail == nullptr) {
    m_head = new Node(value);
    m_tail = m_head;
    return;
  }

  m_tail->next = new Node(value);
  m_tail = m_tail->next;
}

template <typename T> void ForwardList<T>::pushBack(value_type &&value) {
  m_size++;

  if (m_tail == nullptr) {
    m_head = new Node(value);
    m_tail = m_head;
    return;
  }

  gsl::owner<Node *> new_head = new Node(std::move(value));
  new_head->next = m_head;
  m_head = new_head;
}
template <typename T> void ForwardList<T>::pushBack(const_reference value) {
  m_size++;

  if (m_tail == nullptr) {
    m_head = new Node(value);
    m_tail = m_head;
    return;
  }

  gsl::owner<Node *> new_head = new Node(value);
  new_head->next = m_head;
  m_head = new_head;
}

template <typename T>
void ForwardList<T>::resize(const size_type &count, const_reference value) {

  m_size = count;
  bool is_incremented = count >= m_size;

  for (size_type i = m_size; i < count; ++i) {
    pushFront(value);
  }

  if (is_incremented) {
    return;
  }

  gsl::owner<Node *> curr = m_head;
  for (size_type i = 0; i < count; ++i) {
    curr = curr->next;
  }

  delete curr->next;
  curr->next = nullptr;
}

template <typename T> void ForwardList<T>::resize(const size_type &count) {
  resize(count, T());
}

template <typename T>
auto ForwardList<T>::eraseAfter(const_iterator pos) -> iterator {

  if (pos == cend()) {
    return end();
  }

  iterator it = pos;

  auto *removed = it.current->next;
  it.current->next = removed->next;
  removed->next = nullptr;

  delete removed;
  return it++;
}

template <typename T>
auto ForwardList<T>::eraseAfter(const_iterator first, const_iterator last)
    -> iterator {

  auto remove_count = std::distance(first, last) - 1;

  for (auto i = 0; i < remove_count; i++) {
    eraseAfter(first);
  }

  return last;
}

template <typename T>
template <bool isConst>
ForwardList<T>::fl_iterator<isConst>::fl_iterator(Node *node) : current(node) {}

template <typename T>
template <bool isConst>
auto ForwardList<T>::fl_iterator<isConst>::operator*() -> reference {
  return current->data;
}
template <typename T>
template <bool isConst>
auto ForwardList<T>::fl_iterator<isConst>::operator->() -> value_pointer {
  return &current->data;
}
template <typename T>
template <bool isConst>
auto ForwardList<T>::fl_iterator<isConst>::operator++() -> fl_iterator & {

  current = current->next;
  return *this;
}

template <typename T>
template <bool isConst>
auto ForwardList<T>::fl_iterator<isConst>::operator++(int) -> fl_iterator {
  auto tmp = *this;
  ++(*this);
  return tmp;
}

template <typename T>
template <bool isConst>
bool ForwardList<T>::fl_iterator<isConst>::operator==(
    const fl_iterator &rhs) const {
  return current == rhs.current;
}
template <typename T>
template <bool isConst>
bool ForwardList<T>::fl_iterator<isConst>::operator!=(
    const fl_iterator &rhs) const {
  return current != rhs.current;
}

template <typename T> auto ForwardList<T>::begin() noexcept -> iterator {
  return iterator(m_head);
}
template <typename T>
auto ForwardList<T>::begin() const noexcept -> const_iterator {
  return const_iterator(m_head);
}
template <typename T>
auto ForwardList<T>::cbegin() const noexcept -> const_iterator {
  return const_iterator(m_head);
}

template <typename T> auto ForwardList<T>::end() noexcept -> iterator {
  return iterator(nullptr);
}
template <typename T>
auto ForwardList<T>::end() const noexcept -> const_iterator {
  return const_iterator(nullptr);
}
template <typename T>
auto ForwardList<T>::cend() const noexcept -> const_iterator {
  return const_iterator(nullptr);
}

// Explicit instantiation
template class ForwardList<int>;
template class ForwardList<int>::fl_iterator<true>;
template class ForwardList<int>::fl_iterator<false>;

template class ForwardList<RadixTrie::Node>;
template class ForwardList<RadixTrie::Node>::fl_iterator<true>;
template class ForwardList<RadixTrie::Node>::fl_iterator<false>;

} // namespace Utils
