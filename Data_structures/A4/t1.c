#include <stdio.h>
#include <stdlib.h>
#include"t1.h"

// #define SIZE 40;
struct queue {
  int items[40];
  int front;
  int rear;
};

int DFScallonce  = 0; // Creating a flag variable to print "DFS: " once in the output(Recursive function making it print recursively)

// Create a node
struct node* createNode(int v) {
  struct node* newNode = malloc(sizeof(struct node));
  newNode->vertex = v;
  newNode->next = NULL;
  return newNode;
}

// Create graph
Graph* create_graph(int num_nodes) {
  Graph* graph = malloc(sizeof(Graph));
  graph->num_nodes = num_nodes;

  graph->adjLists = malloc(num_nodes * sizeof(struct node*));

  graph->visited = malloc(num_nodes * sizeof(int));

  graph->visitedbfs = malloc(num_nodes * sizeof(int));

  int i;
  for (i = 0; i < num_nodes; i++) {
    graph->adjLists[i] = NULL;
    graph->visited[i] = 0;
    graph->visitedbfs[i] = 0;
  }
  return graph;
}

// Add edge
void add_edge(Graph* graph, int from, int to) {
  struct node* newNode = createNode(to);
  newNode->next = graph->adjLists[from];
  graph->adjLists[from] = newNode;
  // free(newNode);
}

void delete_graph(struct Graph* g){
  free(g->visitedbfs);
  free(g->visited);
  for (int i = 0; i < g->num_nodes; i++)
    free(g->adjLists[i]);
  free(g->adjLists);
  free(g);
}

// DFS algorithm
void dfs(Graph* g, int origin) {
    if(DFScallonce == 0)
        printf("DFS:");
    struct node* adjList = g->adjLists[origin];
    struct node* temp = adjList;

    g->visited[origin] = 1;
    char charValue = origin + 65;
    printf("  %c", charValue);

    while (temp != NULL) {
        int connectedVertex = temp->vertex;

    if (g->visited[connectedVertex] == 0) {
        DFScallonce = 1;
        dfs(g, connectedVertex);
    }
    temp = temp->next;
    }

}




void bfs(struct Graph* graph, int startVertex) {
  printf("\n");
  printf("BFS ");
  struct queue* q = createQueue();

  graph->visitedbfs[startVertex] = 1;
  addToQueue(q, startVertex);

  while (!isEmpty(q)) {
    // printQueue(q);
    int currentVertex = removeFromQueue(q);
    // for (int i = 0; i < graph->num_nodes; i++) {
    //     graph->visited[i] = 0;
    //   }
    char charValue = currentVertex + 65;
    printf("  %c", charValue);
    // printf("Visited %d\n", charValue);

    struct node* temp = graph->adjLists[currentVertex];

    while (temp) {
      int adjVertex = temp->vertex;

      if (graph->visitedbfs[adjVertex] == 0) {
        graph->visitedbfs[adjVertex] = 1;
        addToQueue(q, adjVertex);
      }
      temp = temp->next;
    }
  }
  printf(" \n");
}

// Create a queue
struct queue* createQueue() {
  struct queue* q = malloc(sizeof(struct queue));
  q->front = -1;
  q->rear = -1;
  return q;
}

// Check if the queue is empty
int isEmpty(struct queue* q) {
  if (q->rear == -1)
    return 1;
  else
    return 0;
}

// Adding elements into queue
void addToQueue(struct queue* q, int value) {
  if (q->rear == 40 - 1)
    printf("\nQueue is Full!!");
  else {
    if (q->front == -1)
      q->front = 0;
    q->rear++;
    q->items[q->rear] = value;
  }
}

// Removing elements from queue
int removeFromQueue(struct queue* q) {
  int item;
  if (isEmpty(q)) {
    // printf("Queue is empty");
    item = -1;
  } else {
    item = q->items[q->front];
    q->front++;
    if (q->front > q->rear) {
      // printf("Resetting queue ");
      q->front = q->rear = -1;
    }
  }
  return item;
}

void printQueue(struct queue* q) {
  int i = q->front;

  if (isEmpty(q)) {
    printf("Queue is empty");
  } else {
    printf("\nQueue contains \n");
    // for (i = q->front; i < q->rear + 1; i++) {
      // printf("%d ", q->items[q->rear-1]);
    // }
  }
}










// static int char2val ( char c ) {
// 	if ( c < 'A' || c > 'Z' ) {
// 		return -1;
// 	}

// 	return c-'A';
// }


// int main(){
//     int num_nodes = 6;
//     Graph *graph = create_graph(num_nodes);
   
//     add_edge(graph, char2val('A'), char2val('E'));
//     add_edge(graph, char2val('B'), char2val('D'));
//     add_edge(graph, char2val('A'), char2val('D'));
//     add_edge(graph, char2val('A'), char2val('B'));
//     add_edge(graph, char2val('B'), char2val('C'));
//     add_edge(graph, char2val('C'), char2val('B'));
//     add_edge(graph, char2val('E'), char2val('D'));
//     add_edge(graph, char2val('D'), char2val('F'));
//     add_edge(graph, char2val('F'), char2val('C'));
//     add_edge(graph, char2val('C'), char2val('D'));

//     dfs(graph, char2val('A'));
//     bfs(graph, char2val('A'));

//     delete_graph(graph);
//     // printGraph(graph);

// }