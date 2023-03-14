/*
 * Our graph implementation.
 *
 * Author: A. Tafliovich.
 */

#include "graph.h"

/*********************************************************************
 ** Required functions
 *********************************************************************/


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
