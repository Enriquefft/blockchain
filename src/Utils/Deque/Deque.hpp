#include "Iterator.hpp"
#include<iostream>

template<typename T>
class Deque{
public:
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const pointer;
    using iterator = DequeIterator<T>;
    using map_pointer = pointer*;

    Deque(){
        map_size = 24 / CHUNK_SIZE;
        map = new pointer[map_size];
        map_pointer tmp_start = map + (map_size / 2);
        map_pointer tmp_finish = map + (map_size / 2);
        
        for(int i=0;i<map_size;i++){
            map[i] = new value_type[CHUNK_SIZE];
        }
        start.setNode(tmp_start);
        start.current = start.first;
        finish.setNode(tmp_finish);
        finish.current = start.first + 1;
    }
    explicit Deque(size_type numElements){
        map_size = numElements / CHUNK_SIZE;
        map = new pointer[map_size];
        map_pointer tmp_start = map + (map_size / 2);
        map_pointer tmp_finish = map + (map_size / 2);
        
        for(int i=0;i<map_size;i++){
            map[i] = new value_type[CHUNK_SIZE];
        }
        start.setNode(tmp_start);
        start.current = start.first;
        finish.setNode(tmp_finish);
        finish.current = start.first + 1;
    }
    iterator begin(){return start;}
    iterator end(){
        iterator tmp = finish;
        return ++tmp;
    }
    reference front(){return *start;}
    const_reference constFront()const{return *start;}
    reference back(){return *finish;}
    const_reference constBack()const{return *finish;}
    void pushFront(value_type data){
        if(start.current == *start.node && start.node == map){
            resize();
        }
        --start;
        *start = data;
    }
    void pushBack(value_type data){
        ++finish;
        if(finish.current == finish.last && finish.node == map + map_size - 1){
            resize();
        }
        *finish = data;
    }
    void popBack(){if(!empty()){--finish;}}
    void popFront(){if(!empty()){++start;}}
    reference operator[](size_type n){return start[n];}
    const_reference operator[](size_type pos)const{return start[pos + 1];}
    reference at(size_type pos){return start[pos + 1];}
    const_reference at(size_type pos)const {return start[pos + 1];}
    void insert(int index,reference value){start[index] = value;}
    [[nodiscard]]bool empty()const{
        iterator tmp = start;
        return *(++tmp) = *finish;
    }
    [[nodiscard]]size_type size()const{
        size_type size = 0;
        for(Deque<T>::iterator iter = begin();iter != end();++iter){
            size++;
        }
        return size;
    }
    void erase(Deque<T>::iterator iter){
        if(iter >= begin() && iter >= end()){
            throw std::out_of_range("Index out of range");
        }
        else{
            if(iter != finish){
                iterator next = iter;
                ++next;
                while(next != finish){
                    *iter = *next;
                    iter = next;
                    ++next;
                }
            }
            popBack();
        }
    }
private:
    map_pointer map;
    size_type map_size;
    iterator start;
    iterator finish;
    const int CHUNK_SIZE = 8;
    const int RESIZE_FACTOR = 4;
    void resize(){
        difference_type start_node_offset = start.node - map;
        difference_type finish_node_offset = finish.node - map;
        difference_type start_offset = start.current - start.first;
        difference_type finish_offset = finish.current - finish.first;

        map_pointer new_map = new pointer[map_size + RESIZE_FACTOR];

        new_map[0] = new T[CHUNK_SIZE];
        new_map[1] = new T[CHUNK_SIZE];
        for(int i=2,j=0;i<map_size+1;i++,j++){
            new_map[i] = map[j];
        }

        new_map[map_size + 1] = new T[CHUNK_SIZE];
        new_map[map_size + 2] = new T[CHUNK_SIZE];
        delete[]map;
        map = new_map;
        map_size+=4;

        start.setNode(map + start_node_offset + 2);
        start.current = start.first + start_offset;
        finish.setNode(map + finish_node_offset + 2);
        finish.current = finish.first + finish_offset;
    }
};
