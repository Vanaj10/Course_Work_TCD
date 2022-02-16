#include "t2.h"
#include <limits.h>

#include <stdio.h>
#include <stdlib.h>

Graph* create_graph(int num_nodes) {
    // Allocate memory for graph's adj matrix
    Graph* graph = (Graph*) malloc(sizeof(Graph));
    graph->numNodes = num_nodes;
    
    graph->matrix = malloc(num_nodes * sizeof(graph->matrix));
    for (int i = 0; i < num_nodes; i++) {
        graph->matrix[i] =malloc(num_nodes * sizeof(graph->matrix[i]));
    }

    // Fill matrix with initial values as -1 (path does not exist)
    for (int i = 0; i < num_nodes; i++)
        for (int j = 0; j < num_nodes; j++)
            graph->matrix[i][j] = -1;
    
    // Fill matrix diagonal with zeros as path from node to itself is zero
    for (int i = 0; i < num_nodes; i++)
        graph->matrix[i][i] = 0;
    
    return graph;
}

void add_edge(Graph *g, int from, int to, int weight) {
    g->matrix[from][to] = weight;
    g->matrix[to][from] = weight;
}

void delete_graph(Graph* g){
    for (int i = 0; i < g->numNodes; i++) {
        free(g->matrix[i]);
    }
    free(g->matrix);
    free(g);
    // printf("Graph memory cleared\n"); 
}

// Utility functions
int minDistance(int *d, int *v, int len) {   // returns min distance node which hasn't been visited
    int min = INT_MAX, idx;
    for (int i = 0; i < len; i++) {
        if (v[i] == 0 && d[i] < min) {
            min = d[i];
            idx = i;
        }
    }

    return idx;
}

void printMatrix(Graph *g) {
    for (int i = 0; i < g->numNodes; i++) {
        for (int j = 0; j < g->numNodes; j++) {
            printf("%15d", g->matrix[i][j]);
        }
        printf("\n\n");
    }
}

void printArray(int *arr, int len) {
    for (int i = 0; i < len; i++)
        printf("%5d", arr[i]);
    printf("\n");
}

// Dijkstra algorithm
void dijkstra(Graph* g, int origin) {
    int nodes = g->numNodes;

    int *distance = (int*) malloc(nodes * sizeof(int));    // min distance for each node
    int *visited = (int*) malloc(nodes * sizeof(int));    
    // int *selected = malloc(nodes * sizeof(int));

    // Set distance to all vertices as INFINITY and visited as -1
    for (int i = 0; i < nodes; i++) {
        if (i != origin)
            distance[i] = INT_MAX;
        visited[i] = 0;
    }

    distance[origin] = 0;

    // Iterate over all vertices
    for (int i = 0; i < nodes; i++) {
        // printArray(distance, nodes);
        // printArray(visited, nodes);

        int currNode = minDistance(distance, visited, nodes);
        visited[currNode] = 1;  // Set current node as visited
        printf("%c ", currNode + 65);

        // Check current node against all other unvisited vertices
        for (int i = 0; i < nodes; i++) {
            if (visited[i] == 0 && g->matrix[currNode][i] != -1 && distance[currNode] != INT_MAX && distance[currNode] + g->matrix[currNode][i] < distance[i])
                distance[i] = distance[currNode] + g->matrix[currNode][i];
        }
    }
    printf("\n");

    // Print final distances
    for (int i = 0; i < nodes; i++) {
        printf("The length of the shortest path between %c and %c is %d\n", origin + 65, i + 65, distance[i]);
    }
    free(distance);
    free(visited);
}


// TEST ONLY, DELETE LATER
// ----------------------------------------------------------------------------------------------------------------
// converts an upper case character (A-Z) to a numerical value (between 0 and 25) 
// static int char2val ( char c ) {
// 	if ( c < 'A' || c > 'Z' ) {
// 		return -1;
// 	}

// 	return c-'A';
// }

// int main(){
//     int num_nodes = 7;
//     // int num_nodes = 5;
//     Graph *graph = create_graph(num_nodes);
   
//     add_edge(graph, char2val('A'), char2val('B'),1);
//     add_edge(graph, char2val('A'), char2val('C'),3);
//     add_edge(graph, char2val('A'), char2val('F'), 10);
//     add_edge(graph, char2val('B'), char2val('G'), 2);
//     add_edge(graph, char2val('B'), char2val('D'), 7);
//     add_edge(graph, char2val('B'), char2val('E'), 5);
//     add_edge(graph, char2val('B'), char2val('C'), 1);
//     add_edge(graph, char2val('C'), char2val('D'), 9);
//     add_edge(graph, char2val('C'), char2val('E'), 3);
//     add_edge(graph, char2val('D'), char2val('G'), 12);
//     add_edge(graph, char2val('D'), char2val('F'), 1);
//     add_edge(graph, char2val('D'), char2val('E'), 2);
//     add_edge(graph, char2val('E'), char2val('F'), 2);

//     // add_edge(graph, char2val('B'), char2val('C'), 2);
//     // add_edge(graph, char2val('B'), char2val('D'), 5);
//     // add_edge(graph, char2val('B'), char2val('E'), 6);
//     // add_edge(graph, char2val('B'), char2val('A'), 4);
//     // add_edge(graph, char2val('C'), char2val('D'), 1);
//     // add_edge(graph, char2val('A'), char2val('E'), 3);

//     // printMatrix(graph);
//     dijkstra(graph, char2val('A'));
//     delete_graph(graph);
// }