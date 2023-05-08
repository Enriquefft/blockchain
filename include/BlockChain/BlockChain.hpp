#ifndef BLOCK_CHAIN_HPP
#define BLOCK_CHAIN_HPP

#include "Person/Person.hpp"
#include <chrono>
#include <string>

namespace BlockChain {

using TIME = std::chrono::time_point<std::chrono::utc_clock>;
using std::string;

template <typename T> class BlockChain {

  class Block {

  public:
    string previous_hash;

  private:
  };
};
} // namespace BlockChain

#endif // !BLOCK_CHAIN_HPP
