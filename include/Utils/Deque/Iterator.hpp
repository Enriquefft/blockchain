#include <cstddef>

template<typename T>
struct DequeIterator
{
public:
    using iterator = DequeIterator<T>;
    using value_type = T;
    using map_pointer = value_type**;
    using pointer = value_type*;
    using reference = value_type&;
    using const_reference = const value_type&;
    using size_type = std::size_t;
    using difference_type = ptrdiff_t;
    
    pointer current;
    pointer first;
    pointer last;
    map_pointer node;
    
    const int CHUNK_SIZE = 8;

    int chunkSize(){return CHUNK_SIZE;}
    void setNode(map_pointer newNode){
        node = newNode;
        first = *newNode;
        last = last + chunkSize();
    }
    reference operator*(){return *current;}
    iterator& operator++(){
        ++current;
        if(current == last){
            setNode(node + 1);
            current = first;
        }
        return *this;
    }
    iterator& operator--(){
        --current;
        if(current == first){
            setNode(node - 1);
            current = last;
        }
        return *this;
    }
    iterator& operator+=(difference_type n){
        difference_type offset = n + (current - first);
        if(offset >= 0 && offset < CHUNK_SIZE){
            current += n;
        }
        else{
            difference_type node_offset = 0;
            if(offset > 0){
                node_offset = offset / CHUNK_SIZE;
                offset %= CHUNK_SIZE;
            }
            else{
                node_offset = -((offset - 1) / difference_type(chunkSize)) - 1;
                offset  = offset % CHUNK_SIZE + CHUNK_SIZE; 
            }
            setNode(node + node_offset);
            current = first + offset;
        }
        return *this;
    }
    reference operator[](difference_type n){return *(*this + n);}
    bool operator !=(iterator other)const{
        return this->current != other.current;
    }   
    bool operator <=(const iterator& other)const{
        if(node == other.node){
            return current <=other.current;
        }
        else{
            return node <= other.node;
        }
    }
};
