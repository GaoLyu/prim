/*
 * Our graph implementation.
 *
 * Author: A. Tafliovich.
 */

#include "graph.h"

/*********************************************************************
 ** Required functions
 *********************************************************************/
/* Returns a newly created Edge from vertex with ID 'fromVertex' to vertex
 * with ID 'toVertex', with weight 'weight'.
 */
Edge* newEdge(int fromVertex, int toVertex, int weight) {
  Edge* new = malloc(sizeof(Edge));
  new->fromVertex = fromVertex;
  new->toVertex = toVertex;
  new->weight = weight;
  return new;
}

/* Returns a newly created AdjList containing 'edge' and pointing to the next
 * AdjList node 'next'.
 */
AdjList* newAdjList(Edge* edge, AdjList* next) {
  AdjList* new = calloc(1, sizeof(AdjList));
  new->edge = edge;
  new->next = next;
  return new;
}

/* Returns a newly created Graph with space for 'numVertices' vertices.
 * Precondition: numVertices >= 0
 */
Graph* newGraph(int numVertices) {
  Graph* new = malloc(sizeof(Graph));
  new->numVertices = numVertices;
  new->numEdges = 0;
  new->vertices = malloc(sizeof(Vertex) * numVertices);
  for (int i = 0; i < numVertices; i++) {
    new->vertices[i].id = i;
    new->vertices[i].adjList = NULL;
  }
  return new;
}

/* Frees memory allocated for AdjList starting at 'head'.
 */
void deleteAdjList(AdjList* head) {
  AdjList* next;
  while (head != NULL) {
    next = head->next;
    free(head->edge);
    free(head);
    head = next;
  }
}

/* Frees memory allocated for 'vertex''s adjacency list.
 */
void deleteVertex(Vertex* vertex) { deleteAdjList(vertex->adjList); }

/* Frees memory allocated for 'graph'.
 */
void deleteGraph(Graph* graph) {
  for (int i = 0; i < graph->numVertices; i++) {
    deleteVertex(&(graph->vertices[i]));
  }
  free(graph->vertices);
  free(graph);
}

/*********************************************************************
 ** Helper function provided in the starter code
 *********************************************************************/

void printEdge(Edge* edge) {
  if (edge == NULL) return;
  printf("(%d -- %d, %d)", edge->fromVertex, edge->toVertex, edge->weight);
}

void printAdjList(AdjList* head) {
  while (head != NULL) {
    printEdge(head->edge);
    printf("  ");
    head = head->next;
  }
}

void printVertex(Vertex* vertex) {
  if (vertex == NULL) return;
  printf("%d: ", vertex->id);
  printAdjList(vertex->adjList);
}

void printGraph(Graph* graph) {
  if (graph == NULL) return;

  printf("Number of vertices: %d. Number of edges: %d.\n\n", graph->numVertices,
         graph->numEdges);

  for (int i = 0; i < graph->numVertices; i++) {
    printVertex(&graph->vertices[i]);
    printf("\n");
  }
  printf("\n");
}