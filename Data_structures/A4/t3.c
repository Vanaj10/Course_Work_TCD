#include "t3.h"
#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_BUFFER_SIZE 1024
#define MAX_NODES 8000
#define elementSize 256

// Global graph variable and list of nodes
Graph* graph;
Node *nodes[MAX_NODES];

Node *getNode(char buffer[]) {
    // printf("\n%s\n", buffer);
    int column = 0;
    int foundQuote = 0;
    char element[elementSize];
    memset(element, 0, elementSize); // Clear variable to build next field

    // Result Game struct
    Node *node = malloc(sizeof(Node));

    // Iterate over the buffer and print column elements
    for (int i = 0; i < strlen(buffer); i++)
    {
        switch (buffer[i])
        {
        case ',':
            if (!foundQuote)
            {
                // Column data present in element
                // printf("Column data -> %s, Found Quote %d\n", element, foundQuote);

                // Store this element in struct!
                // TODO: This part can be abstracted into a neat function...
                switch (column)
                {
                case 0:
                    node->stopId = atoi(element);
                    break;
                case 1:
                    node->name = strdup(element);
                    break;
                case 2:
                    node->latitude = strdup(element);
                    break;
                case 3:
                    node->longitude = strdup(element);
                    break;
                default:
                    break;
                }
                column++;
                // Store this in our struct.
                memset(element, 0, elementSize); // Clear variable to build next field
            }
            break;
        case '\"':
            foundQuote = !foundQuote;
            continue;
        default:
            strcat(element, (char[2]){buffer[i], '\0'});
            break;
        }
    }

    node->longitude = strdup(element);
    return node;
}

Edge *getEdge(char buffer[]) {
    // printf("\n%s\n", buffer);
    int column = 0;
    int foundQuote = 0;
    char element[elementSize];
    memset(element, 0, elementSize); // Clear variable to build next field

    // Result Game struct
    Edge *edge = malloc(sizeof(Edge));

    for (int i = 0; i < strlen(buffer); i++)
    {
        switch (buffer[i])
        {
        case ',':
            if (!foundQuote)
            {
                // Column data present in element
                // printf("Column data -> %s, Found Quote %d\n", element, foundQuote);

                // Store this element in struct!
                // TODO: This part can be abstracted into a neat function...
                switch (column)
                {
                case 0:
                    // graph->stopId = atoi(element);
                    edge->startId = atoi(element);
                    break;
                case 1:
                    // node->name = strdup(element);
                    edge->endId = atoi(element);
                    break;
                case 2:
                    // node->latitude = strdup(element);
                    edge->weight = atoi(element);
                    break;
                default:
                    break;
                }
                column++;
                // Store this in our struct.
                memset(element, 0, elementSize); // Clear variable to build next field
            }
            break;
        case '\"':
            foundQuote = !foundQuote;
            continue;
        default:
            strcat(element, (char[2]){buffer[i], '\0'});
            break;
        }
    }

    edge->weight = atoi(element);
    return edge;
}

void printMatrix(Graph *g) {
    for (int i = 0; i < 25; i++) {
        for (int j = 0; j < 25; j++) {
            printf("%5d", g->matrix[i][j]);
        }
        printf("\n\n");
    }
}

//loads the vertices from the CSV file of name fname
int load_vertices ( char *fname ) {
    FILE *file = fopen(fname, "r");

    int numNodes = 0;
    if (!file)
        printf("Can not open the File\n");
    else {
        char buffer[MAX_BUFFER_SIZE];
        int isFirst = 1;

        while (fgets(buffer, 1024, file))
        {
            if (isFirst)
            {
                isFirst = 0;
                continue;
            }

            Node *node = getNode(buffer);
            nodes[node->stopId] = node;
            numNodes++;
        }
    }
    printf("Loaded %d vertices\n", numNodes);
    
    return numNodes;
}

int load_edges ( char *fname ) {
    // Before we load edges, build graph
    // Allocate memory for graph's adj matrix
    graph = (Graph*) malloc(sizeof(Graph));
    graph->numNodes = MAX_NODES;
    
    graph->matrix = malloc(MAX_NODES * sizeof(graph->matrix));
    for (int i = 0; i < MAX_NODES; i++) {
        graph->matrix[i] =malloc(MAX_NODES * sizeof(graph->matrix[i]));
    }

    // TODO: Fill everything with -1 except actual nodes
    // Fill matrix with initial values as -1 (path does not exist)
    for (int i = 0; i < MAX_NODES; i++)
        for (int j = 0; j < MAX_NODES; j++)
            graph->matrix[i][j] = -1;
    
    // Fill matrix diagonal with zeros as path from node to itself is zero
    for (int i = 0; i < MAX_NODES; i++)
        graph->matrix[i][i] = 0;

    // Graph is built. Add edges!
    // printf("INFO: Base graph built. Loading edges...\n");

    FILE *file = fopen(fname, "r");
    int numEdges = 0;
    if (!file)
        printf("Can not open the File\n");
    else {
        char buffer[MAX_BUFFER_SIZE];
        int isFirst = 1;

        while (fgets(buffer, 1024, file))
        {
            if (isFirst)
            {
                isFirst = 0;
                continue;
            }

            Edge *edge = getEdge(buffer);
            // Edge data parsed. Add this to graph's adj matrix
            graph->matrix[edge->startId][edge->endId] = edge->weight;
            graph->matrix[edge->endId][edge->startId] = edge->weight;

            numEdges++;
        }
    }
    printf("Loaded %d edges\n", numEdges);

    return numEdges;
}

int minDistance(int *d, int *v, int len) {   // returns min distance node which hasn't been visited
    int min = INT_MAX, idx;
    for (int i = 0; i < len; i++) {
        if (v[i] == 0 && d[i] < min) {
            min = d[i];
            idx = i;
        }
    }
    // printf("Returning idx %d\n", idx);
    return idx;
}

void printArray(int *arr, int len) {
    for (int i = 0; i < len; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

void printPath(int *lastNodes, int start, int end) {
    int pathIdx = 0;
    int reversePath[MAX_NODES] = {0};

    int currNode = end; // Start from the back
    
    while (currNode != start) {
        reversePath[pathIdx] = currNode;
        currNode = lastNodes[currNode];
        pathIdx++;
    }

    // printf("Total In Path: %d\n", pathIdx);

    printf("%5d %20s %15s %15s\n", nodes[start]->stopId, nodes[start]->name, nodes[start]->latitude, nodes[start]->longitude);
    for (int i = pathIdx - 1; i >= 0; i--) {
        int id = reversePath[i];
        printf("%5d %20s %15s %15s\n", nodes[id]->stopId, nodes[id]->name, nodes[id]->latitude, nodes[id]->longitude);
    }
    printf("\n");
}

void free_memory() {
    for (int i = 0; i < MAX_NODES; i++) {
        free(graph->matrix[i]);
    }
    free(graph->matrix);
    free(graph);
    // printf("Graph memory cleared\n");    
}

// Shortest path using Dijkstra's algorithm 
void shortest_path(int startNode, int endNode) {
    int *distance = (int*) malloc(MAX_NODES * sizeof(int));    // min distance for each node
    int *visited = (int*) malloc(MAX_NODES * sizeof(int));
    int *lastNode = (int*) malloc(MAX_NODES * sizeof(int));    

    // Set distance to all vertices as INFINITY and visited as -1
    for (int i = 0; i < MAX_NODES; i++) {
        if (i != startNode)
            distance[i] = INT_MAX;
        visited[i] = 0;
        lastNode[i] = -1;
    }

    distance[startNode] = 0;

    // Iterate over all vertices
    for (int i = 0; i < MAX_NODES; i++) {
        int currNode = minDistance(distance, visited, MAX_NODES);
        visited[currNode] = 1;  // Set current node as visited

        // Check current node against all other unvisited vertices
        for (int i = 0; i < MAX_NODES; i++) {
            if (visited[i] == 0 && graph->matrix[currNode][i] != -1 && distance[currNode] != INT_MAX && distance[currNode] + graph->matrix[currNode][i] < distance[i]) {
                distance[i] = distance[currNode] + graph->matrix[currNode][i];
                lastNode[i] = currNode;
            }
        }
    }

    // printf("Last node for 497 is %d\n", lastNode[497]);
    printPath(lastNode, startNode, endNode);

    // Free memory
    free(distance);
    free(visited);
    free(lastNode);
}

// int main(int argc, char **argv) {
//     if (argc < 2)
//     {
//         printf("Filename not provided. Exiting...\n");
//         return 1;
//     }
//     char *vertFile = argv[1];
//     char *edgeFile = argv[2];

//     int total = load_vertices(vertFile);
//     printf("Vertices loaded: %d\n", total);

//     // for (int i = 0; i < 50; i++) { 
//     //     if (nodes[i]) {
//     //         printf("Node Index %d: StopId: %d, Name - %s\n", i, nodes[i]->stopId, nodes[i]->name);
//     //     }
//     // }


//     int totalEdges = load_edges(edgeFile);
//     printf("Edges loaded: %d\n", totalEdges);

//     shortest_path(300, 253);
    
//     free_memory();
//     return 0;
// }
