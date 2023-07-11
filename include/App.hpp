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

  void addClient(const string &name, const string &password){
    m_clients.addClient(&name,&password);
  }
  [[nodiscard]] bool clientExists(const string &name,const string &password) const{
    return m_clients.contains(&name);
  }
  [[nodiscard]] const Client &getClient(const string &client) const{
    for(const auto& forwardlist : m_clients.m_clients){
      for (const  auto& pair : forwardlist) { 
        if(pair.first == client) return pair.second;
      }
    }
  }
  [[nodiscard]] const Utils::Vector<Client> &getClients() const{
    Vector<Client> clients;
    for(const auto& forwardlist : m_clients.m_clients){
      for (const  auto& pair : forwardlist) { 
        if(pair.first == client) clients.push_back(pair.second);
      }
    }
    return clients;
  }

  /*
   * Blockchain related functions
   */

  // Adds a block to the blockchain and to every ACTIVE index structure
  void addBlock(const Data &data){
    m_blockChain.addBlock(data);
  }

  [[nodiscard]] const blockchain::BlockChain &getBlockChain() const {
    return m_blockChain;

  }
  [[nodiscard]] blockchain::BlockChainInfo getBlockChainInfo() const;

  /*
   * Index structure related queries
   */
  template <CompareType compareType, IndexStructures structure>
  [[nodiscard]] const Utils::Vector<Data> &min(){
    structure<compareType>();
    for(auto data : m_blockChain){
      const Data *ptr = &data;
      structure.insert(ptr);
    }
    Vector<const Data*> result = structure.min();
    return result;
  }
  template <CompareType compareType, IndexStructures structure>
  [[nodiscard]] const Utils::Vector<Data> &max(){
    structure<compareType>();
    for(auto data : m_blockChain){
      const Data *ptr = &data;
      structure.insert(ptr);
    }
    Vector<const Data*> result = structure.max();
    return result;
  }
  template <CompareType compareType, IndexStructures structure>
  [[nodiscard]] const Utils::Vector<Data> &contains(const getType<compareType> &value){
    structure<compareType>();
    for(auto data : m_blockChain){
      const Data *ptr = &data;
      structure.insert(ptr);
    }
    vector<const Data *> result = structure.getElements(value);
  }
  template <CompareType compareType, IndexStructures structure>
  [[nodiscard]] const Utils::Vector<Data> &
  search(const getType<compareType> &value){
    structure<compareType>();
    for(auto data : m_blockChain){
      const Data *ptr = &data;
      structure.insert(ptr);
    }
    const Data *ptr = &value;
    return structure.search(ptr);
  }
  template <CompareType compareType, IndexStructures structure>
  [[nodiscard]] const Utils::Vector<Data> &
  rangeSearch(const getType<compareType> &min, const getType<compareType> &max){
    structure<compareType>();
    for(auto data : m_blockChain){
      const Data *ptr = &data;
      structure.insert(ptr);
    }
    return structure.rangeSearch(min, max);
  }

private:
  blockchain::BlockChain m_blockChain;
  Person::ClientDict m_clients;

  // Data structure to store all the index structures
  // Should only initialize the index strucures when a query is made, then,
  // those structures should be automatically updated
  // TODO(Joel): Implement
};

#endif // !APP_HPP
