#include<iostream>
using namespace std;


template<typename T, int grado>
struct NodeBplus{
	T* keys;
	NodeBplus** children;
	NodeBplus* next;
	int count;
	bool leaf;

	NodeBplus(){
		children = new NodeBplus*[grado];
		keys = new T[grado -1];
		count = 0;
		leaf = true;
		next = nullptr;
	}
	
	void splitChildren(int index){
		NodeBplus<T,grado>* origin = children[index];
		NodeBplus<T,grado>* sibling = new NodeBplus<T,grado>();

		if(origin->leaf){
			for(int i=grado/2,j=0;i<grado;i++,j++){
				sibling->keys[j] = origin->keys[i];
			}
			sibling->count = grado - grado/2;
			origin->count = grado/2;
			T mid_value = origin->keys[grado/2];

			int index_parent_insert = count - 1;
			while(index_parent_insert >=0 && keys[index_parent_insert] > mid_value){
				keys[index_parent_insert + 1] = keys[index_parent_insert];
				index_parent_insert--;
			}
			index_parent_insert++;
			keys[index_parent_insert]  = mid_value;
			count++;
			leaf = false;

			int index_children = count - 1;
			while(index + 1 <= index_children){
				children[index_children + 1] = children[index_children];
				index_children--;
			}
			index_children++;
			children[index_children] = sibling;
			sibling->leaf = origin->leaf;

			sibling->next = origin->next;
			origin->next = sibling;
		}
		else if(origin->leaf == false){
			for(int i=grado/2+1,j=0;i<grado;i++,j++){
				sibling->keys[j] = origin->keys[i];
			}

			sibling->count = grado - ((grado/2) + 1);
			origin->count = grado/2;

			if(!origin->leaf){
				for(int i=grado/2 + 1,j=0;i < grado + 1;i++,j++){
					sibling->children[j] = origin->children[i];
					origin->children[i] = nullptr;
				}
			}

			int insert_parent_index = count  - 1;
			while(insert_parent_index >= 0 && keys[insert_parent_index] > origin->keys[grado/2]){
				keys[insert_parent_index + 1] = keys[insert_parent_index];
				insert_parent_index--;
			}
			keys[++insert_parent_index] = origin->keys[grado/2];
			count++;
			leaf = false;

			int index_children = count -1;
			while(index + 1 <= index_children){
				children[index_children + 1] = children[index_children];
				index_children--;
			}
			children[++index_children] = sibling;
			sibling->leaf = origin->leaf;
			
		}
		

	}
	void insertNotFull(T value){
		int index = count - 1;
		if(leaf){
			while(index >= 0 && value < keys[index]){
				keys[index + 1] = keys[index];
				index--;
			}
			index++;
			keys[index] = value;
			count++;
		}
		else{
			while(index >= 0 && value < keys[index]){
				index--;
			}
			index++;
			if(children[index]->count == grado){
				splitChildren(index);
				if(value > keys[index]){
					index++;
				}
			}
			children[index]->insertNotFull(value);

		}

	}
	
	void deleteFromLeaf(T key){
		int index = count - 1;
		while(index >= 0 && keys[index] != key){
			index--;
		}
		while(index <= count -1){
			keys[index] = keys[index + 1];
			index++;
		}
		count--;
	}
	
	void deleteFromNonLeaf(int index,T key){
		T value = successor(key);
		keys[index] = value;
		children[index + 1]->removeNotFull(key);

	}
	
	T successor(T key){
		NodeBplus* nodito = this;
		int index = nodito->count-1;
		while(index >= 0 && nodito->keys[index] > key){
			index--;
		}
		index++;
		nodito = nodito->children[index];
		while(nodito->children[0] != nullptr){
			nodito = nodito->children[0];
		}
		return nodito->keys[1];
	}
		
	void removeNotFull(T key){
		int index = count - 1;
		if(leaf){
			deleteFromLeaf(key);
			return;
		}
		else{
			while(index >= 0 && keys[index] > key){
				index--;
			}
			if(key == keys[index]){
				deleteFromNonLeaf(index,key);
				return;
			}
			children[index + 1]->removeNotFull(key);
		}
		
		NodeBplus* current = children[index + 1];

		if(current->count == (grado/2)-1 && current->leaf){
			cout<<"key "<<key<<endl;
			cout<<"current count "<<current->count<<endl;
			cout<<"izquierda count "<<children[index]->count<<endl;
			cout<<"derecha count "<<children[index + 2]->count<<endl;
			cout<<endl;

			if(index != -1 && (children[index]->count > grado/2 || children[index + 2]->count > grado/2 )){
				cout<<"addition ";
				if(children[index + 2] == nullptr || children[index + 2]->count < grado/2){
					cout<<"1 "<<endl;
					T newPadre = children[index]->keys[children[index]->count -1];
					current->insertNotFull(newPadre);
					children[index]->removeNotFull(newPadre);
					keys[index] = current->keys[0];
				}
				else{
					cout<<"2 "<<endl;
					T newValue = children[index + 2]->keys[0];
					current->insertNotFull(newValue);
					children[index  + 2]->removeNotFull(newValue);
					keys[index + 1] = children[index + 2]->keys[0];
				}
			}
			
			else if(index != -1 && (children[index]->count < grado/2 || children[index + 2]->count < grado/2)){
				cout<<"MERGE ";	
				if(children[index + 2] == nullptr){//hay que arreglar esto
					cout<<"1 "<<endl;
					for(int i=0;i<children[index]->count;i++){
						current->insertNotFull(children[index]->keys[i]);
					}
					leaf = true;
					deleteFromLeaf(keys[index]);
					leaf = false;

					int start = index;
					for(int i = start; i < count -1;i++){
						children[i] = children[i  + 1];
					}
					children[index] = current;
						
				}
				else{
					cout<<"2 "<<endl;
					for(int i=0;i<children[index + 2]->count;i++){
						cout<<"agregando "<<children[index]->keys[i]<<"\n";
						current->insertNotFull(children[index + 2]->keys[i]);
					}
					leaf = true;
					deleteFromLeaf(keys[index + 1]);
					leaf = false;

					int start = index + 1;
					for(int i=start;i<count-1;i++){
						children[i] = children[i + 1];
					}
					current->next = children[index + 2]->next;
					
				}
			}
			cout<<"key "<<key<<"\ncurrent count"<<current->count<<"\nizquierda count "<<children[index]->count<<"\nderecha count "<<children[index+2]->count<<endl<<endl;
		}

	}





	void display(){
		NodeBplus<T,grado>* nodito = children[0]->children[0];

		while(nodito != nullptr){
			for(int i=0;i<nodito->count;i++){
				cout<<nodito->keys[i]<<" ";
			}
			cout<<"-> ";
			nodito = nodito->next;
		}
		
	}
	NodeBplus* search(T key){
		NodeBplus* nodito = this;

		while(nodito->children[0] != nullptr){
			int index = nodito->count -1;
			while(index >= 0 && nodito->keys[index]> key){
				index--;
			}
			//cout<<nodito->keys[index]<<"  ";
			index++;
			if(nodito->keys[index] == key && nodito->leaf == true){
				break;
			}
			if(nodito->leaf == true && nodito->keys[index] != key){
				nodito ==  nullptr;
				break;
			}
			else{
				nodito = nodito->children[index];
			}
			return nodito;
		}

	}
};
