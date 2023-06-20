#include "BlockChain.hpp"
#include "Utils/Deque/Deque.hpp"
#include <iostream>
using std::cout;
#include<vector>
#include "../include/BlockChain/prueba.hpp"


int main() {

  cout << "Hello World!\n";

  blockchain::BlockChain b_chain;

  cout << "Created empty blockchain\n";

  b_chain.addBlock(Data("me", "you", 10));
  b_chain.addBlock(Data("you", "me", 20));
  b_chain.addBlock(Data("1", "2", 30));
  b_chain.addBlock(Data("he", "she", 40));

  cout<<"=====================================\n";
  std::vector<Data> vec;
  for(auto &data : b_chain){
	vec.push_back(data);
  }

  bst* b1 = new bst();
  b1->insert(vec[0]);
  b1->insert(vec[1]);
  b1->insert(vec[2]);
  b1->insert(vec[3]);

  b1->display();

/*
  Data::Compare<Data::CompareType::MONEY_ASCENDING> comparator;

  bool result = comparator(vec[1], vec[0]);
  cout<<std::boolalpha<<result<<std::endl;
 */
	







  //cout<<std::boolalpha<<vec[0] > vec[1];

  /*
  for (const auto &data : b_chain) {
    cout << data << std::endl;
  }

  cout << "isConsistent?: \n"
       << std::boolalpha << b_chain.isConsistent() << std::endl;

  cout << "randomInyection" << std::endl;
  b_chain.randomInyection();

  cout << "isConsistent?: " << std::boolalpha << b_chain.isConsistent()
       << std::endl;
	*/
}
