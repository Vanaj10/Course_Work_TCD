#include <stdio.h>
#include <stdlib.h>
#include "bst.h"

Tree_Node *root = NULL;

Tree_Node *newNode(char data)
{
    Tree_Node *node = (Tree_Node *)malloc(sizeof(Tree_Node));
    node->data = data;
    node->left = node->right = NULL;
    return node;
}

void tree_insert(Tree_Node **root, char data)
{
    //TODO
    if ((*root) == NULL)
    {
        (*root) = newNode(data);
        return 0;
    }

    if (data > (*root)->data)
    {
        tree_insert(&(*root)->right, data);
    }
    else
    {
        tree_insert(&(*root)->left, data);
    }
}

Tree_Node *create_bst(char data[])
{
    // TODO
    Tree_Node *root = NULL;
    int i = 0;
    while (data[i] != '\0')
    {
        tree_insert(&root, data[i]);
        i++;
    }
    return root;
}

Tree_Node *tree_search(Tree_Node *root, char data)
{
    //TODO
    if (root == NULL || root->data == data)
        return root;
    // else if (root->data == data)
    //     return root;
    // Recursively calling search until we find the data required to be found
    if (root->data < data)
        return tree_search(root->right, data);
    return tree_search(root->left, data);
}

void tree_print_sorted(Tree_Node *root)
{
    //TODO
    if (root != NULL)
    {
        tree_print_sorted(root->left);
        printf("%c", root->data);
        tree_print_sorted(root->right);
    }
}

void tree_delete(Tree_Node *root)
{
    //TODO
    if (root != NULL)
    {
        tree_delete(root->left);
        tree_delete(root->right);
        free(root);
    }
}

 