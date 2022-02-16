
#ifndef T3_H_
#define T3_H_

typedef struct Node {
    int stopId;
    char *name;
    char *latitude;
    char *longitude;
} Node;

typedef struct Graph{
    int numNodes;
    int **matrix;
} Graph;

typedef struct Edge{
    int weight;
    int startId;
    int endId;
} Edge;


int load_edges ( char *fname ); //loads the edges from the CSV file of name fname
int load_vertices ( char *fname );  //loads the vertices from the CSV file of name fname
void shortest_path(int startNode, int endNode); // prints the shortest path between startNode and endNode, if there is any
void free_memory ( void ) ; // frees any memory that was used

#endif