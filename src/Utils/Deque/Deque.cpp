#include "Utils/Deque/Deque_Prototipo.hpp"

// PRIVATE

template <typename T>
void Deque<T>::fillInitialize(size_t size, const_reference value) {

  createMapAndNodes(size);

  for (pointer cur = start.current; cur != finish.current; ++cur) {
    *cur = value;
  }
}
