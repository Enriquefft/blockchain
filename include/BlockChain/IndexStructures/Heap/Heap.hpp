#ifndef HEAP_HPP
#define HEAP_HPP

#include "Data.hpp"
#include "Utils/Vector/Vector.hpp"
#include <BlockChain.hpp>
#include <stdexcept>

#include <Utils/gsl.hpp>

using Utils::Vector;

template <Data::CompareType type> class Heap {

public:
  enum Type { MAX_HEAP, MIN_HEAP };

private:
  gsl::owner<Data> *m_elements;
  int m_size = 0;
  int m_capacit0 = 3;

  Data::Compare<type> comparator;

public:
  explicit Heap(const blockchain::BlockChain &blockchain) {
    for (const auto &block : blockchain) {
      this->push(block);
    }
  }

  ~Heap() { delete[] m_elements; }

  int size() { return m_size; }

  bool is_empty() { return m_size == 0; }

  void push(Data value) {
    if (n == capacity) {
      capacity *= 2;
      Data *newElements = new Data;
      for (int i = 0; i < n; i++) {
        newElements[i] = elements[i];
      }
      delete[] elements;
      elements = newElements;
    }

    elements[n] = value;
    n++;
    heapify_up(n - 1);
  }

  Data pop() {
    if (is_empty())
      throw std::out_of_range("El heap esta vacio");

    Data root = m_elements[0];
    m_elements[0] = m_elements[m_size - 1];
    m_size--;
    heapify_down(0);

    return root;
  }

  Data top() {
    if (m_size == 0)
      throw std::out_of_range("El heap esta vacio");

    return m_elements[0];
  }

  Vector<Data> extractTop(int TopElems) {
    if (TopElems > m_size)
      throw std::out_of_range("k es mayor que n");

    Vector<Data> result;
    for (int i = 0; i < TopElems; i++) {
      result.push_back(pop());
    }

    return result;
  }

  static void sortAsc(Data *arr, int n) {
    Heap heap(arr, n, MAX_HEAP);
    for (int i = n - 1; i >= 0; i--) {
      arr[i] = heap.pop();
    }
  }

  static void sortDesc(Data *arr, int n) {
    Heap heap(arr, n, MIN_HEAP);
    for (int i = n - 1; i >= 0; i--) {
      arr[i] = heap.pop();
    }
  }

private:
  int Parent(int i) { return (i - 1) / 2; }

  int Left(int i) { return (2 * i + 1); }

  int Right(int i) { return (2 * i + 2); }

  void heapify_down(int i) {
    int left = Left(i);
    int right = Right(i);
    int Mas_grande = i;

    if (type == MAX_HEAP && left < n && elements[left] > elements[Mas_grande])
      Mas_grande = left;
    if (type == MAX_HEAP && right < n && elements[right] > elements[Mas_grande])
      Mas_grande = right;
    if (type == MIN_HEAP && left < n && elements[left] < elements[Mas_grande])
      Mas_grande = left;
    if (type == MIN_HEAP && right < n && elements[right] < elements[Mas_grande])
      Mas_grande = right;
    if (Mas_grande != i) {
      swap(elements[i], elements[Mas_grande]);
      heapify_down(Mas_grande);
    }
  }

  void heapify_up(int i) {
    int parent = Parent(i);

    if (type == MAX_HEAP && i > 0 && elements[i] > elements[parent]) {
      swap(elements[i], elements[parent]);
      heapify_up(parent);
    }

    if (type == MIN_HEAP && i > 0 && elements[i] < elements[parent]) {
      swap(elements[i], elements[parent]);
      heapify_up(parent);
    }
  }

  void buildHeap() {
    for (int i = (n / 2) - 1; i >= 0; i--) {
      heapify_down(i);
    }
  }
};

#endif // HEAP_HPP
