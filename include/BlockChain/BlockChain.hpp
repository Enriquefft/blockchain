#ifndef BLOCK_CHAIN_HPP
#define BLOCK_CHAIN_HPP

#include "Data.hpp"
#include <string>

namespace BlockChain {

using std::string;

class BlockChain {

public:
  class Block {

  public:
    std::string currentHash();

  private:
    Data data;
    string previous_hash;
    Block *next;
    Block *previous;
  };
};

using Block = BlockChain::Block;

} // namespace BlockChain

#endif // !BLOCK_CHAIN_HPP
