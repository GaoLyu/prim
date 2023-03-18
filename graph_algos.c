/*
 * Graph algorithms.
 *
 * Author: A. Tafliovich.
 */

#include <limits.h>

#include "graph.h"
#include "minheap.h"

#define NOTHING -1

typedef struct records {
  int numVertices;    // total number of vertices in the graph
                      // vertex IDs are 0, 1, ..., numVertices-1
  MinHeap* heap;      // priority queue
  bool* finished;     // finished[id] is true iff vertex id is finished
                      //   i.e. no longer in the PQ
  int* predecessors;  // predecessors[id] is the predecessor of vertex id
  Edge* tree;         // keeps edges for the resulting tree
  int numTreeEdges;   // current number of edges in mst
} Records;

/*************************************************************************
 ** Suggested helper functions, to help with your program design
 *************************************************************************/

/* Creates, populates, and returns a MinHeap to be used by Prim's and
 * Dijkstra's algorightms on Graph 'graph' starting from vertex with ID
 * 'startVertex'.
 * Precondition: 'startVertex' is valid in 'graph'
 */
MinHeap* initHeap(Graph* graph, int startVertex){
  int numVertices=graph->numVertices;
  MinHeap* heap=newHeap(numVertices);
  insert(heap,0,startVertex);
  for(int i=0; i<numVertices; i++){
    if(graph->vertices[i].id!=startVertex){
      insert(heap,INT_MAX,graph->vertices[i].id);
    }
  }
  return heap;
}

/* Creates, populates, and returns all records needed to run Prim's and
 * Dijkstra's algorithms on Graph 'graph' starting from vertex with ID
 * 'startVertex'.
 * Precondition: 'startVertex' is valid in 'graph'
 */
//????????????????????calloc when to free

Records* initRecords(Graph* graph, int startVertex, int alg){
  int numVertices=graph->numVertices;
  Records* record=malloc(sizeof(Records));
  record->numVertices=numVertices;
  record->numTreeEdges=0;
  record->heap=initHeap(graph,startVertex);
  record->finished=malloc(sizeof(bool)*numVertices);
  for(int i=0;i<numVertices;i++){
    record->finished[i]=false;
    //is startvertex finished in the beginning
  }
  record->predecessors=malloc(sizeof(int)*numVertices);
  for(int i=0;i<numVertices;i++){
    record->predecessors[i]=NOTHING;
    //predecessor of startvertex
  }
  if(alg==0){//prim get MST
    record->tree=malloc(sizeof(Edge)*(numVertices-1));
  }
  if(alg==1){//dijkstra
    record->tree=malloc(sizeof(Edge)*(numVertices));
  }
  return record;

  //heap contains all vertex, numtreeed
}

/* Returns true iff 'heap' is NULL or is empty. */
bool isEmpty(MinHeap* heap){
  return (heap==NULL || heap->size==0);
}

/* Add a new edge to records at index ind. */
void addTreeEdge(Records* records, int ind, int fromVertex, int toVertex,
                 int weight){
  records->tree[ind].fromVertex=fromVertex;
  records->tree[ind].toVertex=toVertex;
  records->tree[ind].weight=weight;
  if(0<=fromVertex && fromVertex<records->numVertices){
    records->finished[fromVertex]=true;
  }
  if(0<=toVertex && toVertex<records->numVertices){
    records->finished[toVertex]=true;
  }
  records->numTreeEdges++;
}

/* Creates and returns a path from 'vertex' to 'startVertex' from edges
 * in the distance tree 'distTree'.
 */
//??????????????????how many edges
AdjList* makePath(Edge* distTree, int vertex, int startVertex){
  if(vertex==startVertex){
    return NULL;
  }
  int nextVertexId;
  if(distTree[vertex].fromVertex==vertex){
    nextVertexId=distTree[vertex].toVertex;
  }
  else{
    nextVertexId=distTree[vertex].fromVertex;
  }
  AdjList* result=malloc(sizeof(AdjList));
  result->edge->fromVertex=distTree[vertex].fromVertex;
  result->edge->toVertex=distTree[vertex].toVertex;
  result->edge->weight=distTree[vertex].weight-distTree[nextVertexId].weight;
  result->next=makePath(distTree,nextVertexId,startVertex);
  //????????????????????can we directly give the address of distTree[] or copy the whole things
  
  return result;
}

/*************************************************************************
 ** Required functions.
 *************************************************************************/
/* Runs Prim's algorithm on Graph 'graph' starting from vertex with ID
 * 'startVertex', and return the resulting MST: an array of Edges.
 * Returns NULL if 'startVertex' is not valid in 'graph'.
 * Precondition: 'graph' is connected.
 */
//???????????predesessor of startvertex
Edge* primGetMST(Graph* graph, int startVertex){
  int numVertices=graph->numVertices;
  if(startVertex<0 || startVertex>=numVertices){
    return NULL;
  }
  Vertex currentVertex;
  AdjList* adjList;
  int adjId;
  Records records=initRecords(graph,startVertex,0);
  while(!(isEmpty(records->heap))){
    HeapNode currentNode=extractMin(records->heap);
    int currentId=currentNode.id;
    int currentWeight=currentNode.priority;
    Vertex currentVertex=graph->vertices[currentId];
    if(currentId!=startVertex){
      addTreeEdge(records,records->numTreeEdges,records->predecessors[currentId],currentId,currentWeight);
    }
    adjList=currentVertex.adjList;
    while(adjList!=NULL){
      if(adjList->edge->fromVertex==currentId){
        adjId=adjList->edge->toVertex;
      }
      else{
        adjId=adjList->edge->fromVertex;
      }
      if(finished[adjId]==false && adjList->edge->weight<getPriority(records->heap,adjId)){
        decreasePriority(records->heap,adjId,adjList->edge->weight);
        records->predecessors[adjId]=currentId;
      }
      adjList=adjList->next;
    }
  }
  deleteHeap(records->heap);
  free(records->finished);
  free(records->predecessors);
  Edge* result=records->tree;
  free(records);
  return result;
}

/* Runs Dijkstra's algorithm on Graph 'graph' starting from vertex with ID
 * 'startVertex', and return the resulting distance tree: an array of edges.
 * Returns NULL is 'startVertex' is not valid in 'graph'.
 * Precondition: 'graph' is connected.
 */
Edge* getShortestPaths(Graph* graph, int startVertex){
  int numVertices=graph->numVertices;
  if(startVertex<0 || startVertex>=numVertices){
    return NULL;
  }
  Vertex currentVertex;
  AdjList* adjList;
  int adjId;
  int totalWeight;
  Records records=initRecords(graph,startVertex,1);
  while(!(isEmpty(records->heap))){
    HeapNode currentNode=extractMin(records->heap);
    int currentId=currentNode.id;
    int currentWeight=currentNode.priority;
    Vertex currentVertex=graph->vertices[currentId];
    if(currentId==startVertex){
      addTreeEdge(records,currentId,currentId,currentId,0);
      records->finished[currentId]=true;
    }
    else{
      addTreeEdge(records,currentId,currentId,records->predecessors[currentId],currentWeight);
    }
    adjList=currentVertex.adjList;
    while(adjList!=NULL){
      if(adjList->edge->fromVertex==currentId){
        adjId=adjList->edge->toVertex;
      }
      else{
        adjId=adjList->edge->fromVertex;
      }
      totalWeight=adjList->edge->weight+currentWeight;
      if(totalWeight<getPriority(records->heap,adjId)){
        decreasePriority(records->heap,adjId,totalWeight);
        records->predecessors[adjId]=currentId;
      }
      adjList=adjList->next;
    }
  }
  deleteHeap(records->heap);
  free(records->finished);
  free(records->predecessors);
  Edge* result=records->tree;
  free(records);
  return result;
}

/* Creates and returns an array 'paths' of shortest paths from every vertex
 * in the graph to vertex 'startVertex', based on the information in the
 * distance tree 'distTree' produced by Dijkstra's algorithm on a graph with
 * 'numVertices' vertices and with the start vertex 'startVertex'.  paths[id]
 * is the array of Edges of the form
 *   [(id -- id_1, w_0), (id_1 -- id_2, w_1), ..., (id_n -- start, w_n)]
 *   where w_0 + w_1 + ... + w_n = distance(id)
 * Returns NULL if 'startVertex' is not valid in 'distTree'.
 */
AdjList* getPaths(Edge* distTree, int numVertices, int startVertex){
  if(startVertex<0 || startVertex>=numVertices){
    return NULL;
  }
  AdjList* result=malloc(sizeof(AdjList)*numVertices);
  for(int i=0;i<numVertices;i++){
    result[i]=makePath(distTree,u,startVertex);
  }
  return result;
}

/*************************************************************************
 ** Provided helper functions -- part of starter code to help you debug!
 *************************************************************************/
void printRecords(Records* records) {
  if (records == NULL) return;

  int numVertices = records->numVertices;
  printf("Reporting on algorithm's records on %d vertices...\n", numVertices);

  printf("The PQ is:\n");
  printHeap(records->heap);

  printf("The finished array is:\n");
  for (int i = 0; i < numVertices; i++)
    printf("\t%d: %d\n", i, records->finished[i]);

  printf("The predecessors array is:\n");
  for (int i = 0; i < numVertices; i++)
    printf("\t%d: %d\n", i, records->predecessors[i]);

  printf("The TREE edges are:\n");
  for (int i = 0; i < records->numTreeEdges; i++) printEdge(&records->tree[i]);

  printf("... done.\n");
}
