#ifndef AVL_HPP
#define AVL_HPP

#include "BlockChain.hpp"
#include "SearchStructures/SearchStructureBase.hpp"

#include <memory>

namespace DataStructures {

template <CompareType type> class AVL : public SearchStructureBase<type> {
private:
  class Node;

public:
  AVL() = default;
  explicit AVL(BlockChain::BlockChain);

private:
  Node *root;

  class Node {
  public:
    Node() = delete;
    explicit Node(const Data &data);

  private:
    std::shared_ptr<Data> m_data;
    Node *m_left;
    Node *m_right;
    size_t m_height{};
  };
};

} // namespace DataStructures

#endif // !AVL_HPP
