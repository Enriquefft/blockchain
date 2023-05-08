
/*
 * Header only implementation of a stack container adapter.
 */

#ifndef STACK_HPP
#define STACK_HPP
#include <deque>
#include <ostream>

template <typename Container, typename T>
concept stack_container_t =
    requires(Container cont, T type) {
      { cont.push_back(type) } -> std::same_as<void>;
      { cont.pop_back() } -> std::same_as<void>;
      { cont.back() } -> std::same_as<T &>;
      { cont.size() } -> std::same_as<typename Container::size_type>;
      { cont.empty() } -> std::same_as<bool>;
    };

template <typename T, typename Container = std::deque<T>>
  requires stack_container_t<Container, T>
class Stack {
public:
  // Typedefs
  using value_type = T;
  using container_type = Container;
  using size_type = typename Container::size_type;
  using reference = typename Container::reference;
  using const_reference = typename Container::const_reference;
  using iterator = typename Container::iterator;
  using const_iterator = typename Container::const_iterator;

  // Constructors
  Stack() = default;
  explicit Stack(const Container &cont) : c(cont) {}

  // Accessors
  const_reference top() const { return c.back(); }
  reference top() { return c.back(); }
  const_iterator begin() const { return c.begin(); }
  const_iterator end() const { return c.end(); }
  iterator begin() { return c.begin(); }
  iterator end() { return c.end(); }

  // Capacity
  [[nodiscard]] bool empty() const { return c.empty(); }
  size_type size() const { return c.size(); }

  // Modifiers
  void push(const value_type &value) { c.push_back(value); }
  void push(value_type &&value) { c.push_back(std::move(value)); }
  template <typename... Args> void emplace(Args &&...args) {
    c.emplace_back(std::forward<Args>(args)...);
  }

  void pop() { c.pop_back(); }
  void swap(Stack &other) noexcept(noexcept(c.swap(other.c))) {
    c.swap(other.c);
  }

  // Friend operators
  friend bool operator==(const Stack &lhs, const Stack &rhs) {
    return lhs.c == rhs.c;
  }
  friend bool operator!=(const Stack &lhs, const Stack &rhs) {
    return lhs.c != rhs.c;
  }
  friend std::ostream &operator<<(std::ostream &os, const Stack &stack) {
    for (const auto &elem : stack.c) {
      os << elem << " ";
    }
    return os;
  }

private:
  Container c;
};

#endif // !STACK_HPP
