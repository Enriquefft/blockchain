#include<iostream>
#include <memory>
using namespace std;

int chunksize = 8;

int chunksize = 3;

//buff_size is the length of the chunk
template<typename T>
struct deque_iterator{
    typedef deque_iterator iterator;

    T* current;//pointer to the chunk
    T* first;//begin of the chunk
    T* last;//end of the chunk
    T** node;//pointer to the map

    void set_node(T** new_node){//salta a otro chunk
        node = new_node;
        first = *new_node;
        last = first + chunksize;
    }
    T& operator*()const{return *current;}
    iterator& operator++(){
        ++current;
        if(current == last){
            set_node(node + 1);
            current = first;
        }
        return *this;
    }
    iterator& operator--(){
        if(current == first){
            set_node(node -1);
            current = last;
        }
        --current;
        return *this;
    }

    iterator& operator+=(ptrdiff_t n){
        ptrdiff_t  offset = n + (current - first);
        if(offset >= 0 && offset< ptrdiff_t(chunksize)){
            current += n;
        }
        else{
            ptrdiff_t node_offset;
            if(offset > 0){
                node_offset = offset / ptrdiff_t(chunksize);
            }
            else{
                node_offset = -((offset - 1) / ptrdiff_t(chunksize)) - 1;
            }
            set_node(node + node_offset);
            current = first + (offset - node_offset * chunksize);
        }
        return *this;
    }
    iterator operator+(ptrdiff_t n)const{
        iterator temp = *this;
        return temp += n;
    }
    iterator& operator -=(ptrdiff_t n)const{
        iterator tmp = *this;
        return tmp += n;
    }
    iterator operator-(ptrdiff_t n)const{
        iterator tmp = *this;
        return tmp -= n;
    }
    T& operator[](ptrdiff_t n)const{
        return *(*this + n);
    }
    bool operator!=(const iterator& other){
        return this->current != other.current;
    }
};

template<typename T>
class deque{
public:
    typedef deque_iterator<T> iterator;
    typedef allocator<T> dataAllocator;
    typedef allocator<T*> mapAllocator;

private:
    T** map;
    size_t map_size;
    iterator start;
    iterator finish;

    void create_map_and_nodes(size_t num_elements){
        size_t num_nodes = num_elements / chunksize;
        if(num_elements % chunksize !=0){
            ++num_nodes;
        }
        map_size = size_t(std::max(8,int(num_nodes + 2)));
        mapAllocator  alloc;
        map = alloc.allocate(map_size);

        T** tmp_start = map + (map_size - num_nodes) / 2;
        T** tmp_finish = tmp_start + num_nodes - 1;
        T** cur;
        dataAllocator alloc1;
        for(T** current = tmp_start;current <= tmp_finish;++current){
            *current = alloc1.allocate(chunksize);
        }


        start.set_node(tmp_start);
        start.current = start.first;

        finish.set_node(tmp_finish);
        finish.current = finish.first + num_elements % chunksize;
    }

    void fill_initialize(size_t n,const T& value){
        create_map_and_nodes(n);

        T* cur;
        for(cur = start.current; cur != finish.current; ++cur){
            *cur = value;
        }
    }

public:
    deque()=default;
    explicit deque(size_t num_elements){
        fill_initialize(num_elements,T());
    }
    deque(size_t num_elements,const T& value){
        fill_initialize(num_elements,value);
    }
    T& front(){
        return *start;
    }
    T& back(){
        iterator tmp = finish;
        --tmp;
        return *tmp;
    }
    T& operator[](size_t n){
        return start[n];
    }
    void push_back(const T& value){
        *(finish.current) = value;
        ++finish.current;
    }
    void push_front(const T& value){
        *(start.current) = value;
        --start.current;
    }
    T& set_front(){
        iterator tmp = start;
        ++tmp;
        return *tmp;
    }
    void test(){
        --start.current;
    }
    iterator begin(){return start;}
    iterator end(){return finish;}
};
