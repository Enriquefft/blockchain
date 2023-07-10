#ifndef CLIENT_DICT_HPP
#define CLIENT_DICT_HPP

#include "ForwardList/ForwardList.hpp"
#include "Person.hpp"
#include "Vector/Vector.hpp"

namespace Person {

using Person::Client;
using std::pair;
using Utils::ForwardList;
using Utils::Vector;

class ClientDict {
  template <bool IsConst> class ClientIterator;

public:
  // Typedefs
  using key_type = string;
  using mapped_type = Client;
  using value_type = pair<const key_type, Client>;
  using size_type = size_t;

  using reference = value_type &;
  using const_reference = const value_type &;
  using iterator = ClientIterator<false>;
  using const_iterator = ClientIterator<true>;

  ClientDict() = default;

  // Capacity
  [[nodiscard]] bool empty() const;
  [[nodiscard]] size_type size() const;

  // Modifiers
  void clear();
  void addClient(const string &name);
  bool removeClient(const string &name);

  // Lookup
  reference at(const string &name);
  [[nodiscard]] const_reference at(const string &name) const;

  reference operator[](const string &name);

private:
  Vector<ForwardList<pair<string, Client>>> m_clients;
  size_type m_bucket_size{};
  size_type m_total_elements{};
  float m_load_factor{};

  [[nodiscard]] size_t hash(const string &key) const;

  iterator find(const string &key);
  [[nodiscard]] const_iterator find(const string &key) const;

  [[nodiscard]] bool contains(const string &key) const;
};

} // namespace Person

#endif
