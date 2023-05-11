#ifndef AVL_HPP
#define AVL_HPP

#include "BlockChain.hpp"
#include "SearchStructures/SearchStructureBase.hpp"

class AVL : public SearchStructureBase {
public:
private:
  explicit AVL(BlockChain::BlockChain);
};

#endif // !AVL_HPP
