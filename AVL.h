#pragma once
#include<iostream>
#include<fstream>
#include<string>




namespace avl
{
	using namespace std;

	template<typename T>
	//Nde struct
	struct Node {
		T data;
		size_t fileno;
		Node<T>* left;
		Node<T>* right;
		int height;
		Node(T dat) {
			data = dat;
			left = right = nullptr;
			height = 0;
		}

	};

	template <class T>
	class Tree {
	private:

		
		//Calculating balance factor
		int balanc_fact(Node<T>* node) {
			return get_height(node->left) - get_height(node->right);
		}
		int get_height(Node<T>* node) {
			if (node == nullptr)
				return-1;
			else
				return node->height;
		}
		void maintain_bal(Node<T>*& node, T data) {
			//node is null i.e no need to balance
			if (node == nullptr)
				return;
			//if left subtree is heavy
			if (balanc_fact(node) == 2) {
				//single right rotation (left child's left subtree is heavy)
				if (get_height(node->left->left) >= get_height(node->left->right))
					RR(node);

				//double rotation (left child's right subtree is heavy)
				else
					RL(node);
			}

			//if right subtree is heavy
			if (balanc_fact(node) == -2) {
				//single left rotation (right child's right subtree is heavy)
				if (get_height(node->right->right) >= get_height(node->right->left))
					LL(node);

				//double rotation (right child's left subtree is heavy)
				else
					LR(node);
			}
			//set height
			node->height = max(node->left, node->right) + 1;
		}
		//return max height
		int max(Node<T>*& node1, Node<T>*& node2) {
			if (get_height(node1) > get_height(node2))
				return get_height(node1);
			else
				return get_height(node2);
		}

		//interface for inserting
		void insert_fun(Node<T>*& node, T data , size_t filenum) {

			if (node == nullptr)
			{
				node = new Node<T>(data);
				node->fileno = filenum;
			}
			else if (data < node->data)
			{
				insert_fun(node->left, data, filenum);
			}
			else if (data > node->data)
			{
				insert_fun(node->right, data, filenum);
			}
				
			//balance the tree if there are any violations
			maintain_bal(node, data);

		}

		void RR(Node<T>*& node) {
			//Perform right rotation
			Node<T>* temp_node;
			temp_node = node->left;
			node->left = temp_node->right;
			temp_node->right = node;

			//get max height from right and left subtree and assign
			node->height = max(node->left, node->right) + 1;

			//get max height from right and left subtree and assign
			temp_node->height = max(temp_node->left, node) + 1;

			//Replace node with temp_node
			node = temp_node;

		}
		void LL(Node<T>*& node) {
			//Perform left rotation
			Node<T>* temp_node;
			temp_node = node->right;
			node->right = temp_node->left;
			temp_node->left = node;

			//get max height from right and left subtree and assign
			node->height = max(node->left, node->right) + 1;

			//get max height from right and left subtree and assign
			temp_node->height = max(temp_node->right, node) + 1;

			//Replace node with temp_node
			node = temp_node;
		}

		void RL(Node<T>*& node) {
			//first perform left rotation on right subtree
			LL(node->left);

			//perform right rotation on node
			RR(node);
		}
		void LR(Node<T>*& node) {
			//first perform right rotation on right subtree
			RR(node->right);

			//perform left rotation on node
			LL(node);
		}
		// find min node
		Node<T>*& getMin_func(Node<T>*& node) {
			if (node->left == nullptr)
				return node;
			getMin_func(node->left);
		}

		void deletion(Node<T>*& node) {
			Node<T>* temp_node;
			//left subtree is NULL
			if (!node->left) {
				temp_node = node;
				node = node->right;
				delete temp_node;
			}
			//right subtree is NULL
			else if (!node->right) {
				temp_node = node;
				node = node->left;
				delete temp_node;
			}
			//both children exist
			else {
				//get right subtree's min node
				temp_node = getMin_func(node->right);
				//give the node the value of right subtree's min value
				node->data = temp_node->data;
				//delete temp_node which will reassign left subtree's right subtree (if present) to left subtree's parent
				delete_fun(node->right, temp_node->data);
			}
		}

		void delete_fun(Node<T>*& node, T data) {
			// data not found
			if (node == nullptr)
				return;
			//data is less than node's data
			if (data < node->data)
				delete_fun(node->left, data);
			//data is greater than node's data
			else if (data > node->data)
				delete_fun(node->right, data);
			// node is found
			else if (data == node->data) {
				deletion(node);
			}

			//balance the tree if there are any violations
			maintain_bal(node, data);
		}

		void PreOrderTraversal_func(Node<T>* node) {
			if (node == nullptr)
				return;
			cout << node->data << "\n";
			PreOrderTraversal_func(node->left);
			PreOrderTraversal_func(node->right);
		}

		void PostOrderTraversal_func(Node<T>* node) {
			if (node == nullptr)
				return;
			PostOrderTraversal_func(node->left);
			PostOrderTraversal_func(node->right);
			cout << node->data << "\n";
		}

		void InOrderTraversal_func(Node<T>* node) {
			if (node == nullptr)
				return;
			InOrderTraversal_func(node->left);
			cout << node->data << "\n";
			InOrderTraversal_func(node->right);
		}

public:
		bool search_fun(Node<T>* root, T key)
		{
			if (root == NULL)
				return false;
			else if (key == root->data)
				return true;
			else if (key < root->data)
				return search_fun(root->left, key);
			else if (key > root->data)
				return search_fun(root->right, key);
		}
		size_t pointsearch_fun(Node<T>* root, T key)
		{
			if (root == NULL)
				return 0;
			else if (key == root->data)
				return root->fileno;
			else if (key < root->data)
				return pointsearch_fun(root->left, key);
			else if (key > root->data)
				return pointsearch_fun(root->right, key);
		}


		string keyname;
		Node<T>* root;
		Tree() {
			root = nullptr;
		}
		void setkey(string key)
		{
			keyname = key;
		}
		void PreOrderTraversal()
		{
			PreOrderTraversal_func(root);
		}
		void PostOrderTraversal()
		{
			PostOrderTraversal_func(root);
		}
		void InOrderTraversal()
		{
			InOrderTraversal_func(root);
		}

		void insert(T data,size_t nodeiterator) {
			insert_fun(root, data,nodeiterator);
		}
		void remove(T data) {
			delete_fun(root, data);
		}
		bool search(T data) {
			return search_fun(root, data);
		}
		size_t pointsearch(T data) {
			return pointsearch_fun(root, data);
		}
	};

}
