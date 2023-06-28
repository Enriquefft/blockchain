#ifndef IS_BST_HPP
#define IS_BST_HPP

#include "BlockChain/Data.hpp"
#include <Utils/Vector/Vector.hpp>

template <CompareType comp> class BST {
public:
  using size_type = std::size_t;
  using search_type = getType<comp>;

  BST();
  void insert(Data &data);
  [[nodiscard]] bool search(const search_type &value) const;

  [[nodiscard]] /*std::ranges::subrange*/ Utils::Vector<search_type>
  rangeSearch(const search_type &minValue, const search_type &maxValue) const;

  [[nodiscard]] /*std::ranges::subrange*/ Utils::Vector<Data>
  getElements() const;

  [[nodiscard]] size_type height() const;
  [[nodiscard]] size_type size() const;
  void clear();
  [[nodiscard]] bool empty() const;

private:
  class Node {};

  Node *root;
};

#endif // !IS_BST_HPP
