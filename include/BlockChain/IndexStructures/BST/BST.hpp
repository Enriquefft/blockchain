#ifndef IS_BST_HPP
#define IS_BST_HPP

#include "Data.hpp"
#include "Vector/Vector.hpp"
#include "gsl.hpp"

namespace IndexStructure {

using Utils::Vector;

template <CompareType compare_t> class BST {
public:
  using size_type = std::size_t;
  using index_type = getType<compare_t>;
  using value_type = const Data *;

  BST();
  ~BST();

  BST(BST &&) noexcept;
  BST &operator=(BST &&) noexcept;
  BST(const BST &) = delete;
  BST &operator=(const BST &) = delete;

  void insert(value_type data);
  [[nodiscard]] bool search(const index_type &value) const;

  [[nodiscard]] Vector<value_type>
  rangeSearch(const index_type &minValue, const index_type &maxValue) const;

  [[nodiscard]] Vector<value_type> getElements() const;

  [[nodiscard]] size_type height() const;
  [[nodiscard]] size_type size() const;
  void clear();
  [[nodiscard]] bool empty() const;

private:
  class Node {
    friend class BST;

  private:
    value_type m_data = nullptr;
    gsl::owner<Node *> m_left = nullptr;
    gsl::owner<Node *> m_right = nullptr;

    explicit Node(value_type data);
    ~Node();
    Node(const Node &other);
    Node(Node &&other) noexcept;
    Node &operator=(const Node &other);
    Node &operator=(Node &&other) noexcept;

    void insert(value_type data);
    [[nodiscard]] bool search(const index_type &value) const;

    void rangeSearch(const index_type &minValue, const index_type &maxValue,
                     Vector<value_type> &vec) const;

    void getElements(Vector<value_type> &vec) const;

    [[nodiscard]] size_type height() const;
    [[nodiscard]] size_type size() const;
    void clear();
    [[nodiscard]] bool empty() const;
  };

  gsl::owner<Node *> root;
  inline static constexpr Data::Compare<compare_t> COMPARE{};
};

extern template class BST<CompareType::MONEY_DESCENDING>;
extern template class BST<CompareType::MONEY_ASCENDING>;
extern template class BST<CompareType::TIME_DESCENDING>;
extern template class BST<CompareType::TIME_ASCENDING>;
extern template class BST<CompareType::SENDER_NAME_DESCENDING>;
extern template class BST<CompareType::SENDER_NAME_ASCENDING>;
extern template class BST<CompareType::RECEIVER_NAME_DESCENDING>;
extern template class BST<CompareType::RECEIVER_NAME_ASCENDING>;

} // namespace IndexStructure

#endif // !IS_BST_HPP
