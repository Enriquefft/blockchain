#ifndef BLOCK_CHAIN_HPP
#define BLOCK_CHAIN_HPP

#include "Data.hpp"
#include <iostream>
#include <string>
#include <utility>

#include "Utils/gsl.hpp"

namespace blockchain {

// using std::shared_ptr;
using std::string;

class BlockChain {

  // template <bool IsConst> class BlockChainIterator;

public:
  // Typedefs
  using value_type = Data;
  using reference = value_type &;
  using const_reference = const value_type &;
  using pointer = value_type *;

  using size_type = size_t;

  void addBlock(const Data &data);

  [[nodiscard]] reference getLastBlock();
  [[nodiscard]] const_reference getLastBlock() const;

  [[nodiscard]] bool isConsistent() const;

  void randomInyection();
  [[nodiscard]] size_type size() const;

  BlockChain() = default;

  ~BlockChain() { delete m_head; }

  // Disable move/copy semantics
  BlockChain(const BlockChain &) = delete;
  BlockChain(BlockChain &&) = delete;
  BlockChain &operator=(const BlockChain &) = delete;
  BlockChain &operator=(BlockChain &&) = delete;

private:
  class Block {

    friend class BlockChain;

    ~Block() {

      // Shoudl the destructor be recursive?
      // as its inline, the overhead should be to big, right?
      delete next;
    }

  public:
    // Cant copy or move blocks
    Block(const Block &) = delete;
    Block(Block &&) = delete;
    Block &operator=(const Block &) = delete;
    Block &operator=(Block &&) = delete;

    Block(Data _data, Block *_previous);
    [[nodiscard]] const Data &getData() const;
    [[nodiscard]] Data &getData();

  private:
    string hash();

    Data data;
    string previous_hash;

    gsl::owner<Block *> next = nullptr;
    Block *previous;
  };

  // Block *m_head = nullptr;
  gsl::owner<Block *> m_head = nullptr;
  Block *m_tail = nullptr;

public:
  template <bool IsConst> class BlockChainIterator {
  public:
    // Typedefs
    // using Block = BlockChain::Block;
    using value_type = std::conditional_t<IsConst, const Data, Data>;
    using Block_ = std::conditional_t<IsConst, const Block, Block>;

    using reference = value_type &;
    using pointer = value_type *;
    using difference_type = std::ptrdiff_t;
    // using iterator = BlockChainIterator<IsConst>;

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
