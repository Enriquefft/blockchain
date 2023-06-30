#ifndef RADIX_TRIE_HPP
#define RADIX_TRIE_HPP

#include "Utils/ForwardList/ForwardList.hpp"
#include <set>
#include <string>
#include <utility>

namespace Utils {

using std::string;

class RadixTrie {

public:
  void insert(const string &content) { m_root.insert(content); }
  void remove(const string &content);
  [[nodiscard]] std::set<string> getElements() const;

  [[nodiscard]] bool search(const string &content) const;

  [[nodiscard]] string toString(const string &delimiter = ", ") const;

private:
  class Node {
    // public:
    friend class RadixTrie;

  public:
    Node() = default;
    explicit Node(string content, bool _isWord = false)
        : m_content{std::move(content)}, isWord{_isWord} {}

    void insert(const string &content);
    void remove(const string &content);
    // [[nodiscard]] std::set<string> getElements() const;
    [[nodiscard]] bool search(const string &content) const;
    [[nodiscard]] bool operator!=(const Node &other) const;

  private:
    string m_content;
    ForwardList<Node> m_children;

    bool isWord{false};

    void splitNode(std::size_t idx);
    void getElementsRecursive(std::string &currentWord,
                              std::set<std::string> &words) const;
  };
  Node m_root;
};

extern template class ForwardList<RadixTrie::Node>;
extern template class ForwardList<RadixTrie::Node>::fl_iterator<true>;
extern template class ForwardList<RadixTrie::Node>::fl_iterator<false>;

} // namespace Utils

#endif // !RADIX_TRIE_HPP
