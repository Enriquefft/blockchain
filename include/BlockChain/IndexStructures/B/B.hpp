//
// Created by joel on 7/6/23.
//

#ifndef TODO_NADA_B_H
#define TODO_NADA_B_H

#include "Data.h"
#include <vector>
#include <stack>
#include <iostream>
using namespace std;

template <CompareType compare_t> class B {
public:
    using size_type = std::size_t;
    using index_type = getType<compare_t>;
    using value_type = const Data *;

    explicit B(int _grado): grado(_grado){
        root = nullptr;
    }
    ~B(){}

    B(B &&other) noexcept : root(other.root){
        other.root = nullptr;
    }
    B &operator=(B &&other) noexcept {
        if( this != &other){
            delete root;
            root = other.root;
            other.root = nullptr;
        }
        return *this;
    }
    B(const B&) = delete;
    B &operator=(const B&) = delete;

    void insert(value_type data){
        if( root == nullptr) {
            root = new Node(4);
            root->keys[0] = data;
            root->count++;
        }
        else{
            if(root->count == grado){
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

    [[nodiscard]] vector<value_type> rangeSearch(const index_type &minValue, const index_type &maxValue) const{
        vector<value_type> result;
        stack<pair<const Node*,int>> salvacion;
        root->rangeSearch(minValue,maxValue,result,salvacion);

        salvacion.pop();
        while (!salvacion.empty()){
            const Node * temp = salvacion.top().first;
            int inner_index = salvacion.top().second;
            while (inner_index < temp->count){
                result.push_back(temp->keys[inner_index]);
                if (temp->keys[inner_index]->template get<compare_t>() == maxValue){
                    break;
                }
                if(temp->children[salvacion.top().second + 1] != nullptr){
                    temp->children[salvacion.top().second + 1]->getElements_rangeSearch(result,maxValue);
                }
                inner_index++;

            }


            salvacion.pop();
        }
        cout << endl;




        return result;
    }

    [[nodiscard]] vector<value_type> getElements() const{
        vector<value_type > result;
        root->getElements(result);
        return  result;
    }

    [[nodiscard]] size_type height() const{
        Node* tmp = root;
        size_type contador = 0;
        while (tmp->children[0] != nullptr){
            contador++;
            tmp = tmp->children[0];
        }
        return contador;
    }
    [[nodiscard]] size_type  size() const {
        size_type contador = 0;
        root->size(contador);
        return contador;
    }
    void clear() { root = nullptr;}
    [[nodiscard]] bool empty() const{ return  root->count == 0;}

private:
    class Node{
        friend class B;
    private:
        value_type * keys;
        Node** children;
        int count;
        bool leaf;
        int grado;

        explicit Node(int _grado) : grado(_grado), keys(nullptr),children(nullptr),count(0),leaf(true){
            keys = new value_type [grado - 1];
            children = new Node*[grado];
        }
        ~Node(){
            delete keys;
            for(int i=0;i<grado +  1; i++){
                delete[] children[i];
            }
            delete[] children;
        }
        Node(const Node &other) : grado(other.grado),leaf(other.leaf),count(other.count),
            keys(nullptr),children(nullptr){
            if(other.keys != nullptr){ keys = other.keys;}
            if(other.children != nullptr){children = other.children;}
        }
        Node(Node &&other) noexcept : keys(other.keys),children(other.children),count(other.count),
            leaf(other.leaf),grado(other.grado){
            other.keys = nullptr;
            other.children = nullptr;
            other.count = 0;
        }
        Node &operator=(const Node &other){
            if (this != &other) {
                delete keys;
                for (int i=0;i < grado + 1; i++){
                    delete[] children[i];
                }
                delete children;
                count = other.count;
                grado = other.grado;
                leaf = other.leaf;
                if (other.keys != nullptr) {
                    keys = new value_type [grado -1];
                    for(int i=0;i<other.count;i++){
                        keys[i] = other.keys[i];
                    }
                }
                if (other.children != nullptr) {
                    children = new Node* [grado];
                    for(int i=0;i<other.count + 1;i++){
                        children[i] = other.children[i];
                    }
                }
            }
            return  *this;
        }
        Node &operator=(Node &&other) noexcept {
            if (this != &other) {
                delete keys;
                for (int i=0;i < grado + 1; i++){
                    delete[] children[i];
                }
                delete children;
                count = other.count;
                grado = other.grado;
                leaf = other.leaf;

                keys = other.keys;
                children = other.children;

                other.keys = nullptr;
                other.children = nullptr;
            }
            return *this;
        }

        void rangeSearch(const index_type &minValue, const index_type &maxValue, vector<value_type> &vec,stack<pair<const Node*,int>> &salavacion) const{

            int index = count - 1;
            while (index > 0 && keys[index]->get<compare_t>() > minValue){
                index--;
            }
            pair<const Node*,int> a(this,index);
            salavacion.push(a);
            if(keys[index]->get<compare_t>() == minValue){
               int inner_index = index;
                while (inner_index < count && keys[inner_index]->get<compare_t>() != maxValue) {
                    vec.push_back(keys[inner_index]);
                    inner_index++;
                }
            }
            if( children[index] != nullptr && keys[index]->get<compare_t>() != minValue ){
                children[index]->rangeSearch(minValue,maxValue,vec,salavacion);
            }
            if(leaf == true && index == -1){
                return;
            }









        }
        void getElements(vector<value_type> &vec) const{
            for(int i=0;i<count;i++){
                if(children[i] != nullptr){
                    children[i]->getElements(vec);
                }
                vec.push_back(keys[i]);
            }
            if(children[count] != nullptr){
                children[count]->getElements(vec);
            }
        }
        void getElements_rangeSearch(vector<value_type> &vec,const index_type maxValue) const{
            for(int i=0;i<count;i++){
                if(children[i] != nullptr){
                    children[i]->getElements(vec);
                }

                vec.push_back(keys[i]);
                if(keys[i]->get<compare_t>() == maxValue){
                    return;
                }
            }
            if(children[count] != nullptr){
                children[count]->getElements(vec);
            }
        }
        [[nodiscard]] size_type size(int &contador) const{
            for(int i=0;i<count;i++){
                if(children[i] != nullptr){
                    children[i]->size(contador);
                }
                contador++;
            }
            if(children[count] != nullptr){
                children[count]->size(contador);
            }
        }
        void clear() {
            delete keys;
            for(int i=0;i<grado +  1; i++){
                delete[] children[i];
            }
            delete[] children;
        }
        [[nodiscard]] bool empty() const{
            return (this == nullptr);
        }

        void splitChildren(int index){
            Node* origin = children[index];
            Node* sibling = new Node(grado);

            for(int i=grado/2 + 1,j=0;i<grado;i++,j++){
                sibling->keys[j] = origin->keys[i];
            }
            sibling->count = grado - (( grado / 2) + 1);
            origin->count = grado / 2;

            if(!origin->leaf){
                for(int i = grado / 2 + 1, j = 0; i < grado + 1 ; i++,j++){
                    sibling->children[j] = origin->children[i];
                    origin->children[i] = nullptr;
                }
            }

            int insert_parent_index = count - 1;
            while(insert_parent_index  >= 0 && COMPARE(*(keys[insert_parent_index]) , *(origin->keys[grado/2]))){
                keys[insert_parent_index + 1] = keys[insert_parent_index];
            }
            keys[++insert_parent_index] = origin->keys[grado/2];//inserting the middle value
            count++;
            leaf = false;

            int index_children = count-1;
            while(index + 1  <= index_children){
                children[index_children + 1] = children[index_children];
                index_children--;
            }
            children[++index_children] = sibling;
            sibling->leaf = origin->leaf;
        }
        void insertNotFull(value_type value){
            int index = count - 1;
            if(leaf){
                while(index >= 0 && COMPARE( *keys[index],*value )){
                    keys[index + 1] = keys[index];
                    index--;
                }
                index++;
                keys[index] = value;
                count++;
            }
            else{
                while(index >= 0 && COMPARE( *keys[index],*value )){
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
        bool search(index_type data){
            int i=0;
            while (i < count && data > keys[i]->get<compare_t>()){
                i++;
            }
            if (keys[i]->get<compare_t>() == data){return true;}
            if (leaf){return false;}
            return children[i]->search(data);
        }

    };

    Node* root;
    inline static constexpr Data::Compare<compare_t> COMPARE{};
    int grado;
};


#endif //TODO_NADA_B_H
