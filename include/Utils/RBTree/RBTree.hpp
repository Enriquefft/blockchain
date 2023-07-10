#ifndef RED_BLACK_TREE_H
#define RED_BLACK_TREE_H

#include <iomanip>
#include <iostream>
#include <memory>

enum RBColor { RED, BLACK }; // define colors for nodes

template <class T, class Comparator = std::less<T>> class RedBlackTree {
  // forward declaration of iterator class
  struct Node;
  template <bool isConst = false> class _iterator;

public:
  using iterator = _iterator<false>;
  using const_iterator = _iterator<true>;
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using rvalue_reference = T &&;

  RedBlackTree();

  // Basic operations: insert, delete, search, balancing
  void insert(const_reference value);
  void erase(const_reference value);
  reference search(const_reference value);
  const_reference search(const_reference value) const;

  // Iterators
  iterator begin() { return iterator(minimum(root)); }

  [[nodiscard]] const_iterator begin() const {
    return const_iterator(minimum(root));
  }

  [[nodiscard]] const_iterator cbegin() const {
    return const_iterator(minimum(root));
  }

  iterator end() { return iterator(nullptr); }

  [[nodiscard]] const_iterator end() const { return const_iterator(nullptr); }

  [[nodiscard]] const_iterator cend() const { return const_iterator(nullptr); }

  // Capacity
  [[nodiscard]] bool empty() const { return node_count == 0; }
  [[nodiscard]] size_t size() const { return node_count; }

  void prettyPrint(int indent = 0) { prettyPrint(root, indent); }
  // ~RedBlackTree() { deleteNode(root); }

private:
  std::shared_ptr<Node> root;
  size_t node_count{};
  Comparator comp;

  void rotateLeft(std::shared_ptr<Node> &x);
  void rotateRight(std::shared_ptr<Node> &x);

  void transplant(std::shared_ptr<Node> u, std::shared_ptr<Node> v);

  void deleteFixup(std::shared_ptr<Node> x);
  void deleteNode(std::shared_ptr<Node> node);

  std::shared_ptr<Node> search(std::shared_ptr<Node> node,
                               const_reference value) const;

  std::shared_ptr<Node> minimum(std::shared_ptr<Node> node) const {
    while (node->left != nullptr) {
      node = node->left;
    }
    return node;
  }

  void prettyPrint(std::shared_ptr<Node> node, int indent);

  struct Node {
    value_type value;
    RBColor color{RED};
    std::shared_ptr<Node> left;
    std::shared_ptr<Node> right;

    std::weak_ptr<Node> parent{};

    explicit Node(const_reference _value)
        : value(_value), left(nullptr), right(nullptr) {}
  };

  template <bool isConst> class _iterator {
    friend class RedBlackTree;

    using node_value_type =
        std::conditional_t<isConst, const value_type, value_type>;
    using node_reference = node_value_type &;

    std::shared_ptr<Node> ptr; // Current node pointer

    _iterator();                             // constructor
    node_reference operator*();              // dereference operator
    node_value_type *operator->();           // arrow operator
    _iterator &operator++();                 // pre-increment operator
    _iterator operator++(int);               // post-increment operator
    bool operator==(const _iterator &other); // equality check operator
    bool operator!=(const _iterator &other); // inequality check operator
  };
};

template <class T, class Comparator>
void RedBlackTree<T, Comparator>::prettyPrint(std::shared_ptr<Node> node,
                                              int indent) {
  if (node != nullptr) {
    if (node->right) {
      prettyPrint(node->right, indent + 4);
    }
    if (indent != 0) {
      std::cout << std::setw(indent) << ' ';
    }
    if (node->right) {
      std::cout << " /\n" << std::setw(indent) << ' ';
    }
    std::cout << (node->color == RED ? "*" : "") << node->value << "\n ";
    if (node->left) {
      std::cout << std::setw(indent) << ' ' << " \\\n";
      prettyPrint(node->left, indent + 4);
    }
  }
}

#endif // RED_BLACK_TREE_H
