#include "BlockChain/BlockChain.hpp"
#include <chrono>
#include <openssl/ts.h>
#include <random>

// using blockchain::Block;
using blockchain::BlockChain;

using std::string;

// O(1)
void BlockChain::addBlock(const Data &data) {

  if (m_head == nullptr) {
    m_head = new Block(data, nullptr);
    m_tail = m_head;
    return;
  }
  m_tail->next = new Block(data, m_tail);
  m_tail = m_tail->next;
}

BlockChain::Block::Block(Data _data, Block *_previous)
    : data(std::move(_data)),
      previous_hash(_previous != nullptr ? _previous->hash() : ""),
      previous(_previous) {}

BlockChain::reference BlockChain::getLastBlock() { return m_tail->data; }
BlockChain::const_reference BlockChain::getLastBlock() const {
  return m_tail->data;
}

// O(n) + n*hash
bool BlockChain::isConsistent() const {

  if (m_head == nullptr) {
    return true;
  }

  auto *curr = m_head->next;

  auto c = 1;
  for (; curr != nullptr; curr = curr->next) {
    if (curr->previous->hash() != curr->previous_hash) {
      std::cout << "block " << c << " is not consistent\n";
      return false;
    }
    std::cout << "block " << c << " is consistent\n";
    c++;
  }

  return true;
}

// 2n
void BlockChain::randomInyection() {

  std::default_random_engine gen{std::random_device{}()};

  size_type data_idx =
      std::uniform_int_distribution<size_type>{0, this->size() - 1}(gen);

  auto data = begin();
  std::advance(data, data_idx);

  // Randomize all 4 values
  data->amount = io1::Money(std::uniform_int_distribution<int>{0, 100}(gen));
  data->timestamp = std::chrono::utc_clock::now();
  data->sender = "RANDOM_SENDER" + std::to_string(data_idx);
  data->receiver = "RANDOM_RECEIVER" + std::to_string(data_idx);
}

// n
BlockChain::size_type BlockChain::size() const {

  size_type size = 0;

  for (Block *curr = m_head; curr != nullptr; curr = curr->next) {
    size++;
  }

  return size;
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

template <bool IsConst>
bool BlockChain::BlockChainIterator<IsConst>::operator==(
    const BlockChainIterator &rhs) const {
  return m_curr == rhs.m_curr;
}

template <bool IsConst>
bool BlockChain::BlockChainIterator<IsConst>::operator!=(
    const BlockChainIterator &rhs) const {
  return m_curr != rhs.m_curr;
}

template <bool IsConst>
typename BlockChain::BlockChainIterator<IsConst>::reference
BlockChain::BlockChainIterator<IsConst>::operator*() {
  return m_curr->data;
}

template <bool IsConst>
typename BlockChain::BlockChainIterator<IsConst>::pointer
BlockChain::BlockChainIterator<IsConst>::operator->() {
  if (m_curr == nullptr) {
    throw std::out_of_range("iterator out of range");
  }
  return &(m_curr->data);
}

template <bool IsConst>
BlockChain::BlockChainIterator<IsConst> &
BlockChain::BlockChainIterator<IsConst>::operator++() {
  m_curr = m_curr->next;
  return *this;
}

template <bool IsConst>
BlockChain::BlockChainIterator<IsConst>
BlockChain::BlockChainIterator<IsConst>::operator++(int) {
  auto tmp = *this;
  this->operator++();
  return tmp;
}

template <bool IsConst>
BlockChain::BlockChainIterator<IsConst> &
BlockChain::BlockChainIterator<IsConst>::operator--() {
  m_curr = m_curr->previous;
  return *this;
}

template <bool IsConst>
BlockChain::BlockChainIterator<IsConst>
BlockChain::BlockChainIterator<IsConst>::operator--(int) {
  auto tmp = *this;
  this->operator--();
  return tmp;
}

// Explicit instantiation
template class BlockChain::BlockChainIterator<true>;
template class BlockChain::BlockChainIterator<false>;
