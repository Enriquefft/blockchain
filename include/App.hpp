#include "Person.hpp"
#ifndef APP_HPP

#include "BlockChain.hpp"
#include "Data.hpp"
#include "IndexStructures/IndexStructures.hpp"
#include <ClientDict.hpp>
#include <Vector/Vector.hpp>

using enum Data::CompareType;
using Person::Client;
using Person::ClientDict;

/*
 * Stores:

 * The blockchain
 * The users
 * The index structures
 */

enum class IndexStructures { B_TREE, HASH_TABLE, AVL_TREE };

class App {
public:
  App() = default;

  /*
   * User related functions
   */

  void addClient(const string &name, const string &password);
  [[nodiscard]] bool clientExists(const string &name,
                                  const string &password) const;
  [[nodiscard]] const Client &getClient(const string &client) const;
  [[nodiscard]] const Utils::Vector<Client> &getClients() const;

  /*
   * Blockchain related functions
   */

  // Adds a block to the blockchain and to every ACTIVE index structure
  void addBlock(const Data &data);

  [[nodiscard]] const blockchain::BlockChain &getBlockChain() const;
  [[nodiscard]] blockchain::BlockChainInfo getBlockChainInfo() const;

  /*
   * Index structure related queries
   */
  template <CompareType compareType, IndexStructures structure>
  [[nodiscard]] const Utils::Vector<Data> &min();
  template <CompareType compareType, IndexStructures structure>
  [[nodiscard]] const Utils::Vector<Data> &max();
  template <CompareType compareType, IndexStructures structure>
  [[nodiscard]] const Utils::Vector<Data> &
  contains(const getType<compareType> &value);
  template <CompareType compareType, IndexStructures structure>
  [[nodiscard]] const Utils::Vector<Data> &
  search(const getType<compareType> &value);
  template <CompareType compareType, IndexStructures structure>
  [[nodiscard]] const Utils::Vector<Data> &
  rangeSearch(const getType<compareType> &min, const getType<compareType> &max);

private:
  blockchain::BlockChain m_blockChain;
  Person::ClientDict m_clients;

  // Data structure to store all the index structures
  // Should only initialize the index strucures when a query is made, then,
  // those structures should be automatically updated
  // TODO(Joel): Implement
};

#endif // !APP_HPP
