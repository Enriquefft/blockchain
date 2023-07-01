//include"../BlockChain/BlockChain.hpp"
#include<iostream>
#include "../BlockChain/Data.hpp"
using namespace std;


struct Nodo{
	Data data;
	Nodo* next;
	Nodo(Data value):data(value){next  = nullptr;}
};

class Flist{
	Nodo* head;
public:
	Flist():head(nullptr){}
	Flist(Data value){head = new Nodo(value);}
	void display(){
		Nodo* current = head;
		while(head != nullptr){
			cout<<head->data<<" ";
			head = head->next;
		}
	}
	void insert(Data data){
		Nodo* temp = new Nodo(data);
		temp->next = head;
		head = temp;
	}
	~Flist(){
		delete head;
	}
};
