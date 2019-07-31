#include <iostream>
#include <vector>
#include <stack>
#include <queue>

class node{
public:
	int data;
	node *right, *left;
};


void add_element(node* root, int elem){
	node* new_node = new node;
	new_node->data = elem;
	node* ptr = root;
	bool placed = false;
	while(!placed){
		if(ptr->data>elem){
			if(ptr->left==nullptr){
				ptr->left = new_node;
				placed = true;
			} 
			else{
				ptr = ptr->left;
			}
		}
		else{
			if(ptr->right==nullptr){
				ptr->right = new_node;
				placed = true;
			} 
			else{
				ptr = ptr->right;
			}
		}
	}
}

node* build_tree(std::vector<int> & elements){
	node* root = new node;
	root->data = elements[0];

	for(int i = 1; i<elements.size(); i++){
		add_element(root, elements[i]);
	}
	return root;
}

void print_dfs(node* root){
	std::stack<node*> s;
	node* ptr = root;
	s.push(root);
	while(!s.empty()){
		ptr = s.top();
		s.pop();
		if(ptr->right != nullptr){
			s.push(ptr->right);
		}
		if(ptr->left != nullptr){
			s.push(ptr->left);
		}
		std::cout<<ptr->data<<std::endl;
	}
}

void print_bfs(node* root){
	std::queue<node*> q;
	node* ptr = root;
	q.push(root);
	while(!q.empty()){
		ptr = q.front();
		q.pop();
		if(ptr->right != nullptr){
			q.push(ptr->right);
		}
		if(ptr->left != nullptr){
			q.push(ptr->left);
		}
		std::cout<<ptr->data<<std::endl;
	}

}

int main(){
	std::vector<int> v = {5,2, 1, 3, 7, 6 , 8};
	node* root = build_tree(v);
	print_bfs(root);
	print_dfs(root);
	return 0;
}