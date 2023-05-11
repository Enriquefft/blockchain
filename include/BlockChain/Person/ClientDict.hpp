#ifndef CLIENT_DICT_HPP
#define CLIENT_DICT_HPP

#include "Person.hpp"
#include "Utils/Array/Array.hpp"

namespace Person {

class ClientDict {

  std::vector<Client> m_clients;

  void addClient(const string &name);
};

} // namespace Person

#endif
