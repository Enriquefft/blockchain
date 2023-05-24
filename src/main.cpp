// #include "BlockChain.hpp"
// #include "Utils/Deque/Deque.hpp"
#include <ios>
#include <iostream>
#include "Utils/Deque/Deque.hpp"


using std::cout;
using std::endl;
// using Utils::Deque;

int main() {
  Deque<int>* d1 = new Deque<int>(24);
  d1->pushBack(2);
  d1->pushFront(1);
  cout<<d1->front()<<" "<<d1->back()<<endl;


  return 0;
}
