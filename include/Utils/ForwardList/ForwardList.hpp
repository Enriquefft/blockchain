#ifndef FORWARD_LIST_HPP
#define FORWARD_LIST_HPP

#include "Utils/gsl.hpp"
#include <iterator>

namespace Utils {

template <class T> class ForwardList {

  template <bool isConst> class fl_iterator;
  template <bool isConst> friend class fl_iterator;

public:
  // Typedefs
  using value_type = T;
  using size_type = size_t;
  using reference = T &;
  using const_reference = const T &;

  using pointer = T *;
  using const_pointer = const T *;

  using iterator = fl_iterator<false>;
  using const_iterator = fl_iterator<true>;

private:
  struct Node {

    Node(const Node &) = default;
    Node(Node &&) noexcept = default;
    Node &operator=(const Node &) = default;
    Node &operator=(Node &&) noexcept = default;

    explicit Node(value_type _data) : data(std::move(_data)), next(nullptr) {}

    value_type data;
    gsl::owner<Node *> next;
    ~Node() { delete next; }
  };

  gsl::owner<Node *> m_head = nullptr;
  Node *m_tail = nullptr;
  size_type m_size = 0;

public:
  // Constructors
  ForwardList() = default;
  ~ForwardList() { delete m_head; }

  explicit ForwardList(const size_type &count);
  ForwardList(std::initializer_list<T> init);
  ForwardList(const size_type &count, const T &value);

  ForwardList(const ForwardList &other);
  ForwardList(ForwardList &&other) noexcept;

  ForwardList &operator=(const ForwardList &other);
  ForwardList &operator=(ForwardList &&other) noexcept;

  // Accessors
  reference front();
  [[nodiscard]] const_reference front() const;

  pointer find(const_reference value);

  // Iterators
  iterator begin() noexcept;
  [[nodiscard]] const_iterator begin() const noexcept;
  [[nodiscard]] const_iterator cbegin() const noexcept;

  iterator end() noexcept;
  [[nodiscard]] const_iterator end() const noexcept;
  [[nodiscard]] const_iterator cend() const noexcept;

  // Capacity
  [[nodiscard]] bool empty() const;
  [[nodiscard]] size_type size() const;

  // Modifiers
  void clear() noexcept;
  void pushFront(const_reference value);
  void pushFront(value_type &&value);

  void pushBack(const_reference value);
  void pushBack(value_type &&value);

  void popFront();
  void resize(const size_type &count);
  void resize(const size_type &count, const_reference value);

  iterator eraseAfter(const_iterator pos);
  iterator eraseAfter(const_iterator first, const_iterator last);

  template <class... Args> reference emplaceFront(Args &&...args) {
    m_size++;

    if (m_tail == nullptr) {
      m_head = new Node(T(std::forward<Args>(args)...));
      m_tail = m_head;
      return m_tail->data;
    }

    m_tail->next = new Node(T(std::forward<Args>(args)...));
    m_tail = m_tail->next;
    return m_tail->data;
  }

  template <class... Args> reference emplaceBack(Args &&...args) {
    m_size++;

    if (m_tail == nullptr) {
      m_head = new Node(T(std::forward<Args>(args)...));
      m_tail = m_head;
      return m_tail->data;
    }

    gsl::owner<Node *> new_head = new Node(T(std::forward<Args>(args)...));
    new_head->next = m_head;
    m_head = new_head;

    return m_tail->data;
  }

private:
  template <bool isConst> class fl_iterator {
    friend class ForwardList;

  public:
    // Typedefs
    using difference_type = std::ptrdiff_t;
    using value_type = std::conditional_t<isConst, const T, T>;
    using reference = value_type &;
    using value_pointer = value_type *;
    using iterator_category = std::forward_iterator_tag;

    explicit fl_iterator(Node *node = nullptr);

    // Implicit conversion between const_iterator and iterator
    template <bool otherConst> operator fl_iterator<otherConst>() {
      return fl_iterator<otherConst>(current);
    }

    // Accesors
    reference operator*();
    value_pointer operator->();

    // Traversal
    fl_iterator &operator++();
    auto operator++(int) -> fl_iterator;

    bool operator==(const fl_iterator &rhs) const;
    bool operator!=(const fl_iterator &rhs) const;

  private:
    Node *current;
  };
};

extern template class ForwardList<int>;
extern template class ForwardList<int>::fl_iterator<true>;
extern template class ForwardList<int>::fl_iterator<false>;

} // namespace Utils

#endif // !FORWARD_LIST_HPP
