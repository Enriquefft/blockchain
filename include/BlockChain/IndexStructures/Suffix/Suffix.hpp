#ifndef SUFFIX_TRIE_HPP
#define SUFFIX_TRIE_HPP
#include "ForwardList/ForwardList.hpp"
#include <set>
#include <string>
#include <utility>

namespace Utils {

    using std::string;

    class SuffixTrie : public RadixTrie {

    public:
        void insert(const string& content) { m_root.insert(content); }
        void remove(const string& content) { m_root.remove(content); }
        [[nodiscard]] std::set<string> getElements() const { return m_root.getElements(); }
        [[nodiscard]] bool search(const string& content) const { return m_root.search(content); }
        [[nodiscard]] string toString(const string& delimiter = ", ") const { return m_root.toString(delimiter); }

    private:
        class Node {
            friend class RadixTrie;

        public:
            Node() = default;
            explicit Node(string content, bool _isWord = false)
                    : m_content{std::move(content)}, isWord{_isWord} {}

            void insert(const string& content);
            void remove(const string& content);
            [[nodiscard]] bool search(const string& content) const;
            [[nodiscard]] bool operator!=(const Node& other) const;

        private:
            string m_content;
            ForwardList<Node> m_children;

            bool isWord{false};

            void splitNode(std::size_t idx);
            void getElementsRecursive(std::string& currentWord,
                                      std::set<std::string>& words) const;
        };
        Node m_root;
    };

} // namespace Utils

#endif // !SUFFIX_TRIE_HPP
