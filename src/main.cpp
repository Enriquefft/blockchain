#include "BlockChain.hpp"
#include "Utils/Deque/Deque.hpp"
#include <iostream>
using std::cout;

int main() {

  cout << "Hello World!\n";

  blockchain::BlockChain b_chain;

  cout << "Created empty blockchain\n";

  b_chain.addBlock(Data("me", "you", 10));

  cout << "Added block\n";

  b_chain.addBlock(Data("you", "me", 10));
  b_chain.addBlock(Data("1", "2", 10));
  b_chain.addBlock(Data("he", "she", 10));

  for (const auto &data : b_chain) {
    cout << data << std::endl;
  }

  cout << "isConsistent?: \n"
       << std::boolalpha << b_chain.isConsistent() << std::endl;

  cout << "randomInyection" << std::endl;
  b_chain.randomInyection();

  cout << "isConsistent?: " << std::boolalpha << b_chain.isConsistent()
       << std::endl;
}
