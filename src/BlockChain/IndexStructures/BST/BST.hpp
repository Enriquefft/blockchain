#ifndef IS_BST_HPP
#define IS_BST_HPP

#include "BlockChain/Data.hpp"
#include <Utils/Vector/Vector.hpp>

template <CompareType comp> class BST {
public:
  using size_type = std::size_t;
  using search_type = getType<comp>;

  BST() { root = nullptr; }
  void insert(Data &data) { root->insert(data); }
  //[[nodiscard]] bool search(const search_type &value) const;

  //[[nodiscard]] /*std::ranges::subrange*/ Utils::Vector<search_type>
  // rangeSearch(const search_type &minValue, const search_type &maxValue)
  // const;

  [[nodiscard]] /*std::ranges::subrange*/ Utils::Vector<Data>
  getElements() const;

  [[nodiscard]] size_type height() const;
  [[nodiscard]] size_type size() const;
  void clear();
  [[nodiscard]] bool empty() const;

  ~BST() {
    if (root) {
      root->killself();
    }
  }

private:
  class Node {
    Data data;
    Node *left;
    Node *right;
    Data::Compare<Data::CompareType::MONEY_ASCENDING> compare;

    friend class BST;
    void killself() {
      if (left == nullptr) {
        left->killself();
      }
      if (right == nullptr) {
        right->killself();
      }
      delete this;
    }
    explicit Node(Data _data, Node *_left = nullptr, Node *_right = nullptr)
        : data(std::move(_data)), left(_left), right(_right) {}
    ~Node() { killself(); }
    void insert(const Data &value) {
      if (!compare(value, data)) {
        if (left == nullptr) {
          left = new Node(value);
        } else {
          left->insert(value);
        }
      } else {
        if (right == nullptr) {
          right = new Node(value);
        } else {
          right->insert(value);
        }
      }
    }
  };

  Node *root;
};

#endif // !IS_BST_HPP
