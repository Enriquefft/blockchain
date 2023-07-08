#ifndef RED_BLACK_TREE_H
#define RED_BLACK_TREE_H

#include <iomanip>
#include <iostream>
#include <memory>

enum RBColor { RED, BLACK }; // define colors for nodes

template <class T> class RedBlackTree {
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

private:
  std::shared_ptr<Node> root;
  size_t node_count{};

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

    std::shared_ptr<Node> parent;

    explicit Node(const_reference _value)
        : value(_value), left(nullptr), right(nullptr), parent(nullptr) {}
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

  // here e.g., begin(), end(), rbegin(), rend(), size(), empty()...
};

template <class T> RedBlackTree<T>::RedBlackTree() : root(nullptr) {}

template <class T> void RedBlackTree<T>::insert(const_reference value) {
  // // Step 1: Regular Binary Search Tree  insertion
  std::shared_ptr<Node> new_node(new Node(value));

  if (root == nullptr) {
    root = new_node;
  } else {
    std::shared_ptr<Node> x = root;
    std::shared_ptr<Node> y;
    while (x != nullptr) {
      y = x;
      if (new_node->value < x->value) {
        x = x->left;
      } else {
        x = x->right;
      }
    }

    // y is parent of x
    new_node->parent = y;
    if (new_node->value < y->value) {
      y->left = new_node;
    } else {
      y->right = new_node;
    }
  }

  // Step 2: Fix Red Black Tree properties if violated
  while (new_node != root && new_node->parent->color == RED) {

    // Define uncle and grandparent for restructure or recoloring
    std::shared_ptr<Node> uncle;
    std::shared_ptr<Node> grand_parent;
    grand_parent = new_node->parent->parent;

    // If parent of node is left child of grandparent
    if (new_node->parent == grand_parent->left) {

      // Uncle is right child of grandparent
      uncle = grand_parent->right;

      // Case 1: Both uncle and parent are RED
      if (uncle != nullptr && uncle->color == RED) {
        grand_parent->color = RED;
        new_node->parent->color = BLACK;
        uncle->color = BLACK;
        new_node = grand_parent;
      } else {

        // Cases 2 & 3: Parent is RED and uncle is BLACK
        if (new_node == new_node->parent->right) {
          new_node = new_node->parent;
          rotateLeft(new_node);
        }
        // Recoloring and then rotate grandparent
        new_node->parent->color = BLACK;
        grand_parent->color = RED;
        rotateRight(grand_parent);
      }
    }

    // Mirror case where parent of node is right child of grandparent
    else {

      // Uncle is left child of grandparent
      uncle = grand_parent->left;

      // Case 1: Both uncle and parent are RED
      if (uncle != nullptr && uncle->color == RED) {
        grand_parent->color = RED;
        new_node->parent->color = BLACK;
        uncle->color = BLACK;
        new_node = grand_parent;
      } else {
        // Cases 2 & 3: Parent is RED and uncle is BLACK
        if (new_node == new_node->parent->left) {
          new_node = new_node->parent;
          rotateRight(new_node);
        }
        // Recoloring and then left rotate grandparent
        new_node->parent->color = BLACK;
        grand_parent->color = RED;
        rotateLeft(grand_parent);
      }
    }
  }

  root->color = BLACK;

  node_count++;
}

template <class T> void RedBlackTree<T>::erase(const_reference value) {
  std::shared_ptr<Node> node_to_delete = search(root, value);
  if (node_to_delete == nullptr) {
    return;
  }
  deleteNode(node_to_delete);
}

template <class T>
auto RedBlackTree<T>::search(std::shared_ptr<Node> node,
                             const_reference value) const
    -> std::shared_ptr<Node> {
  while (node != nullptr && value != node->value) {
    if (value < node->value) {
      node = node->left;
    } else {
      node = node->right;
    }
  }
  return node;
}

template <class T>
void RedBlackTree<T>::deleteNode(std::shared_ptr<Node> node) {
  std::shared_ptr<Node> y = node;
  std::shared_ptr<Node> x;
  RBColor original_y_color = y->color;

  if (node->left == nullptr) {
    x = node->right;
    transplant(node, node->right);
  } else if (node->right == nullptr) {
    x = node->left;
    transplant(node, node->left);
  } else {
    y = minimum(node->right);
    original_y_color = y->color;
    x = y->right;
    if (y->parent == node) {
      x->parent = y;
    } else {
      transplant(y, y->right);
      y->right = node->right;
      y->right->parent = y;
    }
    transplant(node, y);
    y->left = node->left;
    y->left->parent = y;
    y->color = node->color;
  }

  if (original_y_color == BLACK) {
    deleteFixup(x);
  }
}

template <class T>
void RedBlackTree<T>::transplant(std::shared_ptr<Node> u,
                                 std::shared_ptr<Node> v) {
  if (u->parent == nullptr) {
    root = v;
  } else if (u == u->parent->left) {
    u->parent->left = v;
  } else {
    u->parent->right = v;
  }
  v->parent = u->parent;
}

template <class T> void RedBlackTree<T>::deleteFixup(std::shared_ptr<Node> x) {
  while (x != root && x->color == BLACK) {
    if (x == x->parent->left) {
      std::shared_ptr<Node> w = x->parent->right;
      if (w->color == RED) {
        w->color = BLACK;
        x->parent->color = RED;
        rotateLeft(x->parent);
        w = x->parent->right;
      }
      if (w->left->color == BLACK && w->right->color == BLACK) {
        w->color = RED;
        x = x->parent;
      } else {
        if (w->right->color == BLACK) {
          w->left->color = BLACK;
          w->color = RED;
          rotateRight(w);
          w = x->parent->right;
        }
        w->color = x->parent->color;
        x->parent->color = BLACK;
        w->right->color = BLACK;
        rotateLeft(x->parent);
        x = root;
      }
    } else {
      // Mirror case
      // ... similar code as above, but with 'right' and 'left' swapped
    }
  }
  x->color = BLACK;
}

template <class T>
auto RedBlackTree<T>::search(const_reference value) -> reference {
  std::shared_ptr<Node> node = search(root, value);
  if (node == nullptr) {
    throw std::runtime_error("Value not found in tree");
  }
  return node->value;
}

template <class T>
auto RedBlackTree<T>::search(const_reference value) const -> const_reference {
  std::shared_ptr<Node> node = search(root, value);
  if (node == nullptr) {
    throw std::runtime_error("Value not found in tree");
  }
  return node->value;
}

template <class T> void RedBlackTree<T>::rotateLeft(std::shared_ptr<Node> &x) {
  std::shared_ptr<Node> y = x->right;
  x->right = y->left;
  if (y->left != nullptr) {
    y->left->parent = x;
  }
  y->parent = x->parent;
  if (x->parent == nullptr) {
    root = y;
  } else if (x == x->parent->left) {
    x->parent->left = y;
  } else {
    x->parent->right = y;
  }
  y->left = x;
  x->parent = y;
}

template <class T> void RedBlackTree<T>::rotateRight(std::shared_ptr<Node> &y) {
  std::shared_ptr<Node> x = y->left;
  y->left = x->right;
  if (x->right != nullptr)
    x->right->parent = y;
  x->parent = y->parent;
  if (y->parent == nullptr)
    root = x;
  else if (y == y->parent->left)
    y->parent->left = x;
  else
    y->parent->right = x;
  x->right = y;
  y->parent = x;
}

template <class T>
template <bool isConst>
RedBlackTree<T>::_iterator<isConst>::_iterator() : ptr(nullptr) {
  // Initializing an iterator with null pointer.
}

template <class T>
template <bool isConst>
auto RedBlackTree<T>::_iterator<isConst>::operator*() -> node_reference {
  return ptr->value;
}

template <class T>
template <bool isConst>
auto RedBlackTree<T>::_iterator<isConst>::operator->() -> node_value_type * {
  return &(ptr->value);
}

template <class T>
template <bool isConst>
auto RedBlackTree<T>::_iterator<isConst>::operator++() -> _iterator<isConst> & {
  // pre increment operation
  // Depending on the tree shape, you might need to go up then down.
}

template <class T>
template <bool isConst>
auto RedBlackTree<T>::_iterator<isConst>::operator++(int)
    -> _iterator<isConst> {
  // post increment - uses pre increment internally.
}

template <class T>
template <bool isConst>
bool RedBlackTree<T>::_iterator<isConst>::operator==(
    const RedBlackTree<T>::_iterator<isConst> &other) {
  return ptr == other.ptr;
}

template <class T>
template <bool isConst>
bool RedBlackTree<T>::_iterator<isConst>::operator!=(
    const RedBlackTree<T>::_iterator<isConst> &other) {
  return ptr != other.ptr;
}

template <class T>
void RedBlackTree<T>::prettyPrint(std::shared_ptr<Node> node, int indent) {
  if (node != nullptr) {
    if (node->right) {
      prettyPrint(node->right, indent + 4);
    }
    if (indent) {
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
