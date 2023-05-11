#ifndef BLOCK_CHAIN_HPP
#define BLOCK_CHAIN_HPP

#include "Person/Person.hpp"
#include "io1/money.hpp"

#include <chrono>
#include <string>

namespace BlockChain {

using TIME = std::chrono::time_point<std::chrono::utc_clock>;
using std::string;

class BlockChain {

public:
  class Block {

    struct Data {
      io1::Money m_amount;
      TIME m_timestamp;
      string m_origin_wallet;
      string m_target_wallet;
      Data() = delete;
      Data(const string &originWalletId, const string &targetWalletId,
           const io1::Money &amount);
    };

  public:
    std::string currentHash();

  private:
    Data data;
    string previous_hash;
    Block &next;
    Block &previous;
  };
};

using Block = BlockChain::Block;

} // namespace BlockChain

#endif // !BLOCK_CHAIN_HPP
