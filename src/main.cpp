#include "B/B.hpp"
#include "BlockChain.hpp"
#include <vector>

int main() {
  cout << "Hello World!\n";

  blockchain::BlockChain b_chain;
  B<Data::CompareType::MONEY_ASCENDING> btree(4);

  cout << "Created empty blockchain\n";

  b_chain.addBlock(Data("me", "you", 10));

  cout << "=====================================\n";

  for (const auto &data : b_chain) {
    btree.insert(&data);
  }

  return 0;
}
