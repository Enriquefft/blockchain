#include "nodeB+.hpp"
#include<iostream>
using namespace std;

template<typename T,int grado>
class BPlustree{
	NodeBplus<T,grado>* root;
public:
	BPlustree(){
		root = nullptr;
	}
	void insert(T key){
		if(root == nullptr){
			root = new NodeBplus<T,grado>();
			root->keys[0] = key;
			root->count++;
		}
		else{
			if(root->count == grado){
				NodeBplus<T,grado>* newRoot = new NodeBplus<T,grado>();
				newRoot->children[0]  = root;
				root  = newRoot;
				root->leaf = false;
				root->splitChildren(0);
			}	
			root->insertNotFull(key);
		}
	}
	void remove(T key){root->removeNotFull(key);}
	void display(){
		for(int i=0;i<root->count;i++){
			cout<<root->keys[i]<<" ";
		}
		cout<<endl;
	
	}
	void forward(){
		root->display();
	}
	void displayChildren(int index){
		for(int i=0;i<root->children[index]->count;i++){
			cout<<root->children[index]->keys[i]<<" ";
		}
		cout<<" | ";
	}

	bool search(T key){
		if(root->search(key) == nullptr){return false;}
		else{return true;}
	}
};
