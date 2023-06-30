#include "RadixTrie/RadixTrie.hpp"
#include <algorithm>
#include <cstdio>
#include <queue>
#include <stack>

namespace Utils {

string RadixTrie::toString(const string &delimiter) const {
  auto elems = getElements();
  string result;

  for (const auto &elem : elems) {
    result += elem + delimiter;
  }
  return result;
}

void RadixTrie::Node::insert(const std::string &content) {

  for (ulong i = 0; i < m_content.size(); i++) {

    if (i >= content.size()) {
      // m_content includes content, splitting
      splitNode(i);
      break;
    }

    if (content.at(i) != m_content.at(i)) {
      // Difer in [i], splitting
      splitNode(i);
      break;
    }
  }

  if (content == m_content) {
    isWord = true;
    return;
  }

  // Remove m_content from content
  string new_content = content.substr(m_content.size());

  auto next_child = std::find_if(
      m_children.begin(), m_children.end(), [&new_content](auto &_child) {
        return _child.m_content.at(0) == new_content.at(0);
      });

  if (next_child == m_children.end()) {
    m_children.emplaceFront(new_content, true);
    // m_children.emplaceFront(new_content, true);
  } else {
    next_child->insert(new_content);
  }
}

bool RadixTrie::search(const string &content) const {

  return std::any_of(
      m_root.m_children.begin(), m_root.m_children.end(),
      [&content](const auto &child) { return child.search(content); });
}

bool RadixTrie::Node::search(const string &content) const {

  if (m_content == content) {
    return isWord;
  }

  auto new_str = content.substr(m_content.size());
  for (const auto &child : m_children) {

    if (new_str.find(child.m_content) == 0) {
      return child.search(new_str);
    }
  }

  return false;
}

void RadixTrie::remove(const string &content) {
  for (auto &child : m_root.m_children) {
    child.remove(content);
  }
}

void RadixTrie::Node::remove(const string &content) {

  if (m_content == content) {
    isWord = false;
    return;
  }

  string new_content = content.substr(m_content.size());

  for (auto it = m_children.begin(); it != m_children.end(); ++it) {

    if (new_content.find(it->m_content) == 0) {

      it->remove(new_content);

      if (it->m_content.empty() && !it->isWord) {
        m_children.eraseAfter(it);
      }

      return;
    }
  }
}

void RadixTrie::Node::getElementsRecursive(std::string &currentWord,
                                           std::set<std::string> &words) const {
  currentWord += m_content;

  if (isWord) {
    words.insert(currentWord);
  }

  for (const auto &child : m_children) {
    child.getElementsRecursive(currentWord, words);
  }

  currentWord.resize(currentWord.size() - m_content.size());
}

std::set<std::string> RadixTrie::getElements() const {
  std::set<std::string> words;
  std::string currentWord;
  m_root.getElementsRecursive(currentWord, words);
  return words;
}

bool RadixTrie::Node::operator!=(const Node &other) const {
  auto currChild = m_children.begin();

  if (other.m_content != this->m_content) {
    return true;
  }

  for (const auto &child : m_children) {

    if (child != *currChild) {
      return true;
    }

    currChild++;
  }
  return false;
}
void RadixTrie::Node::splitNode(std::size_t idx) {

  if (idx == 0) {
    return;
  }

  // Second part
  Node new_node(m_content.substr(idx), this->isWord);

  // First part
  m_content.resize(idx);
  isWord = false;

  new_node.m_children = m_children;

  m_children = {new_node};
}

} // namespace Utils
