#ifndef SET_HPP
#define SET_HPP

#include "RBTree/RBTree.hpp"

namespace Utils {

template <class Key, class Compare = std::less<Key>> class Set {
  // Virtual parent class
public:
  template <bool IsConst> class _iterator;

  // Type aliases
  using key_type = Key;
  using value_type = Key;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using key_compare = Compare;
  using value_compare = Compare;
  using reference = value_type &;
  using const_reference = const value_type &;
  using pointer = value_type *;
  using const_pointer = const value_type *;
  using iterator = _iterator<false>;
  using const_iterator = _iterator<true>;

  // Constructors
  Set() = default;

  Set(std::initializer_list<value_type>);

  // Iterators
  iterator begin() noexcept;
  [[nodiscard]] const_iterator begin() const noexcept;
  [[nodiscard]] const_iterator cbegin() const noexcept;

  iterator end() noexcept;
  [[nodiscard]] const_iterator end() const noexcept;
  [[nodiscard]] const_iterator cend() const noexcept;

  // Capacity
  [[nodiscard]] bool empty() const noexcept;
  [[nodiscard]] size_type size() const noexcept;

  // Modifiers
  void clear() noexcept;
  std::pair<iterator, bool> insert(const value_type &);
  std::pair<iterator, bool> insert(value_type &&);
  template <class... Args> std::pair<iterator, bool> emplace(Args &&...);

  template <bool IsConst> iterator erase(_iterator<IsConst> position) noexcept;
  template <bool IsConst>
  iterator erase(_iterator<IsConst> first, _iterator<IsConst> last) noexcept;
  iterator erase(const value_type &);

  // Lookup
  [[nodiscard]] size_type count(const value_type &) const noexcept;

  iterator find(const value_type &) noexcept;
  [[nodiscard]] const_iterator find(const value_type &) const noexcept;

  bool contains(const value_type &) const noexcept;

private:
  RedBlackTree<value_type, key_compare> tree;
};
} // namespace Utils

#endif // !SET_HPP
