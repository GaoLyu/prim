/*
 *  Some (very) light testing of our Graph implementation.
 *
 *
 *  Author: A. Tafliovich.
 *
 *  ---------------------------------------------------------------------------
 *   Compile:
 *   gcc -Wall -Werror graph.c minheap.c graph_algos.c graph_tester.c -o tester
 *
 *   Run:
 *   ./tester sample_input.txt
 *
 *   SEE FILE expected_output.txt FOR EXPECTED OUTPUT
 *
 *   Don't forget:
 *   valgrind --show-leak-kinds=all --leak-check=full ./tester sample_input.txt
 *   clang-format -style=Google --dry-run myfile.c
 *   clang-tidy --config-file=if-you-want-custom.txt myfile.c
 *  ---------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graph.h"
#include "graph_algos.h"
#include "minheap.h"

#define MAX_LIMIT 1024

/* functions to create a Graph from a file */
Graph* createGraph(FILE* f);
int readVertexID(char* token, int numVertices);
int readWeight(char* token);
AdjList* addEdge(AdjList* head, int fromVertex, int toVertex, int weight);
bool updateVertex(Graph* graph, char* line);

/* run and print */
void runPrim(Graph* graph, int startVertex);
void runDijkstra(Graph* graph, int startVertex);
int printTree(Edge* mst, int numTreeEdges);
void printPaths(AdjList* paths, int numVertices);

/* cleanup */
void freePaths(AdjList* paths, int numVertices);

int main(int argc, char* argv[]) {
  if (argc == 1) {
    printf("You did not specify an input file. Please, try again.\n");
    return 1;
  }
  FILE* f = fopen(argv[1], "r");
  if (f == NULL) {
    fprintf(stderr, "Unable to open the specified input file: %s\n", argv[1]);
    return 1;
  }

  Graph* graph = createGraph(f);
  fclose(f);

  printGraph(graph);

  runPrim(graph, 0);  // try other vertices!
  runDijkstra(graph, 0);

  deleteGraph(graph);
  return 0;
}

/* Runs Prim's algorithm on 'graph' starting at vertex 'startVertex',
 * and prints the result.
 */
void runPrim(Graph* graph, int startVertex) {
  if (graph == NULL) return;

  int numTreeEdges = graph->numVertices - 1;
  Edge* mst = primGetMST(graph, startVertex);
  if (mst == NULL) return;

  printf("Prim's from %d returned this MST:\n", startVertex);
  int totalWeight = printTree(mst, numTreeEdges);
  printf("Total weight: %d\n\n", totalWeight);

  free(mst);
}

/* Runs Dijkstra's algorithm on 'graph' starting at vertex 'startVertex',
 * runs getPaths on the resuting distance tree, and prints all results.
 */
void runDijkstra(Graph* graph, int startVertex) {
  if (graph == NULL) return;

  Edge* distanceTree = getShortestPaths(graph, startVertex);

  printf("Dijkstra's from %d returned this distance tree:\n", startVertex);
  printTree(distanceTree, graph->numVertices);
  printf("\n");

  AdjList* paths = getPaths(distanceTree, graph->numVertices, startVertex);

  printf("getPaths from %d produced these paths:\n", startVertex);
  printPaths(paths, graph->numVertices);

  freePaths(paths, graph->numVertices);
  free(paths);
  free(distanceTree);
}

/* Creates and returns a new Graph from the information in the file 'f'.
 */
Graph* createGraph(FILE* f) {
  char line[MAX_LIMIT];

  if (!fgets(line, MAX_LIMIT, f)) {  // read first line
    printf("Could not read number of vertices from input file. Giving up.\n");
    return NULL;
  }

  int numVertices = atoi(line);  // first line is number of vertices
  if (numVertices < 0) {
    printf("Number of vertices must be positive. Read: %d. Giving up.\n",
           numVertices);
    return NULL;
  }

  Graph* graph = newGraph(numVertices);
  if (graph == NULL) {
    printf("Could not create a new graph. Giving up.\n");
    return NULL;
  }

  while (fgets(line, MAX_LIMIT, f)) {  // read next line
    if (!updateVertex(graph, line)) {      // update vertex info from line
      printf("Could not get vertex info from a line. Giving up.\n");
      deleteGraph(graph);
      return NULL;
    }
  }
  return graph;
}

/* Updates / poulates the corresponding vertex in 'graph' using information
 * from the line 'line' in an input file. Returns true iff update was
 * successful.
 */
bool updateVertex(Graph* graph, char* line) {
  if (graph == NULL) return false;

  // parse vertex ID
  char* token = strtok(line, " ");
  int id = readVertexID(token, graph->numVertices);
  if (id == -1) return false;

  // parse adjacency list
  AdjList* head = NULL;
  int toVertex = 0;
  int weight = 0;
  token = strtok(NULL, " ");
  while (token) {
    toVertex = readVertexID(token, graph->numVertices);
    if (toVertex == -1) return false;

    token = strtok(NULL, " ");
    weight = readWeight(token);
    if (weight == -1) return false;

    head = addEdge(head, id, toVertex, weight);
    if (head == NULL) return false;
    graph->numEdges++;

    token = strtok(NULL, " ");
  }

  graph->vertices[id].id = id;
  graph->vertices[id].value = NULL;  // nothing in our file
  graph->vertices[id].adjList = head;

  return true;
}

/* Prepends a new Edge from vertex 'fromVertex' to vertex 'toVertex' with
 * weight 'weight', to the adjacency list 'head' and returns the result.
 */
AdjList* addEdge(AdjList* head, int fromVertex, int toVertex, int weight) {
  Edge* edge = newEdge(fromVertex, toVertex, weight);
  if (edge == NULL) {
    printf("Could not allocate a new Edge. Giving up.\n");
    return NULL;
  }
  AdjList* adjList = newAdjList(edge, head);
  if (adjList == NULL) {
    printf("Could not allocate a new AdjList. Giving up.\n");
    return NULL;
  }
  return adjList;
}

/* Parses and validates a vertex ID for a graph with 'numVertices' vertices,
 * from 'token'. Returns the ID if validation is successful, and -1 if it is
 * not.
 */
int readVertexID(char* token, int numVertices) {
  if (!token) {
    printf("Could not read vertex ID from input file. Giving up.\n");
    return -1;
  }
  int id = atoi(token);
  if (id < 0 || id >= numVertices) {
    printf("Invalid vertex ID: %d. Giving up.\n", id);
    return -1;
  }
  return id;
}

/* Parses and validates an edge weight from 'token'. Returns the weight if
 * validation is successful, and -1 if it not.
 */
int readWeight(char* token) {
  if (!token) {
    printf("Could not read edge weight from input file. Giving up.\n");
    return -1;
  }
  int weight = atoi(token);
  if (weight < 0) {
    printf("Invalid edge weight: %d. Giving up.\n", weight);
    return -1;
  }
  return weight;
}

/* Prints the spanning tree 'tree' with 'numTreeEdges' edges. Returns the
 * total weight of 'tree'.
 */
int printTree(Edge* tree, int numTreeEdges) {
  if (tree == NULL) return -1;

  int totalWeight = 0;
  for (int i = 0; i < numTreeEdges; i++) {
    printEdge(&tree[i]);
    printf("\n");
    totalWeight += tree[i].weight;
  }
  return totalWeight;
}

/* Prints all adjacency lists in the array 'paths' of 'numVertices' lists.
 */
void printPaths(AdjList* paths, int numVertices) {
  if (paths == NULL) return;

  for (int i = 0; i < numVertices; i++) {
    printf("From vertex %d: ", i);
    printAdjList(&paths[i]);
    printf("\n");
  }
}

/* Frees memory for all adjacency lists in the array 'paths' of 'numVertices'
 * lists.
 */
void freePaths(AdjList* paths, int numVertices) {
  if (paths == NULL) return;

  AdjList* curr = NULL;
  AdjList* next = NULL;
  for (int i = 0; i < numVertices; i++) {
    if (paths[i].edge) free(paths[i].edge);
    curr = paths[i].next;
    while (curr != NULL) {
      next = curr->next;
      if (curr->edge) free(curr->edge);
      free(curr);
      curr = next;
    }
  }
}
