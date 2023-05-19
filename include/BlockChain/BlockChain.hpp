#ifndef BLOCK_CHAIN_HPP
#define BLOCK_CHAIN_HPP

#include "Data.hpp"

#include <memory>
#include <string>
#include <utility>

namespace blockchain {

using std::shared_ptr;
using std::string;

class BlockChain {

  template <bool IsConst> class BlockChainIterator;

public:
  // Typedefs
  using iterator = BlockChainIterator<false>;
  using const_iterator = BlockChainIterator<true>;
  using value_type = Data;
  using reference = value_type &;
  using const_reference = const value_type &;
  using pointer = value_type *;

  void addBlock(const Data &data);

  [[nodiscard]] reference getLastBlock();
  [[nodiscard]] const_reference getLastBlock() const;

  // Iterators
  [[nodiscard]] iterator begin();
  [[nodiscard]] const_iterator begin() const;

  [[nodiscard]] iterator end();
  [[nodiscard]] const_iterator end() const;

private:
  class Block {
    friend class BlockChain;

  public:
    Block(Data _data, shared_ptr<Block> _previous)
        : data(std::move(_data)), previous(std::move(_previous)) {}
    [[nodiscard]] const Data &getData() const;
    [[nodiscard]] Data &getData();

  private:
    string hash();

    Data data;
    string previous_hash;

    shared_ptr<Block> next = nullptr;
    shared_ptr<Block> previous;
  };

  shared_ptr<Block> m_head = nullptr;
  shared_ptr<Block> m_tail = nullptr;

  template <bool IsConst> class BlockChainIterator {
  public:
    // Typedefs
    // using Block = BlockChain::Block;
    using value_type = std::conditional_t<IsConst, const Data, Data>;
    using Block = std::conditional_t<IsConst, const Block, Block>;

    using reference = value_type &;
    using pointer = value_type *;
    using difference_type = std::ptrdiff_t;
    using iterator = BlockChainIterator<IsConst>;

    explicit BlockChainIterator(const shared_ptr<Block> &ptr) : m_curr(ptr) {}

    BlockChainIterator() = default;

    // Comparison
    bool operator==(const BlockChainIterator &) const;
    bool operator!=(const BlockChainIterator &) const;

    // Accessors
    reference operator*();
    pointer operator->();

    // Increment
    iterator &operator++();
    iterator operator++(int);

    // Decrement
    iterator &operator--();
    iterator operator--(int);

  private:
    shared_ptr<Block> m_curr = nullptr;
  };
};

} // namespace blockchain

#endif // !BLOCK_CHAIN_HPP
