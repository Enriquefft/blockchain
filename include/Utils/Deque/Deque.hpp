#ifndef DEQUE_HPP
#define DEQUE_HPP

#include <cstddef>
#include "Iterator.hpp"
#define BLOCK_SIZE XXXX

template <typename T,size_t buffsize = 4> class Deque {
public:
  // template <bool IsConst> class Iterator;
  typedef deque_iterator<T,buffsize> iterator;
  using value_type = T;
  using size_type = std::size_t;
  using reference = T &;
  using const_reference = const T &;
  using pointer = T *;
  using const_pointer = const T *;
  using map_pointer = pointer*;

  // using iterator = Iterator<false>;
  // using const_iterator = Iterator<true>;

  // Constructors
  Deque() noexcept = default;
  // explicit Deque(size_type count, const_reference value = T());
  explicit Deque(size_type count){
    map_size = count / buffsize;

    map = new *T[map_size];
    for(int i=0;i<map_size;i++){
      map[i] = new T[buffsize];
    }
    map[int(map_size/2)] = new T[buffsize];
    map_pointer tmp_start = map + int(map_size / 2);
    map_pointer tmp_finish = map + int(map_size / 2);

    start.set_node(tmp_start);
    start.current = start.first;

    finish.set_node(tmp_finish);
    finish.current = finish.first + 1;
  }

  // Accessors
  reference at(size_type pos){
    return start[pos + 1];
  }
  const_reference at(size_type pos) const{
    return start[pos + 1];
  }

  reference operator[](size_type pos){
    return start[pos + 1];
  }
  const_reference operator[](size_type pos) const{
    return start[pos + 1];
  }

  reference front(){
    iterator temp = start;
    ++temp;
    return *temp;
  }
  const_reference front() const{
    iterator temp = start;
    ++temp;
    return *temp;
  }

  reference back(){
    iterator temp = finish;
    --temp;
    return *temp;
  }
  const_reference back() const{
    iterator temp = finish;
    --temp;
    return *temp;
  }

  // Iterators
  iterator begin() noexcept{
    iterator tmp = start;
    ++tmp;
    return tmp;
  }
  // const_iterator begin() const noexcept;
  iterator end() noexcept{
    return finish;
  }
  // const_iterator end() const noexcept;

  // Capacity
  [[nodiscard]] bool empty() const noexcept;
  [[nodiscard]] size_type size() const noexcept;
  [[nodiscard]] size_type max_size() const noexcept;
  void shrink_to_fit();

  // Modifiers
  void clear() noexcept;
  /*insert*/
  template <typename... Args> reference emplace_front(Args &&...args);

private:
  // Que atributos usar
  map_pointer m_map;
  size_type map_size;
  // Tamaño del mapa
  iterator start;
  iterator finish;
};

#endif // !DEQUE_HPP
