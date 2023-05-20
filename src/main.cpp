#include "BlockChain.hpp"
#include <ios>
#include <iostream>
int main() {

  blockchain::BlockChain b_chain;
  b_chain.addBlock(Data("me", "you", io1::Money(10)));
  b_chain.addBlock(Data("you", "me", io1::Money(20)));
  b_chain.addBlock(Data("1", "2", io1::Money(30)));
  b_chain.addBlock(Data("he", "she", io1::Money(40)));

  // for (const auto &data : b_chain) {
  //   std::cout << data.sender << " Sent " << data.amount << " to "
  //             << data.receiver << " on " << data.timestamp << std::endl;
  //   std::cout << "next block" << std::endl;
  // }

  std::cout << "end isEnd: " << std::boolalpha << b_chain.end().isEnd() << '\n';
  for (blockchain::BlockChain::iterator it = b_chain.begin();
       it != b_chain.end(); ++it) {
    std::cout << (*it).sender << " Sent " << (*it).amount << " to "
              << (*it).receiver << " on " << (*it).timestamp << std::endl;

    std::cout << "curr isEnd: " << std::boolalpha << it.isEnd() << '\n';
    std::cout << "next isEnd: " << std::boolalpha << std::next(it).isEnd()
              << '\n';
  }

  std::cout << "done" << std::endl;

  return 0;
}
