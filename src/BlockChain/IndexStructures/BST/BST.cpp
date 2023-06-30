#include "IndexStructures/BST/BST.hpp"
#include "Data.hpp"

namespace IndexStructure {

template <CompareType comp> BST<comp>::BST() : root(nullptr) {}
template <CompareType comp> BST<comp>::~BST() { delete root; }

// Move
template <CompareType comp>
BST<comp>::BST(BST<comp> &&other) noexcept : root(other.root) {
  other.root = nullptr;
}

template <CompareType comp>
BST<comp> &BST<comp>::operator=(BST<comp> &&other) noexcept {

  if (this != &other) {
    delete root;
    root = other.root;
    other.root = nullptr;
  }
  return *this;
}

template <CompareType comp> void BST<comp>::insert(value_type data) {
  if (root == nullptr) {
    root = new Node(data);
  } else {
    root->insert(data);
  }
}
template <CompareType comp>
bool BST<comp>::search(const index_type &value) const {
  if (root == nullptr) {
    return false;
  }
  return root->search(value);
}

template <CompareType comp>
auto BST<comp>::rangeSearch(const index_type &minValue,
                            const index_type &maxValue) const
    -> Utils::Vector<value_type> {
  Utils::Vector<value_type> result;
  if (root == nullptr) {
    return result;
  }
  root->rangeSearch(minValue, maxValue, result);
  return result;
}

template <CompareType comp>
auto BST<comp>::getElements() const -> Vector<value_type> {
  Vector<value_type> result;
  if (root == nullptr) {
    return result;
  }
  root->getElements(result);
  return result;
}

template <CompareType comp> auto BST<comp>::height() const -> size_type {
  if (root == nullptr) {
    return 0;
  }
  return root->height();
}

template <CompareType comp> auto BST<comp>::size() const -> size_type {
  if (root == nullptr) {
    return 0;
  }
  return root->size();
}

template <CompareType comp> void BST<comp>::clear() {
  delete root;
  root = static_cast<gsl::owner<Node *>>(nullptr);
}

template <CompareType comp> auto BST<comp>::empty() const -> bool {
  return root == nullptr;
}

/*******
  NODE
 *******/

template <CompareType comp>
BST<comp>::Node::Node(value_type data)
    : m_data(data), m_left(nullptr), m_right(nullptr) {}

template <CompareType comp> BST<comp>::Node::~Node() {
  delete m_left;
  delete m_right;
}

template <CompareType comp>
BST<comp>::Node::Node(const Node &other)
    : m_data(other.m_data), m_left(nullptr), m_right(nullptr) {
  if (other.m_left != nullptr) {
    m_left = new Node(*other.m_left);
  }
  if (other.m_right != nullptr) {
    m_right = new Node(*other.m_right);
  }
}

template <CompareType comp>
BST<comp>::Node::Node(Node &&other) noexcept
    : m_data(other.m_data), m_left(other.m_left), m_right(other.m_right) {
  other.m_left = nullptr;
  other.m_right = nullptr;
}

template <CompareType comp>
BST<comp>::Node &BST<comp>::Node::operator=(const Node &other) {
  if (this != &other) {
    delete m_left;
    delete m_right;
    m_data = other.m_data;
    if (other.m_left != nullptr) {
      m_left = new Node(*other.m_left);
    }
    if (other.m_right != nullptr) {
      m_right = new Node(*other.m_right);
    }
  }
  return *this;
}

template <CompareType comp>
BST<comp>::Node &BST<comp>::Node::operator=(Node &&other) noexcept {
  if (this != &other) {
    delete m_left;
    delete m_right;
    m_data = other.m_data;
    m_left = other.m_left;
    m_right = other.m_right;
    other.m_left = nullptr;
    other.m_right = nullptr;
  }
  return *this;
}

template <CompareType comp> void BST<comp>::Node::insert(value_type data) {
  if (COMPARE(*data, *m_data)) {
    if (m_left == nullptr) {
      m_left = new Node(data);
    } else {
      m_left->insert(data);
    }
  } else {
    if (m_right == nullptr) {
      m_right = new Node(data);
    } else {
      m_right->insert(data);
    }
  }
}

template <CompareType comp>
bool BST<comp>::Node::search(const index_type &value) const {
  if (m_data->get<comp>() == value) {
    return true;
  }
  if (value < m_data->get<comp>() && m_left != nullptr) {
    return m_left->search(value);
  }
  if (value > m_data->get<comp>() && m_right != nullptr) {
    return m_right->search(value);
  }
  return false;
}

template <CompareType comp>
void BST<comp>::Node::rangeSearch(const index_type &minValue,
                                  const index_type &maxValue,
                                  Vector<value_type> &vec) const {

  if (m_data->get<comp>() < minValue && m_data->get<comp>() > maxValue) {
    return;
  }

  vec.pushBack(m_data);

  if (m_left != nullptr) {
    m_left->rangeSearch(minValue, maxValue, vec);
  }
  if (m_right != nullptr) {
    m_right->rangeSearch(minValue, maxValue, vec);
  }
}

template <CompareType comp>
void BST<comp>::Node::getElements(Vector<value_type> &vec) const {
  vec.pushBack(m_data);
  if (m_left != nullptr) {
    m_left->getElements(vec);
  }
  if (m_right != nullptr) {
    m_right->getElements(vec);
  }
}

// [[nodiscard]] size_type height() const;
// [[nodiscard]] size_type size() const;
// void clear();
// [[nodiscard]] bool empty() const;

template <CompareType comp> auto BST<comp>::Node::height() const -> size_type {
  throw std::runtime_error("Not implemented");
}
template <CompareType comp> auto BST<comp>::Node::size() const -> size_type {
  throw std::runtime_error("Not implemented");
}
template <CompareType comp> void BST<comp>::Node::clear() {
  throw std::runtime_error("Not implemented");
}
template <CompareType comp> bool BST<comp>::Node::empty() const {
  throw std::runtime_error("Not implemented");
}

// Explicit instantiation
template class BST<CompareType::MONEY_DESCENDING>;
template class BST<CompareType::MONEY_ASCENDING>;
template class BST<CompareType::TIME_DESCENDING>;
template class BST<CompareType::TIME_ASCENDING>;
template class BST<CompareType::SENDER_NAME_DESCENDING>;
template class BST<CompareType::SENDER_NAME_ASCENDING>;
template class BST<CompareType::RECEIVER_NAME_DESCENDING>;
template class BST<CompareType::RECEIVER_NAME_ASCENDING>;

} // namespace IndexStructure
