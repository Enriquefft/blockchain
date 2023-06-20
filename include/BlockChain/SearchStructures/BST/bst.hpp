#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <stack>
#include "iterator.hpp"
 

template<typename T>
class BST{
    NodeBT<T>* root;
public:
    BST():root(nullptr){}
    void insert(T value){insert(root,value);}
    bool find(T value){
        if(root != nullptr){
            NodeBT<T>* nodo = find(root,value);
            if(nodo){
                return true;
            }
        }
        return false;
    }
    int height(){
        if(root != nullptr){
            return height(root);
        }
        return 0;
    }
    T minValue(){
        if(root){
            return minValue(root)->data;
        }
        throw std::invalid_argument("root is not initialized, minValue error\n");
    }
    T maxValue(){
        if(root){
            return maxValue(root)->data;
        }
        throw std::invalid_argument("root is not initialized, maxValue error\n");
    }
    void remove(T value){
        if(root){
            remove(root,value);
        }
        throw std::invalid_argument("root is not initialized, remove error\n");
    }
    bool isBalanced(){
        if(root){
            return isBalanced(root);
        }
        return false;
    }
    int size();
    bool isFull(){
        return isFull(root);
    }
    T successor(T value){
        if(root){
            return successor(root,value)->data;
        }
        throw std::invalid_argument("root not initialzed\n");
    }
    T predecessor(T value){
        if(root){
            return predecessor(root,value)->data;
        }
        throw std::invalid_argument("root not initialized\n");
    }
    void clear(){
        if(root){this->root->killself();}
        root = nullptr;
    }
    void buildFromOrderedArray(T* array, int n){
        buildFromOrderedArray(array,0,n-1);
    }
    void display(){
        if(root != nullptr){
            display(root);
        }
        else{
            throw std::invalid_argument("root not initialized\n");
        }
    }
private:
    void insert(NodeBT<T>*& node, T value){
        if(node == nullptr){
            node = new NodeBT<T>(value);
        }
        else if(node->data < value){
            insert(node->right,value);
        }
        else if(node->data > value){
            insert(node->left,value);
        }}
    void display(NodeBT<T>* node){
        if(node == nullptr){
            return;
        }
        display(node->left);
        std::cout<<node->data<<" ";
        display(node->right);
    }
    NodeBT<T>* find(NodeBT<T>* node,T value){
        if(node->data == value){
            return node;
        }
        else if(node->data < value){
            return find(node->right,value);
        }
        else if(node->data > value){
            return find(node->left,value);
        }
        return nullptr;
       
    }
    int height(NodeBT<T>* node){
        if(node == nullptr){
            return -1;
        }
        return std::max(height(node->left),height(node->right)) + 1;
    }
    NodeBT<T>* minValue(NodeBT<T>* node){
        while(node->left != nullptr){
            node = node->left;
        }
        return node;
    }
    NodeBT<T>* maxValue(NodeBT<T>* node){
        while(node->right != nullptr){
            node = node->right;
        }
        return node;
    }
    void remove(NodeBT<T>*& node, T value){
        if(node == nullptr){throw std::invalid_argument("node not found\n");}
        else if(node->data < value){remove(node->right,value);}
        else if(node->data > value){remove(node->left,value);}
        else{
            if(node->right == nullptr && node->left == nullptr){
                delete node;
            }
            else if(node->right == nullptr){
                NodeBT<T> temp = node;
                node = node->left;
                delete temp;
            }
            else if(node->left == nullptr){
                NodeBT<T> temp = node;
                node = node->right;
                delete node;
            }
            else{
                NodeBT<T>* nodito = minValue(node->right);
                node->data = nodito->data;
                remove(node->right,nodito->data);
            }
        }
        
    }
    NodeBT<T>* successor(NodeBT<T>* root, T value){
        std::stack<NodeBT<T>*> nodos;
        NodeBT<T>* nodo = root;
        while(nodo != nullptr && nodo->data != value){
            if(nodo->data < value){
                nodos.push(nodo);
                nodo = nodo->right;
            }
            else if(nodo->data > value){
                nodos.push(nodo);
                nodo = nodo->left;
            }
        }   
        if(nodo == nullptr){
            return nullptr;
        }
        else if(nodo->right != NULL){
            nodo = nodo->right;
            while(nodo->left != nullptr){
                nodo = nodo->left;
            }
            return nodo;
        }
        else{
            while (nodo->data > nodos.top()->data) {
                nodos.pop();
            }
            return nodos.top();
        }
        
    }
    NodeBT<T>* predecessor(NodeBT<T>* nodeRoot, T value){
        std::stack<NodeBT<T>*> nodos;
        NodeBT<T>* nodo = nodeRoot;
        while(nodo != nullptr && nodo->data != value){
            if(nodo->data < value){
                nodos.push(nodo);
                nodo = nodo->right;
            }
            else if(nodo->data > value){
                nodos.push(nodo);
                nodo = nodo->left;
            }
        }  
        if(nodo == nullptr){return nullptr;}
        if(nodo->left != nullptr){
            nodo = nodo->left;
            while(nodo->right != nullptr){
                nodo = nodo->right;
            }
        }
        else{
            while(nodo->data < nodos.top()->data){
                nodos.pop();
            }
            nodo = nodos.top();
        }
        return nodo;
    }
    bool isBalanced(NodeBT<T>* node){     
        if(node == nullptr){return true;}
        int left = height(node->left);
        int right = height(node->right);
        int difference = std::abs(left - right);
        if(difference > 1){return false;}

        return isBalanced(node->left) && isBalanced(node->right);
    }
    bool isFull(NodeBT<T>* nodo){
        if(nodo == nullptr){
            return true;
        }
        else if(nodo->left != nullptr && nodo->right !=nullptr){
            return isFull(nodo->left) && isFull(nodo->right);
        }
        else{
            return false;
        }
    }
    void buildFromOrderedArray(T*& array,int start,int end){
        if(start > end){
            return;
        }
        int middle =  (end + start)/ 2;
        insert(array[middle]);
        buildFromOrderedArray(array,start,middle-1);
        buildFromOrderedArray(array,middle + 1,end);
    }
};
