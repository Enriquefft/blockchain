#include "BlockChain/BlockChain.hpp"
#include <openssl/ts.h>

// using blockchain::Block;
using blockchain::BlockChain;

using std::string;

void BlockChain::addBlock(const Data &data) {

  if (m_head == nullptr) {
    m_head = new Block(data, nullptr);
    m_tail = m_head;
    return;
  }
  m_tail->next = new Block(data, m_tail);
  m_tail = m_tail->next;
}

static string Sha256(std::string_view str) {

  // Transorm to bytes (unsigned char)
  std::span<const unsigned char> data(
      std::bit_cast<const unsigned char *>(str.data()), str.size());

  auto *hash = SHA256(data.data(), data.size(), nullptr);

  std::string result = std::format("{:02x}", hash[0]);
  for (int i = 1; i < SHA256_DIGEST_LENGTH; i++) {
    result += std::format(":{:02x}", hash[i]);
  }

  return result;
}

string BlockChain::Block::hash() {
  return Sha256(previous_hash + std::to_string(data.amount.data()) +
                std::format("{:%Y%m%d%H%M}", data.timestamp) + data.sender +
                data.receiver);
}

////// ITERATOR /////////

BlockChain::iterator BlockChain::begin() {
  return BlockChain::iterator(m_head);
}

BlockChain::iterator BlockChain::end() { return {}; }

BlockChain::const_iterator BlockChain::begin() const {
  return BlockChain::const_iterator(m_head);
}

BlockChain::const_iterator BlockChain::end() const { return {}; }

// template <bool IsConst>
// using iterator_td = typename BlockChain::BlockChainIterator<IsConst>;

// template <bool IsConst>
// bool iterator_td<IsConst>::operator==(const iterator &rhs) const {
//   return m_curr == rhs.m_curr;
// }
//
// template <bool IsConst>
// bool iterator_td<IsConst>::operator!=(const iterator &rhs) const {
//   return m_curr != rhs.m_curr;
// }
//
// template <bool IsConst>
// typename iterator_td<IsConst>::reference iterator_td<IsConst>::operator*() {
//   return m_curr;
// }
//
// template <bool IsConst>
// typename iterator_td<IsConst>::pointer iterator_td<IsConst>::operator->() {
//   return m_curr;
// }
//
// template <bool IsConst>
// typename iterator_td<IsConst>::iterator &iterator_td<IsConst>::operator++() {
//   m_curr = m_curr->next;
//   return *this;
// }
//
// template <bool IsConst>
// typename iterator_td<IsConst>::iterator iterator_td<IsConst>::operator++(int)
// {
//   auto tmp = *this;
//   this->operator++();
//   return tmp;
// }
//
// template <bool IsConst>
// typename iterator_td<IsConst>::iterator &iterator_td<IsConst>::operator--() {
//   m_curr = m_curr->previous;
//   return *this;
// }
//
// template <bool IsConst>
// typename iterator_td<IsConst>::iterator iterator_td<IsConst>::operator--(int)
// {
//   auto tmp = *this;
//   this->operator--();
//   return tmp;
// }
