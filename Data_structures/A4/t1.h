#ifndef T1_H_
#define T1_H_
#include<stdbool.h>

struct node {
  int vertex;
  struct node* next;
  int mark;
};



struct node* createNode(int v);

typedef struct Graph {
  int num_nodes;
  int* visited;
  int* visitedbfs;

  // We need int** to store a two dimensional array.
  // Similary, we need struct node** to store an array of Linked lists
  struct node** adjLists;
}Graph;

Graph* create_graph(int num_nodes); // creates a graph with num_nodes nodes, assuming nodes are stored in alphabetical order (A, B, C..)
void add_edge(struct Graph *g, int from, int to); // adds a directed edge
void bfs(struct Graph* g, int origin); //implements breath first search and prints the results
void dfs(struct Graph* g, int origin); //implements depth first search and prints the results
void delete_graph(struct Graph* g); // Deletes the graph

struct queue* createQueue(); 
void addToQueue(struct queue* q, int); // Adding elements to queue
int removeFromQueue(struct queue* q); // Removing elements from queue
int isEmpty(struct queue* q); // Checking if the queue is empty


#endif