#ifndef SET_HPP
#define SET_HPP

#include <concepts>
#include <functional>

enum class Container { BST, AVL, HASH };
using enum Container;

template <class Key, class Compare = std::less<Key>, Container = BST>
class SetBase {
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
  SetBase() = default;
  SetBase(const SetBase &) = default;
  SetBase(SetBase &&) noexcept = default;
  SetBase &operator=(const SetBase &) = default;
  SetBase &operator=(SetBase &&) noexcept = default;
  SetBase(std::initializer_list<value_type>);
  virtual ~SetBase() = delete;

  // Iterators

private:
};

#endif // !SET_HPP
