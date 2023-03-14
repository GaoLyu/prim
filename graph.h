/*
 * Header file for our graph implementation.
 *
 * You will NOT be submitting this file. Your code will be tested with
 * our own version of this file, so make sure you do not modify it!
 *
 * Author: A. Tafliovich.
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef __Graph_header
#define __Graph_header

typedef struct edge {
  int fromVertex;  // id of the "from" vertex
  int toVertex;    // id of the "to" vertex
  int weight;      // weight of this edge; weight >= 0
} Edge;

typedef struct adj_list {  // a linked list of Edges
  Edge* edge;              // first Edge in this adjacency list
  struct adj_list* next;   // the rest of this adjacency list
} AdjList;

typedef struct vertex {
  int id;            // unique in the graph; 0 <= id < numVertices
  void* value;       // value associated with this vertex
  AdjList* adjList;  // adjacency list of this vertex
} Vertex;

typedef struct graph {
  int numVertices;   // total number of vertices
  int numEdges;      // total number of edges
  Vertex* vertices;  // array of numVertices Vertex's; vertices[v.id] = v
} Graph;

/***** Displaying graph elements ********************************************/

/* Prints Graph 'graph', including total number of vertices, total number of
 * edges, and all vertices with their adjacency lists.
 */
void printGraph(Graph* graph);

/* Prints 'edge', including from vertex, to vertex, and weight. */
void printEdge(Edge* edge);

/* Prints all Edges in the adjacency list starting from 'head'.*/
void printAdjList(AdjList* head);

/* Prints 'vertex', including the ID and the complete adjacency list. */
void printVertex(Vertex* vertex);

/***** Memory management ***************************************************/

/* Returns a newly created Edge from vertex with ID 'fromVertex' to vertex
 * with ID 'toVertex', with weight 'weight'.
 */
Edge* newEdge(int fromVertex, int toVertex, int weight);

/* Returns a newly created AdjList containing 'edge' and pointing to the next
 * AdjList node 'next'.
 */
AdjList* newAdjList(Edge* edge, AdjList* next);

/* Returns a newly created Graph with space for 'numVertices' vertices.
 * Precondition: numVertices >= 0
 */
Graph* newGraph(int numVertices);

/* Frees memory allocated for AdjList starting at 'head'.
 */
void deleteAdjList(AdjList* head);

/* Frees memory allocated for 'vertex''s adjacency list.
 */
void deleteVertex(Vertex* vertex);

/* Frees memory allocated for 'graph'.
 */
void deleteGraph(Graph* graph);

#endif
