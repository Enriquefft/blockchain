#include "BlockChain/BlockChain.hpp"
#include <chrono>
#include <limits>
#include <openssl/ts.h>
#include <random>
#include <sstream>

// using blockchain::Block;
using blockchain::BlockChain;
using blockchain::sha_256_t;

using std::string;

// O(1)
void BlockChain::addBlock(const Data &data, const sha_256_t &hash,
                          const uint8_t &nonce) {

  if (m_head == nullptr) {
    m_head = new Block(data, nullptr, hash, nonce);
    m_tail = m_head;
    return;
  }
  m_tail->next = new Block(data, m_tail, hash, nonce);
  m_tail = m_tail->next;
}
BlockChain::BlockChain() noexcept
    : m_head(new(std::nothrow) Block()), m_tail(m_head) {}
void BlockChain::addBlock(const Data &data) {

  if (m_head == nullptr) {
    m_head = new Block(data, nullptr);
    m_tail = m_head;
    return;
  }
  m_tail->next = new Block(data, m_tail);
  m_tail = m_tail->next;
}
BlockChain::Block::Block()
    : previous(nullptr),
      data("genesis", "genesis", 0), header{sha_256_t{}, sha_256_t{}, 0} {}
// , nullptr, sha_256_t{}, 0};

BlockChain::Block::Block(Data _data, Block *_previous, const sha_256_t &_hash,
                         const uint8_t &_nonce)
    : previous(_previous), data(std::move(_data)),
      header(_previous->hash(), _hash, _nonce) {}

BlockChain::Block::Block(Data _data, Block *_previous)
    : previous(_previous), data(std::move(_data)), header(_previous->hash()) {

  bool found_hash = false;

  while (!found_hash) {

    for (header.nounce = 0; header.nounce < std::numeric_limits<uint8_t>::max();
         ++header.nounce) {

      if (found_hash) {
        break;
      }

      sha_256_t hash = this->hash();

      // Check if hash fullfills TARGET
      for (uint8_t digit = 0; digit < BlockChain::TARGET; digit++) {
        if (hash[digit] != 0) {
          break;
        }
      }
      header.current_hash = hash;
      found_hash = true;
    }

    // Update timestamp and try again
    data.timestamp = std::chrono::utc_clock::now();
  }
}

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
    if (curr->previous->hash() != curr->header.previous_hash) {
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
  data->amount = std::uniform_int_distribution<uint8_t>{0, 4}(gen);
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

static std::string Sha2String(const sha_256_t &sha) {

  std::stringstream stream;

  stream << std::hex << std::setfill('0');

  for (size_t i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
    stream << std::hex << std::setw(2) << static_cast<int>(sha[i]);
  }
  return stream.str();
}

static sha_256_t Sha256(std::string_view str) {

  // Transorm to bytes (unsigned char)
  std::span<const unsigned char> data(
      std::bit_cast<const unsigned char *>(str.data()), str.size());

  std::array<uint8_t, SHA256_DIGEST_LENGTH> result{};
  SHA256(data.data(), data.size(), result.data());

  return result;
}

sha_256_t BlockChain::Block::hash() {
  return Sha256(Sha2String(header.previous_hash) +              // Previous hash
                std::to_string(static_cast<int>(data.amount)) + // Amount
                std::format("{:%Y%m%d%H%M}", data.timestamp) +  // Time
                data.sender +                                   // Sender
                data.receiver);                                 // Receiver
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
