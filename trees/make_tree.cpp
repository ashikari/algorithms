#include <iostream>
#include <vector>
#include <stack>
#include <queue>

class Node{
public:
	int data;
	Node *right, *left;
};


void add_element(Node* root, int elem){
	Node* new_node = new Node;
	new_node->data = elem;
	Node* ptr = root;
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


Node* build_tree(const std::vector<int> & elements){
	Node* root = new Node;
	root->data = elements[0];

	for(int i = 1; i<elements.size(); i++){
		add_element(root, elements[i]);
	}
	return root;
}


void print_dfs(Node* const root){
	std::stack<Node*> s;
	Node* ptr = root;
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


void print_bfs(Node* const root){
	std::queue<Node*> q;
	Node* ptr = root;
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


void printPrettyBFS(Node* const root){
	//container construction
	std::queue<Node*> q1, q2;
	std::vector<std::queue<Node*>> qs = {q1, q2};
	
	//Level Label
	int level = 0;
	//queue control index
	bool whichQ = false;
	//tmp node pointer
	Node* ptr;

	//initialization
	qs[0].push(root);

	//Pretty printing
	while(not (qs[0].empty() and qs[1].empty())){
		std::cout<<"Level "<<level<<": ";
		while(!qs[whichQ].empty()){
			ptr = qs[whichQ].front();
			std::cout<<ptr->data;
			qs[whichQ].pop();
			
			//delimiters
			if(!qs[whichQ].empty()){ 
				std::cout<<", ";
			}

			if(ptr->right != nullptr){
				qs[!whichQ].push(ptr->right);
			}
			if(ptr->left != nullptr){
				qs[!whichQ].push(ptr->left);
			}
		}
		level++;
		whichQ = not(whichQ);
		std::cout<<std::endl;
	}
}


int main(){
	std::vector<int> v = {5, 2, 1, 3, 7, 6 , 8};
	Node* root = build_tree(v);
	std::cout<<"BREATH FIRST: "<<std::endl;
	print_bfs(root);

	std::cout<<"\nDEPTH FIRST: "<<std::endl;
	print_dfs(root);

	std::cout<<"\nPRETTY BREATH FIRST"<<std::endl;
	printPrettyBFS(root);

	return 0;
}