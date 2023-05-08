#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <deque>
#include <ostream>

template <typename T, typename Container>
concept queue_container_t = requires(Container cont, T type) {
    { cont.push_back(type) } -> std::same_as<void>;
    { cont.pop_front() } -> std::same_as<void>;
    { cont.back() } -> std::same_as<T&>;
    { cont.front() } -> std::same_as<T&>;
    { cont.size() } -> std::same_as<typename Container::size_type>;
    { cont.empty() } -> std::same_as<bool>;
};

template <typename T, typename Container = std::deque<T>>
requires queue_container_t<T, Container>
class Queue {
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
  Queue() = default;
  explicit Queue(const Container& cont) : c(cont) {}
  
  // Accessors
  const_reference front() const { return c.front(); }
  reference front() { return c.front(); }
  const_reference back() const { return c.back(); }
  reference back() { return c.back(); }

  iterator begin() { return c.begin(); }
  const_iterator begin() const { return c.begin(); }
  iterator end() { return c.end(); }
  const_iterator end() const { return c.end(); }


  // Capacity
  [[nodiscard]] bool empty() const { return c.empty(); }
  [[nodiscard]] size_type size() const { return c.size(); }
    
  // Modifiers
  void push(const value_type& value) { c.push_back(value); }
  void push(value_type&& value) { c.push_back(std::move(value)); }
  template <typename... Args>
  void emplace(Args&&... args) {
    c.emplace_back(std::forward<Args>(args)...);
  }
  void pop() { c.pop_front(); }
  void swap(Queue& other) noexcept(noexcept(c.swap(other.c))) {
    c.swap(other.c);
  }

  // Friend operators
  friend bool operator==(const Queue& lhs, const Queue& rhs) {
    return lhs.c == rhs.c;
  }
  friend bool operator!=(const Queue& lhs, const Queue& rhs) {
    return lhs.c != rhs.c;
  }
  friend std::ostream& operator<<(std::ostream& os, const Queue& queue) {
    for (const auto& elem : queue.c) {
      os << elem << " ";
    }
    return os;
  }
  private:
  Container c;
};


#endif
