#include "BlockChain.hpp"
#include <chrono>
#include <limits>
#include <openssl/ts.h>
#include <random>
#include <sstream>
#include <string>

constexpr bool POW = false;

// using blockchain::Block;
using blockchain::BlockChain;
using blockchain::sha_256_t;

using std::string;
using Utils::Array;

namespace { // Anonymous namespace

std::string Sha2String(const sha_256_t &sha) {

  std::stringstream stream;

  stream << std::hex << std::setfill('0');

  for (size_t i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
    stream << std::hex << std::setw(2) << static_cast<int>(sha[i]);
  }
  return stream.str();
}

sha_256_t Sha256(std::string_view str) {

  // Transorm to bytes (unsigned char)
  std::span<const unsigned char> data(
      std::bit_cast<const unsigned char *>(str.data()), str.size());

  Array<uint8_t, SHA256_DIGEST_LENGTH> result{};
  SHA256(data.data(), data.size(), result.data());

  return result;
}

} // namespace

sha_256_t BlockChain::Block::hash() {

  string amount;
  string time;
  string sender;
  string receiver;

  for (size_type i = 0; i < BLOCK_SIZE; i++) {
    if (m_data[i]) {
      amount += std::to_string(static_cast<int>(m_data[i]->amount));
      time += std::format("{:%Y%m%d%H%M}", m_data[i]->timestamp);
      sender += m_data[i]->sender;
      receiver += m_data[i]->receiver;
    } else {
      break;
    }
  }

  return Sha256(Sha2String(header.previous_hash) +       // Previous hash
                std::to_string(header.nounce) + amount + // Amount
                time +                                   // Timestamp
                sender +                                 // Sender
                receiver);                               // Receiver
}

void BlockChain::Block::addData(const Data &data) {
  if (isFull()) {
    return;
  }

  // find first empty slot
  for (size_type i = 0; i < BLOCK_SIZE; i++) {
    if (!m_data[i]) {
      m_data[i] = std::make_shared<Data>(data);
      return;
    }
  }
}

bool BlockChain::Block::isFull() const {
  return m_data[BLOCK_SIZE - 1] != nullptr;
}

// O(1)
// void BlockChain::addBlock(const Data &data, const sha_256_t &hash,
//                           const uint8_t &nonce) {
//
//   if (m_head == nullptr) {
//     m_head = new Block(data, nullptr, hash, nonce);
//     m_tail = m_head;
//     return;
//   }
//
//   if (m_tail->isFull()) {
//     m_tail->next = new Block(data, m_tail, hash, nonce);
//     m_tail = m_tail->next;
//     return;
//   }
//
//   m_tail->next = new Block(data, m_tail, hash, nonce);
// }

BlockChain::BlockChain() noexcept
    : m_head(new(std::nothrow) Block()), m_tail(m_head) {}

void BlockChain::addBlock(const Data &data) {

  if (m_head == nullptr) {
    m_head = new Block(data, nullptr);
    m_tail = m_head;
    return;
  }
  if (m_tail->isFull()) {
    m_tail->next = new Block(data, m_tail);
    m_tail = m_tail->next;
    return;
  }
  m_tail->addData(data);
}

BlockChain::Block::Block()
    : previous(nullptr), m_data{std::make_shared<Data>("genesis", "genesis",
                                                       0)},
      header{sha_256_t{}, sha_256_t{}, 0} {}

BlockChain::Block::Block(Data _data, Block *_previous, const sha_256_t &_hash,
                         const uint8_t &_nonce)
    : previous(_previous), m_data{std::make_shared<Data>(std::move(_data))},
      header(_previous->hash(), _hash, _nonce) {}

BlockChain::Block::Block(Data _data, Block *_previous)
    : previous(_previous), m_data{std::make_shared<Data>(std::move(_data))},
      header(_previous->hash()) {

  bool found_hash = false;
  sha_256_t hash;

  do {

    for (header.nounce = 0; header.nounce < std::numeric_limits<uint8_t>::max();
         ++header.nounce) {

      // Check if hash fullfills TARGET
      hash = this->hash();
      string hex_hash = Sha2String(hash);

      found_hash = true;
      for (uint8_t digit = 0; digit < BlockChain::TARGET; digit++) {

        if (!POW) {
          break;
        }

        // Endiannes can be a bitch
        if (hex_hash[digit] != '0') {
          found_hash = false;
          break;
        }
      }

      if (found_hash || !POW) {
        header.current_hash = hash;
        break;
      }
    }

    // Update timestamp and try again
    if (!found_hash) {
      m_data[0]->timestamp = std::chrono::utc_clock::now();
    }
  } while (!found_hash);
}

BlockChain::reference BlockChain::Block::getLast() {
  for (size_type i = 0; i < BLOCK_SIZE; i++) {
    if (!m_data[i]) {
      return *m_data[i - 1];
    }
  }
  return *m_data[BLOCK_SIZE];
}
BlockChain::const_reference BlockChain::Block::getLast() const {
  for (size_type i = 0; i < BLOCK_SIZE; i++) {
    if (!m_data[i]) {
      return *m_data[i - 1];
    }
  }
  return *m_data[BLOCK_SIZE];
}

BlockChain::reference BlockChain::getLastBlock() { return m_tail->getLast(); }

BlockChain::const_reference BlockChain::getLastBlock() const {
  return m_tail->getLast();
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

void BlockChain::recalculateHashes() {
  // if (m_head == nullptr) {
  //   return true;
  // }
  //
  // auto *curr = m_head->next;
  // auto c = 1;
  //
  // for (; curr != nullptr; curr = curr->next) {
  //   if (curr->previous->hash() != curr->header.previous_hash) {
  //     std::cout << "block " << c << " is not consistent\n";
  //     return false;
  //   }
  //   std::cout << "block " << c << " is consistent\n";
  //   c++;
  // }
  //
  // return true;
}

// n
BlockChain::size_type BlockChain::size() const {

  size_type size = 0;

  for (Block *curr = m_head; curr != nullptr; curr = curr->next) {
    size++;
  }

  return size;
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

template <bool IsConst>
bool BlockChain::BlockChainIterator<IsConst>::operator==(
    const BlockChainIterator &rhs) const {

  return m_curr == rhs.m_curr && m_index == rhs.m_index;
}

template <bool IsConst>
bool BlockChain::BlockChainIterator<IsConst>::operator!=(
    const BlockChainIterator &rhs) const {
  return !(*this == rhs);
}

template <bool IsConst>
auto BlockChain::BlockChainIterator<IsConst>::operator*() -> reference {
  return *m_curr->m_data[m_index];
}

template <bool IsConst>
auto BlockChain::BlockChainIterator<IsConst>::operator->() -> pointer {
  if (m_curr == nullptr) {
    throw std::out_of_range("iterator out of range");
  }
  return m_curr->m_data[m_index].get();
}

template <bool IsConst>
BlockChain::BlockChainIterator<IsConst> &
BlockChain::BlockChainIterator<IsConst>::operator++() {

  if (m_index < BLOCK_SIZE - 1) {
    m_index++;
  } else {
    m_index = 0;
    m_curr = m_curr->next;
  }

  if (!m_curr->m_data[m_index]) { // End of chain, invalid iterator
    m_curr = nullptr;
    m_index = 0;
  }

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
