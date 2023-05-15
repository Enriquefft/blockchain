#include<iostream>


template<typename T,std::size_t buff_size>
struct deque_iterator{
    typedef deque_iterator<T,buff_size> iterator;
    typedef T**                         map_pointer;
    typedef T&                          reference;
    typedef T*                          pointer;

    pointer current;
    pointer first;
    pointer last;
    map_pointer node;

    void set_node(map_pointer new_node){
        node = new_node;
        first = *new_node;
        last = first + int(buff_size - 1);
    }
    reference operator*(){
        return *current;
    }
    iterator& operator++(){
        if(current == last){
            set_node(node + 1);
            current = first;
        }
        else{
            ++current;
        }
        return *this;
    }
    iterator& operator--(){
        if(current == first){
            set_node(map_pointer - 1);
            current = last;
        }
        else{
            --current;
        }
        return *this;
    }
    bool operator !=(const iterator& other){
        return this->current != other.current;
    }
    iterator& operator +=(ptrdiff_t n){
        ptrdiff_t offset = n + (current - first);
        if(offset >= 0 && offset < ptrdiff_t(buff_size)){
            current += n;
        }
        else{
            ptrdiff_t node_offset;
            if(offset > 0){node_offset = offset / ptrdiff_t(buff_size);}
            else{
                node_offset = -((offset - 1) / ptrdiff_t(buff_size)) - 1;
            }
            set_node(map_pointer + node_offset);
            current = first + (offset - node_offset * buff_size);
        }
        return *this;
    }
    iterator operator+(ptrdiff_t n)const{
        iterator temp = *this;
        return temp += n;
    }
    iterator& operator -=(ptrdiff_t n)const{
        iterator temp =*this;
        return temp -= n;
    }
    iterator operator-(ptrdiff_t n)const{
        iterator temp = *this;
        return temp -= n;
    }
    reference operator[](ptrdiff_t n)const{
        return *(*this + n);
    }
};

