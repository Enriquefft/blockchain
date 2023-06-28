template<typename T>
struct NodeBT{
    T data;
    NodeBT<T>* left;
    NodeBT<T>* right;
    NodeBT(){
        left = right = nullptr;
    }
    explicit NodeBT(T value):data(value){
        left = right = nullptr;
    }
    void killself(){
        if(left != nullptr){left->killself();}
        if(right != nullptr){right->killself();}
        delete this;
    }
};
