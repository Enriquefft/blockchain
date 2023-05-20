#include "BlockChain.hpp"
#include <ios>
#include <iostream>
int main() {

  blockchain::BlockChain b_chain;
  b_chain.addBlock(Data("me", "you", io1::Money(10)));
  b_chain.addBlock(Data("you", "me", io1::Money(20)));
  b_chain.addBlock(Data("1", "2", io1::Money(30)));
  b_chain.addBlock(Data("he", "she", io1::Money(40)));
  // b_chain.addBlock(Data("her", "this", io1::Money(50)));
  // b_chain.addBlock(Data("no one", "it", io1::Money(60)));

  for (const auto &data : b_chain) {
    std::cout << data.sender << " Sent " << data.amount << " to "
              << data.receiver << " on " << data.timestamp << std::endl;
    std::cout << "next block" << std::endl;
  }

  std::cout << "done" << std::endl;

  return 0;
}
