#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <vector>
#include <iostream>
namespace Utils {

	template <typename T> using Vector = std::vector<T>{
		public:
			using value_type = T;
			using size_type = std::size_t;
			using reference = T&;
			using pointer = T*;
			const_pointer = const T*;
			const_reference = const reference;
			//using iterator , const_iterator

			Vector()=default;
			explicit Vector(size_type cap):capacity(cap){
				array = new T[capacity];
				int position;
			}
			operator=(const_reference other){
				capacity = other.capacity
				T* new_array = new value_type[capacity];
				for(int i=0;i<capacity;i++){
					new_array[i] =  other.array;
				}
				delete array;
				array = new_array;
				index = other.index;
			}
			reference at(int i){
				return array[i];
			}
			reference operator[](int i){
				return array[i];
			}
			const_reference front(){
				return array[0];
			}
			const_reference back(){
				return array[index - 1];
			}
			bool empty(){
				return index==0;
			}
			int size(){
				return index;
			}
			int capacity(){
				return capacity;
			}
			void clear(){
				index = 0;
			}
			void insert(T value);
			void emplate();
			void push_back(T value){array[index++] = value;}
			void pop_back(){index--;}

		private:
				pointer array;
				size_type capacity;
				int index;
				void resize(){
					T* new_array = T[capacity*2];
					for(int i=0;i<capacity;i++){
						new_array[i] = array[i];
					}
					capacity *=2;
				}
	};

}

#endif // !VECTOR_HPP
