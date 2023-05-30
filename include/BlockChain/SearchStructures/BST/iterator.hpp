#include <stack>
#include <iostream>
#include <queue>
#include "node.hpp"
using namespace std;
template<typename T>
class BSTIterator {
public:
    enum Type {
        PreOrder, InOrder, PostOrder, BFS
    };
private:
    NodeBT<T> *current;
    stack<NodeBT<T>*> s;
    Type type;
    queue<NodeBT<T>*> cola;
public:
    BSTIterator() : current(nullptr), type(InOrder) {}
    BSTIterator<T> &operator=(const BSTIterator<T> &other) {
        if (this != &other) {
            current = other.current;
            s = other.s;
            type = other.type;
        }
        return *this;
    }
    explicit BSTIterator(NodeBT<T>* node, Type _type = InOrder) : current(node), type(_type) {
        if(type == PreOrder){
            if (current != nullptr) {
                s.push(current);
            }
        }
        else if(type == InOrder){
            while(node->left != nullptr){
                s.push(node);
                node = node->left;
            }
            current = node;
        }
        else if (type == PostOrder) {
            if (node != nullptr) {
                stack<NodeBT<T>*> tempStack;
                tempStack.push(node);

                while (!tempStack.empty()) {
                    NodeBT<T>* temp = tempStack.top();
                    tempStack.pop();
                    s.push(temp);

                    if (temp->left != nullptr) {
                        tempStack.push(temp->left);
                    }

                    if (temp->right != nullptr) {
                        tempStack.push(temp->right);
                    }
                }
                if (!s.empty()) {
                    current = s.top();
                    s.pop();
                }
            }

        }
        else if(type == BFS){
            current == node;
            cola.push(current);
        }
    }
    BSTIterator& operator++() {
        if(type == InOrder){
            if(current->right != nullptr) {
                current = current->right;
                while (current->left != nullptr) {
                    s.push(current);
                    current = current->left;
                }
            }
            else {
                if (s.empty()) {
                    current = nullptr;
                } else {
                    current = s.top();
                    s.pop();
                }
            }
        }
        else if(type == PreOrder){
            if(!s.empty()){
                current = s.top();
                s.pop();
                if(current->right != nullptr){
                    s.push(current->right);
                }
                if(current->left != nullptr){
                    s.push(current->left);
                }
                if(s.empty()){
                    current = nullptr;
                }
                else{
                    current = s.top();
                }
            }
            else{
                current = nullptr;
            }
        }
        else if (type == PostOrder) {
            if (!s.empty()) {
                NodeBT<T>* parent = s.top();
                if (parent->left != nullptr && parent->left == current) {
                    if (parent->right != nullptr) {
                        NodeBT<T>* temp = parent->right;
                        while (temp != nullptr) {
                            s.push(temp);
                            if (temp->left != nullptr) {
                                temp = temp->left;
                            } else {
                                temp = temp->right;
                            }
                        }
                    } else {
                        current = parent;
                        s.pop();
                    }
                } else {
                    current = parent;
                    s.pop();
                }
            } else {
                current = nullptr;
            }
        }
        else if(type == BFS){
            if(current == nullptr){
                return *this;
            }

            if (!cola.empty()) {
                current = cola.front();
                cola.pop();
                if (current->left != nullptr) {
                    cola.push(current->left);
                }
                if (current->right != nullptr) {
                    cola.push(current->right);
                }
            }
            else {
                current = nullptr;
            }
        }
        return *this;
    }
    bool operator!=(const BSTIterator<T>& other) const {
        return current != other.current;
    }
    T& operator*() {
        return current->data;
    }
};
