#ifndef BLOCK_CHAIN_HPP
#define BLOCK_CHAIN_HPP

#include "Array/Array.hpp"
#include "Data.hpp"
#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

#include "gsl.hpp"

namespace blockchain {

using std::string;

using sha_256_t = Utils::Array<uint8_t, 32 /* SHA256_DIGEST_LENGTH*/>;

struct BlockChainInfo {
  // Last update
  // number of blocks
  std::chrono::time_point<std::chrono::utc_clock> last_entry;
  size_t block_count = 0;
};

class BlockChain {
  class App;
  friend class App;

  inline static const uint8_t TARGET = 2;
  inline static const uint8_t BLOCK_SIZE = 4;

public:
  // Typedefs
  using value_type = Data;
  using reference = value_type &;
  using const_reference = const value_type &;
  using pointer = value_type *;

  using size_type = size_t;

  // void addBlock(const Data &data, const sha_256_t &hash, const uint8_t
  // &nonce);
  void addBlock(const Data &data);

  [[nodiscard]] reference getLastBlock();
  [[nodiscard]] const_reference getLastBlock() const;

  [[nodiscard]] bool isConsistent() const;

  void randomInyection();
  void recalculateHashes();

  [[nodiscard]] size_type size() const;

  BlockChain() noexcept;

  ~BlockChain() { delete m_head; }

  // Disable move/copy semantics
  BlockChain(const BlockChain &) = delete;
  BlockChain(BlockChain &&) = delete;
  BlockChain &operator=(const BlockChain &) = delete;
  BlockChain &operator=(BlockChain &&) = delete;

private:
  class Block {

    struct Header {
      sha_256_t previous_hash;
      sha_256_t current_hash{};
      uint8_t nounce = 0;
      explicit Header(const sha_256_t &prevHahs) : previous_hash(prevHahs) {}
      explicit Header(const sha_256_t &prevHahs, const sha_256_t &hash,
                      const uint8_t &nonce)
          : previous_hash(prevHahs), current_hash(hash), nounce(nonce) {}
    };

    friend class BlockChain;

    ~Block() { delete next; }
    Block(); // Genesis

  public:
    // Cant copy or move blocks
    Block(const Block &) = delete;
    Block(Block &&) = delete;
    Block &operator=(const Block &) = delete;
    Block &operator=(Block &&) = delete;

    Block(Data _data, Block *_previous);
    Block(Data _data, Block *_previous, const sha_256_t &_hash,
          const uint8_t &_nonce);
    [[nodiscard]] const Utils::Array<const Data &, BLOCK_SIZE> &getData() const;
    [[nodiscard]] Utils::Array<Data &, BLOCK_SIZE> &getData();

    [[nodiscard]] reference getLast();
    [[nodiscard]] const_reference getLast() const;

    void addData(const Data &data);
    [[nodiscard]] bool isFull() const;

  private:
    sha_256_t hash();

    Block *previous;
    gsl::owner<Block *> next = nullptr;

    // Data data;
    Utils::Array<std::shared_ptr<Data>, BLOCK_SIZE> m_data;
    Header header;
  };

  // Block *m_head = nullptr;
  gsl::owner<Block *> m_head = nullptr;
  Block *m_tail = nullptr;

public:
  template <bool IsConst> class BlockChainIterator {
  public:
    using value_type = std::conditional_t<IsConst, const Data, Data>;
    using Block_ = std::conditional_t<IsConst, const Block, Block>;

    using reference = value_type &;
    using pointer = value_type *;
    using difference_type = std::ptrdiff_t;

    explicit BlockChainIterator(Block *ptr) : m_curr(ptr) {}

    BlockChainIterator() = default;

    // Comparison
    // bool operator==(const BlockChainIterator &rhs) const;
    bool operator==(const BlockChainIterator &rhs) const;
    bool operator!=(const BlockChainIterator &rhs) const;

    // Accessors
    reference operator*();
    pointer operator->();

    // Increment
    BlockChainIterator &operator++();
    BlockChainIterator operator++(int);

    // Decrement
    BlockChainIterator &operator--();
    BlockChainIterator operator--(int);

  private:
    Block_ *m_curr = nullptr;
    size_t m_index = 0;
  };

  using iterator = BlockChainIterator<false>;
  using const_iterator = BlockChainIterator<true>;
  // Iterators
  [[nodiscard]] iterator begin();
  [[nodiscard]] const_iterator begin() const;

  [[nodiscard]] iterator end();
  [[nodiscard]] const_iterator end() const;
};

extern template class BlockChain::BlockChainIterator<true>;
extern template class BlockChain::BlockChainIterator<false>;

} // namespace blockchain

#endif // !BLOCK_CHAIN_HPP
