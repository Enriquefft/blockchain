#ifndef APP_HPP

#include "BlockChain.hpp"
#include "Data.hpp"
#include "IndexStructures/IndexStructures.hpp"

using enum Data::CompareType;

/*
 * Stores:

 * The blockchain
 * The users
 * The index structures
 */

class App {
public:
  App() = default;

  void addBlock(const Data &data);

private:
  blockchain::BlockChain blockChain;
};

#endif // !APP_HPP
