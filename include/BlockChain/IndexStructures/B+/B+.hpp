//
// Created by joel on 7/7/23.
//

#ifndef TODO_NADA_BPLUS_H
#define TODO_NADA_BPLUS_H
#include <iostream>
#include "Data.h"
#include <vector>
using namespace std;

template <CompareType compare_t> class BPLUS {
public:
    using size_type = std::size_t;
    using index_type = getType<compare_t>;
    using value_type = const Data *;

    BPLUS(int _grado) : grado(_grado),root(nullptr) {}
    ~BPLUS() {}

    BPLUS(BPLUS &&other) noexcept : root(other.root), grado(other.grado){
        other.root = nullptr;
    }
    BPLUS &operator=(BPLUS &&other) noexcept {
        if (this != other){
            delete root;
            root = other.root;
            grado = other.grado;
            COMPARE = other.COMPARE;
        }
        return *this;
    }
    BPLUS(const BPLUS &) = delete;
    BPLUS &operator=(const BPLUS &) = delete;

    void insert(value_type data) {
        if(root == nullptr) {
            root = new Node(4);
            root->keys[0] = data;
            root->count++;
        }
        else {
            if (root->count == grado) {
                Node* newRoot = new Node(grado);
                newRoot->children[0] = root;
                root = newRoot;
                root->splitChildren(0);
            }
            root->insertNotFull(data);
        }
    }
    [[nodiscard]] bool search(const index_type &value) const{
        return root->search(value);
    }

    [[nodiscard]] vector<value_type>
    rangeSearch(const index_type &minValue, const index_type &maxValue) const {
        vector<value_type > result;
        root->rangeSearch(minValue,maxValue,result);
        return result;
    }

    [[nodiscard]] vector<value_type> getElements() const {
        vector<value_type > vec;
        const Node* tmp = root;

        while (tmp->children[0] != nullptr){
            tmp = tmp->children[0];
        }
        while(tmp != nullptr){
            for(int i=0;i<tmp->count;i++){
                vec.push_back(tmp->keys[i]);
            }
            tmp = tmp->next;
        }

        return vec;
    }


    [[nodiscard]] size_type height() const {
        Node* temp = root;
        size_type contador = 0;
        while (temp->children[0] != nullptr){
            contador++;
            temp = temp->children[0];
        }
        return contador;
    }
    [[nodiscard]] size_type size() const {
        size_type contador = 0;
        Node* temp = root;
        while(temp->children[0] != nullptr){
            temp = temp->children[0];
        }
        while(temp != nullptr){
            for(int i=0;i<temp->count;i++){
                contador++;
            }
            temp = temp->next;
        }
        return contador;
    }
    void clear() {
        root->clear();
    }
    [[nodiscard]] bool empty() const{
        return root->count == 0;
    }

private:
    class Node {
        friend class BPLUS;
    private:
        value_type * keys;
        Node *next;
        Node **children;
        int count;
        int leaf;
        int grado;

        explicit Node(int _grado) : grado(_grado), keys(new value_type[_grado -1 ]), children(new Node*[_grado]),next(nullptr),leaf(true){}
        ~Node() {
            delete keys;
            for(int i=0;i<grado + 1 ;i++){
                delete[] children[i];
            }
            delete[] children;
            delete next;
        }
        Node(const Node &other) : keys(nullptr),children(nullptr),next(nullptr),count(other.count) ,
                                    leaf(other.leaf),grado(other.grado){
            if(other.keys != nullptr){
                keys = new value_type [grado - 1];
                for(int i=0;i<grado-1;i++){
                    keys[i] = other.keys[i];
                }
            }
            if(other.children != nullptr){
                keys = new Node*[grado];
                for(int i=0;i<grado;i++){
                    children[i] = other.children[i];
                }
            }
            if(other.next != nullptr){
                next = other.next;
            }
        }
        Node(Node &&other) noexcept : grado(other.grado),leaf(other.leaf),count(other.count),
            keys(other.keys),next(other.next),children(other.children){
            other.keys = nullptr;
            other.next = nullptr;
            other.children = nullptr;
        }
        Node &operator=(const Node &other) {
            if (this != &other) {
                delete keys;
                delete next;
                for (int i=0; i < grado ;i++){
                    delete[] children[i];
                }
                delete children;
                count = other.count;
                grado = other.grado;
                other = other.leaf;
                if (other.keys != nullptr) {
                    keys = new value_type [grado - 1];
                    for (int i=0;i < other.count ; i++){
                        keys[i] = other.keys[i];
                    }
                }
                if (other.children != nullptr){
                    children = new Node*[grado];
                    for (int i=0; i< other.count + 1;i++){
                        children[i] = other.children[i];
                    }
                }
            }
            return *this;
        }
        Node &operator=(Node &&other) noexcept{
            if (this != &other){
                delete keys;
                delete next;
                for(int i=0;i<grado;i++){
                    delete[] children[i];
                }
                leaf = other.leaf;
                count = other.count;
                grado = other.grado;

                other.count = 0;
                other.grado = 0;

                keys = other.keys;
                next = other.next;
                children = other.children;

                other.next = nullptr;
                other.keys = nullptr;
                other.children = nullptr;
            }
            return *this;
        }

        void rangeSearch(const index_type &minValue, const index_type &maxValue,vector<value_type> &vec) const{
            int i = 0;
            while (i  < count  && keys[i]->get<compare_t>() < minValue){
                i++;
            }
            if(!leaf){
                children[i]->rangeSearch(minValue,maxValue,vec);
            }
            else if(leaf && keys[i]->get<compare_t>() == minValue){
                cout<<" cerca" << endl;
                for (int z=i; z < count;z++){
                    vec.push_back(keys[z]);
                }
                Node* temp = this->next;
                while(temp != nullptr){
                    for(int d=0;d<temp->count;d++){
                        if(temp->keys[d]->get<compare_t>() > maxValue){
                            break;
                        }
                        else{
                            vec.push_back(temp->keys[d]);
                        }

                    }
                    temp = temp->next;
                }

            }
        }
        void clear(){
            delete keys;
            for(int i=0;i<grado + 1 ;i++){
                delete[] children[i];
            }
            delete[] children;
            delete next;
        }
        [[nodiscard]] bool empty() const{
            return count == 0;
        }
        void splitChildren(int index){
            Node* origin = children[index];
            Node* sibling = new Node(origin->grado);

            if(origin->leaf){
                for(int i=grado/2,j=0;i<grado;i++,j++){
                    sibling->keys[j] = origin->keys[i];
                }
                sibling->count = grado - grado/2;
                origin->count = grado/2;
                value_type mid_value = origin->keys[grado/2];

                int index_parent_insert = count - 1;//keys realloc
                while(index_parent_insert >=0 && COMPARE(*keys[index_parent_insert] , *mid_value)){
                    keys[index_parent_insert + 1] = keys[index_parent_insert];
                    index_parent_insert--;
                }
                index_parent_insert++;
                keys[index_parent_insert]  = mid_value;
                count++;
                leaf = false;

                int index_children = count - 1;//children realloc
                while(index + 1 <= index_children){
                    children[index_children + 1] = children[index_children];
                    index_children--;
                }
                index_children++;
                children[index_children] = sibling;
                sibling->leaf = origin->leaf;

                sibling->next = origin->next;
                origin->next = sibling;
            }
            else if(origin->leaf == false){
                for(int i=grado/2+1,j=0;i<grado;i++,j++){
                    sibling->keys[j] = origin->keys[i];
                }

                sibling->count = grado - ((grado/2) + 1); // 1 , falta 1 (mid_value)
                origin->count = grado/2; // 2

                if(!origin->leaf){
                    for(int i=grado/2 + 1,j=0;i < grado + 1;i++,j++){
                        sibling->children[j] = origin->children[i];
                        origin->children[i] = nullptr;
                    }
                }

                int insert_parent_index = count  - 1; // keys realloc
                while(insert_parent_index >= 0 && COMPARE(*keys[insert_parent_index] , *origin->keys[grado/2])){
                    keys[insert_parent_index + 1] = keys[insert_parent_index];
                    insert_parent_index--;
                }
                keys[++insert_parent_index] = origin->keys[grado/2];//insert mid_value
                count++;
                leaf = false;

                int index_children = count -1;
                while(index + 1 <= index_children){//children realloc
                    children[index_children + 1] = children[index_children];
                    index_children--;
                }
                children[++index_children] = sibling;
                sibling->leaf = origin->leaf;

            }


        }
        void insertNotFull(value_type value){
            int index = count - 1;
            if(leaf){
                while(index >= 0 && COMPARE(*keys[index],*value)){
                    keys[index + 1] = keys[index];
                    index--;
                }
                index++;
                keys[index] = value;
                count++;
            }
            else{
                while(index >= 0 && COMPARE(*keys[index],*value)){
                    index--;
                }
                index++;
                if(children[index]->count == grado){
                    splitChildren(index);
                    if(COMPARE(*value,*keys[index])){
                        index++;
                    }
                }
                children[index]->insertNotFull(value);

            }

        }
        bool search(const index_type& data){
            int i=0;
            while( i < count && data > keys[i]->get<compare_t>()){
                i++;
            }
            if (keys[i]->get<compare_t>() == data){return true;}
            if (leaf){return false;}
            return children[i]->search(data);
        }
    };
    Node* root;
    int grado;
    inline static constexpr Data::Compare<compare_t> COMPARE{};
};

#endif //TODO_NADA_BPLUS_H
