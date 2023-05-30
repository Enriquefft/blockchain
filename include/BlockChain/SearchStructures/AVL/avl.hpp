#include "node.hpp"
#include <bits/types/struct_tm.h>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <stdexcept>

template <typename T>
class AVL{
    NodeBT<T>* root;
public:
    AVL():root(nullptr){}
    void insert(T value){
        insert(root,value);
    }
    bool find(T value){
        if(!root){
            NodeBT<T>* nodo = find(root,value);
            if(nodo != nullptr){return true;}
        }
        else{
            return false;
        }
    }
    int height(){
        return height(root);
    }
    T minValue(){
        if(root){
            return minValue(root)->data;
        }
        throw std::invalid_argument("root not initialized\n");
    }
    T maxValue(){
        if(root){return maxValue(root)->data;}
        throw std::invalid_argument("root not initialized\n");
    }
    bool isBalanced(){
        if(root){
            return isBalanced(root);
        }
        else{
            throw std::invalid_argument("root isnt initialized\n");
        }
    }
    int size();
    void remove(T value){
        if(root){
            remove(root,value);
        }
        else {
            throw std::invalid_argument("root is not initialized\n");
        }
    }
    void display(){
        if(root == nullptr){
            throw std::invalid_argument("root is not initialized\n");
        }
        else{
            display(root);
        }
    }
    void displayPreOrder(){
        if(root == nullptr){
            throw std::invalid_argument("root is not initialized\n");
        }
        else{
            displayPreOrder(root);
        }
    }
    void clear(){
        if(root){
            root->killself();
            root = nullptr;
        }
    }
    ~AVL(){clear();}
private:
    void insert(NodeBT<T>*& node,T value){
        if(node == nullptr){
            node = new NodeBT<T>(value);
        }
        else if(node->data < value){insert(node->right,value);}
        else if(node->data > value){insert(node->left,value);}        
        updateheight(node);
        balancear(node);

    }
    void remove(NodeBT<T>*& node,T value){
        if(node == nullptr){return;}
        else if(node->data < value){remove(node->right,value);}
        else if(node->data > value){remove(node->left,value);}
        else{
            if(node->left == nullptr && node->right == nullptr){delete node; node = nullptr;}
            else if(node->left == nullptr){
                NodeBT<T>* tmp = node;
                node = node->right;
                delete tmp; 
            }
            else if(node->right == nullptr){
                NodeBT<T>* tmp = node;
                node = node->left;
                delete tmp;
            }
            else{
                T temp = maxValue(node->left)->data;
                node->data = temp;
                remove(node->left,temp);
            }
        }
        if(node){
            updateheight(node);
            balancear(node);
        }
    }
    NodeBT<T>* find(NodeBT<T>* node, T value){
        if(node == nullptr){
            return nullptr;
        }
        if(node->data == value){
            return node;
        }
        else if(node->data < value){
            return find(node->right,value);
        }
        else {
            return find(node->left,value);
        }
    }
    int height(NodeBT<T>* node){
        if(node == nullptr){
            return -1;
        }
        return std::max(height(node->left),height(node->right)) + 1;
    }
    void updateheight(NodeBT<T>*& node){
        node->height = std::max(height(node->left),height(node->right)) + 1;
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
    void display(NodeBT<T>* node){
        if(node == nullptr){return;}
        display(node->left);
        std::cout<<node->data<<" ";
        display(node->right);
    }
    void displayPreOrder(NodeBT<T>* node){
        if(node == nullptr){return;}
        std::cout<<node->data<<" ";
        displayPreOrder(node->left);
        displayPreOrder(node->right);
    }
    bool isBalanced(NodeBT<T>* node){
        if(node == nullptr){return true;}

        int left = height(node->left);
        int right = height(node->right);
        int difference = std::abs(left - right);
        if(difference > 1){return false;}

        return isBalanced(node->left) && isBalanced(node->right);
    }

    void rrota(NodeBT<T>*& node){
        NodeBT<T>* hijo_izquierdo = node->left;
        node->left = hijo_izquierdo->right;
        hijo_izquierdo->right = node;
        updateheight(node);
        updateheight(hijo_izquierdo);
        node = hijo_izquierdo;
    }
    void lrota(NodeBT<T>*& node){
        NodeBT<T>* hijo_derecho = node->right;//hijo derecho de node
        node->right = hijo_derecho->left;// hijo derecho de node es ahora el hijp izquierdo del hijo derecho
        hijo_derecho->left = node;
        updateheight(node);
        updateheight(hijo_derecho);
        node = hijo_derecho;//con esto se converva los enlaces con el padre de node
    }
    void balancear(NodeBT<T>*& node){
        int hb = factorBalanceo(node);
        if(hb > 1){
            if(factorBalanceo(node->left)< 0){
                lrota(node->left);
            }
            rrota(node);
        }
        else if(hb < -1){
            if(factorBalanceo(node->right) > 0){
                rrota(node->right);
            }
            lrota(node);
        }
    }
    int factorBalanceo(NodeBT<T>* node){
        return height(node->left) - height(node->right);
    }
};
