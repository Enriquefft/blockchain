#include "BlockChain.hpp"
// #include "Utils/Deque/Deque.hpp"
#include <ios>
#include <iostream>

using std::cout;
// using Utils::Deque;

int main() {

  // Deque<int> d;
  //
  // d.push_back(1);
  // d.push_back(2);
  // d.pop_back();
  // d.push_front(0);
  //
  // for (auto i : d) {
  //   cout << i << " ";
  // }
  // for (const auto &i : d) {
  //   cout << i << " ";
  // }

  blockchain::BlockChain b_chain;
  b_chain.addBlock(Data("me", "you", io1::Money(10)));
  b_chain.addBlock(Data("you", "me", io1::Money(20)));
  b_chain.addBlock(Data("1", "2", io1::Money(30)));
  b_chain.addBlock(Data("he", "she", io1::Money(40)));

  for (const auto &data : b_chain) {
    cout << data << std::endl;
  }

  cout << "isConsistent?: \n"
       << std::boolalpha << b_chain.isConsistent() << std::endl;

  cout << "randomInyection" << std::endl;
  b_chain.randomInyection();

  cout << "isConsistent?: " << std::boolalpha << b_chain.isConsistent()
       << std::endl;

  return 0;
}
