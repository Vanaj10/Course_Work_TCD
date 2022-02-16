#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<stdbool.h>

#include "bstdb.h"

// Write your submission in this file
//
// A main function and some profiling tools have already been set up to test
// your code in the task2.c file. All you need to do is fill out this file
// with an appropriate Binary Search Tree implementation.
//
// The input data will be books. A book is comprised of a title and a word
// count. You should store both these values in the tree along with a unique
// integer ID which you will generate.
//
// We are aiming for speed here. A BST based database should be orders of
// magnitude faster than a linked list implementation if the BST is written
// correctly.
//
// We have provided an example implementation of what a linked list based
// solution to this problem might look like in the db/listdb.c file. If you
// are struggling to understand the problem or what one of the functions
// below ought to do, consider looking at that file to see if it helps your
// understanding.
//
// There are 6 functions you need to look at. Each is provided with a comment
// which explains how it should behave. The functions are:
//
//  + bstdb_init
//  + bstdb_add
//  + bstdb_get_word_count
//  + bstdb_get_name
//  + bstdb_stat
//  + bstdb_quit
//
// Do not rename these functions or change their arguments/return types.
// Otherwise the profiler will not be able to find them. If you think you
// need more functionality than what is provided by these 6 functions, you
// may write additional functions in this file.


struct my_bst
{
	int doc_id;
	int height;
	char *name;
	int word_count;
	struct my_bst *left;
	struct my_bst *right;
};

struct my_bst *root; // Root of the tree
int next_id;		 // ID of th next document

int g_num_comps;
int g_num_searches;
int num_nodes = 0;


void tree_delete(struct my_bst *root)
{
	//TODO
	if (root != NULL)
	{
		tree_delete(root->left);
		tree_delete(root->right);
		free(root);
		root = NULL;
	}
}

// A utility function to get the height of the tree
int height(struct my_bst *N)
{
	if (N == NULL)
		return 0;
	return N->height;
}

// A utility function to get maximum of two integers
int max(int a, int b)
{
	return (a > b) ? a : b;
}

struct my_bst *newNode(int doc_id, char *name, int word_count)
{
  num_nodes++;
	struct my_bst *node = (struct my_bst *)malloc(sizeof(struct my_bst)); // Allocate memory to tree node
	size_t len_name;

	if (node)
	{
		// Store data in the list node
		node->doc_id = doc_id;
		node->word_count = word_count;
		node->height = 1;
		node->left = node->right = NULL;

		// Allocate memory to store the file name and copy the string into the
		// new list node
		len_name = strlen(name) + 1;
		node->name = calloc(len_name, sizeof(char));
		if (node->name)
		{
			// if calloc was successful, copy the filename into the node
			strcpy(node->name, name);
		}
		else
		{
			// if calloc failed, release any memory that was allocated and
			// report an error by returning NULL
			node->left = node->right = NULL;
			tree_delete(node);
			node = NULL;
		}
	}

	return node;
}

// Get Balance factor of node N
int getBalance(struct my_bst *N)
{
	if (N == NULL)
		return 0;
	return height(N->left) - height(N->right);
}

// A utility function to right rotate subtree rooted with y

struct my_bst *rightRotate(struct my_bst *y)
{
	struct my_bst *x = y->left;
	struct my_bst *T2 = x->right;

	// Perform rotation
	x->right = y;
	y->left = T2;

	// Update heights
	y->height = max(height(y->left), height(y->right)) + 1;
	x->height = max(height(x->left), height(x->right)) + 1;

	// Return new root
	return x;
}

// A utility function to left rotate subtree rooted with x

struct my_bst *leftRotate(struct my_bst *x)
{
	struct my_bst *y = x->right;
	struct my_bst *T2 = y->left;

	// Perform rotation
	y->left = x;
	x->right = T2;

	//  Update heights
	x->height = max(height(x->left), height(x->right)) + 1;
	y->height = max(height(y->left), height(y->right)) + 1;

	// Return new root
	return y;
}

struct my_bst *tree_insert(struct my_bst *node, int doc_id, char *name, int word_count)
{
	//TODO
  
	if (node == NULL)
	{
		return (newNode(doc_id, name, word_count));
	}

	if (doc_id > node->doc_id)
	{
		node->right = tree_insert(node->right, doc_id, name, word_count);
	}
	else
	{
		node->left = tree_insert(node->left, doc_id, name, word_count);
	}

	/* Update height of this ancestor node */
	node->height = 1 + max(height(node->left),
						   height(node->right));

	/* Get the balance factor of this ancestor
          &(*node) to check whether this &(*node) became
          unbalanced */
	int balance = getBalance(node);

	// If this &(*node) becomes unbalanced, then
	// there are 4 cases

	// Left Left Case
	if (balance > 1 && doc_id < node->left->doc_id)
		return rightRotate(node);

	// Right Right Case
	if (balance < -1 && doc_id > node->right->doc_id)
		return leftRotate(node);

	// Left Right Case
	if (balance > 1 && doc_id > node->left->doc_id)
	{
		node->left = leftRotate(node->left);
		return rightRotate(node);
	}

	// Right Left Case
	if (balance < -1 && doc_id < node->right->doc_id)
	{
		node->right = rightRotate(node->right);
		return leftRotate(node);
	}

	/* return the (unchanged) node pointer */
	return node;
}

// void tree_delete(struct my_bst *root)
// {
// 	//TODO
// 	if (root != NULL)
// 	{
// 		tree_delete(root->left);
// 		tree_delete(root->right);
// 		free(root);
// 		root = NULL;
// 	}
// }

struct my_bst *search(struct my_bst *root, int doc_id)
{
	g_num_searches++;
	g_num_comps++;
	// root is null or doc_id is present at root
	if (root == NULL || root->doc_id == doc_id)
		return root;

	g_num_comps++;
	// Required doc_id is greater than root's doc_id
	if (root->doc_id < doc_id)
		return search(root->right, doc_id);
	g_num_comps++;
	// Requuired doc_id is smaller than root's doc_id
	return search(root->left, doc_id);
}

int bstdb_init(void)
{
	// This function will run once (and only once) when the database first
	// starts. Use it to allocate any memory you want to use or initialize
	// some globals if you need to. Function should return 1 if initialization
	// was successful and 0 if something went wrong.
	struct my_bst *root = NULL;
	next_id = 0;
  num_nodes = 0;
	return 1;
}

int bstdb_add(char *name, int word_count)
{
	// This function should create a new node in the binary search tree,
	// populate it with the name and word_count of the arguments and store
	// the result in the tree.
	//
	// This function should also generate and return an identifier that is
	// unique to this document. A user can find the stored data by passing
	// this ID to one of the two search functions below.
	//
	// How you generate this ID is up to you, but it must be an integer. Note
	// that this ID should also form the keys of the nodes in your BST, so
	// try to generate them in a way that will result in a balanced tree.
	//
	// If something goes wrong and the data cannot be stored, this function
	// should return -1. Otherwise it should return the ID of the new node
	root = tree_insert(root, next_id, name, word_count);
  
	if (!root)
		return -1;

	return next_id++;
}

int bstdb_get_word_count(int doc_id)
{
	// This is a search function. It should traverse the binary search tree
	// and return the word_count of the node with the corresponding doc_id.
	//
	// If the required node is not found, this function should return -1
	struct my_bst *node = search(root, doc_id);
	if (node != NULL)
		return node->word_count;
	return -1;
}

char *bstdb_get_name(int doc_id)
{
	// This is a search function. It should traverse the binary search tree
	// and return the name of the node with the corresponding doc_id.
	//
	// If the required node is not found, this function should return NULL or 0
	struct my_bst *node = search(root, doc_id);
	if (node != NULL)
		return node->name;
	return 0;
}

	bool isBalanced(struct my_bst* root)
	{
    	int lh; /* for height of left subtree */
   	 	int rh; /* for height of right subtree */
 
  	  /* If tree is empty then return true */
  	  if (root == NULL)
  	      return 1;
 
  	  /* Get the height of left and right sub trees */
  	  lh = height(root->left);
  	  rh = height(root->right);
 
  	  if (abs(lh - rh) <= 1 && isBalanced(root->left) && isBalanced(root->right))
  	      return 1;
 
  	  /* If we reach here then
  	  tree is not height-balanced */
  	  return 0;
	}
bool checkNode(struct my_bst *root)
	{
		if (num_nodes == 2^(root->height))
		return 1;
		else
		return 0;
	}

void bstdb_stat(void)
{
	
	// Use this function to show off! It will be called once after the
	// profiler ends. The profiler checks for execution time and simple errors,
	// but you should use this function to demonstrate your own innovation.
	//
	// Suggestions for things you might want to demonstrate are given below,
	// but in general what you choose to show here is up to you. This function
	// counts for marks so make sure it does something interesting or useful.
	//
	//  + Check if your tree is balanced and print the result
	//
	//  + Does the number of nodes in the tree match the number you expect
	//    based on the number of insertions you performed?
	//
	//  + How many nodes on average did you need to traverse in order to find
	//    a search result?
	//
	//  + Can you prove that there are no accidental duplicate document IDs
	//    in the tree?
	printf("STAT\n");


	//Checking if tree is balanced

	if (isBalanced(root))
        printf("Tree is balanced\n");
    else
        printf("Tree is not balanced\n");



	//Checking if the number of nodes is matching the number of insertions
	
	if (checkNode(root))
		printf("Number of nodes is as exepected -> %d\n", num_nodes);
	else
		printf("Number of nodes is not as exepected\n");



	//Average number of comparisons per search
	printf("Avg comparisons per search  -> %lf\n",
		   (double)g_num_comps / g_num_searches);
}

void bstdb_quit(void)
{
	// This function will run once (and only once) when the program ends. Use
	// it to free any memory you allocated in the course of operating the
	// database.
	struct my_bst *node = root;
	tree_delete(node);
}
// gcc -Wall -g src/task2.c src/bstdb.c src/db/profiler.c src/db/database.c src/db/listdb.c -o task2 -lm
// valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-out.txt ./task2.exe