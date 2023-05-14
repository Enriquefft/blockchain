#include<iostream>
#include <memory>
using namespace std;

int chunksize = 8;

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
    T& operator[](size_t n)const{
        return *(*this + n);
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
        map = mapAllocator::allocate(map_size);

        T** tmp_start = map + (map_size - num_nodes) / 2;
        T** tmp_finish = tmp_start + num_nodes - 1;
        T** cur;

        for(T** current = tmp_start;current <= tmp_finish;++current){
            *current = dataAllocator::allocate(chunksize);
        }


        start.set_node(tmp_start);
        start.current = start.first;

        finish.set_node(tmp_finish);
        finish.current = finish.first + num_elements % chunksize;
    }

public:
    deque()=default;
    explicit deque(size_t num_elements){
        size_t num_nodes = num_elements / chunksize;
        map_size = size_t(std::max(8,int(num_nodes + 2)));
        mapAllocator alloc;
        map = alloc.allocate(map_size);

        T** tmp_start = map + (map_size - num_nodes) / 2;
        T** tmp_finish = tmp_start + num_nodes - 1;

        dataAllocator alloc1;
        for(T** current = tmp_start;current <= tmp_finish;++current){
            *current = alloc1.allocate(chunksize);
        }

        start.set_node(tmp_start);
        start.current = start.first;

        finish.set_node(tmp_finish);
        finish.current = finish.first + num_elements % chunksize;


    };
    T& operator[](size_t n){
        return start[n];
    }

};