/*
 * Our min-heap implementation.
 *
 * Author (starter code): A. Tafliovich.
 */

#include "minheap.h"

#define ROOT_INDEX 1
#define NOTHING -1

/*************************************************************************
 ** Suggested helper functions to help with your program design
 *************************************************************************/

/* Returns True if 'maybeIdx' is a valid index in minheap 'heap', and 'heap'
 * stores an element at that index. Returns False otherwise.
 */
bool isValidIndex(MinHeap* heap, int maybeIdx) {
  if (ROOT_INDEX <= maybeIdx && maybeIdx <= heap->size) {
    return true;
  }
  return false;
}

/* Returns the index of the left child of a node at index 'nodeIndex' in
 * minheap 'heap', if such exists.  Returns NOTHING if there is no such left
 * child.
 */
int leftIdx(MinHeap* heap, int nodeIndex) {
  int left = 2 * nodeIndex;
  if (isValidIndex(heap, left)) {
    return left;
  } else {
    return NOTHING;
  }
}

/* Returns the index of the right child of a node at index 'nodeIndex' in
 * minheap 'heap', if such exists.  Returns NOTHING if there is no such right
 * child.
 */
int rightIdx(MinHeap* heap, int nodeIndex) {
  int right = 2 * nodeIndex + 1;
  if (isValidIndex(heap, right)) {
    return right;
  } else {
    return NOTHING;
  }
}

/* Returns the index of the parent of a node at index 'nodeIndex' in minheap
 * 'heap', if such exists.  Returns NOTHING if there is no such parent.
 */
int parentIdx(MinHeap* heap, int nodeIndex) {
  int parent = nodeIndex / 2;
  if (isValidIndex(heap, parent)) {
    return parent;
  } else {
    return NOTHING;
  }
}

/* Swaps contents of heap->arr[index1] and heap->arr[index2] if both 'index1'
 * and 'index2' are valid indices for minheap 'heap'. Has no effect
 * otherwise.
 */
void swap(MinHeap* heap, int index1, int index2) {
  if (isValidIndex(heap, index1) && isValidIndex(heap, index2)) {
    HeapNode copyNode = heap->arr[index1];
    int id1 = heap->arr[index1].id;
    int id2 = heap->arr[index2].id;
    int copyIndex = heap->indexMap[id1];
    heap->arr[index1] = heap->arr[index2];
    heap->arr[index2] = copyNode;
    heap->indexMap[id1] = heap->indexMap[id2];
    heap->indexMap[id2] = copyIndex;
  }
}

/* Bubbles up the element newly inserted into minheap 'heap' at index
 * 'nodeIndex', if 'nodeIndex' is a valid index for heap. Has no effect
 * otherwise.
 */
void bubbleUp(MinHeap* heap, int nodeIndex) {
  int parentIdx;
  while (nodeIndex > ROOT_INDEX && nodeIndex <= heap->size) {
    parentIdx = nodeIndex / 2;
    if (heap->arr[parentIdx].priority > heap->arr[nodeIndex].priority) {
      swap(heap, parentIdx, nodeIndex);
      nodeIndex = parentIdx;
    } else {
      break;
    }
  }
}

/* Bubbles down the element newly inserted into minheap 'heap' at the root,
 * if it exists. Has no effect otherwise.
 */
void bubbleDown(MinHeap* heap) {
  if (heap->size <= ROOT_INDEX) {
    return;
  }
  int parent, left, right;
  int minIdx;
  parent = ROOT_INDEX;
  while (parent <= heap->size) {
    left = leftIdx(heap, parent);
    right = rightIdx(heap, parent);
    minIdx = parent;
    if (left != NOTHING &&
        heap->arr[left].priority < heap->arr[parent].priority) {
      minIdx = left;
    }
    if (right != NOTHING &&
        heap->arr[right].priority < heap->arr[minIdx].priority) {
      minIdx = right;
    }
    if (minIdx != parent) {
      swap(heap, parent, minIdx);
      parent = minIdx;
    } else {
      break;
    }
  }
}

/* Returns node at index 'nodeIndex' in minheap 'heap'.
 * Precondition: 'nodeIndex' is a valid index in 'heap'
 *               'heap' is non-empty
 */
HeapNode nodeAt(MinHeap* heap, int nodeIndex) { return heap->arr[nodeIndex]; }

/* Returns priority of node at index 'nodeIndex' in minheap 'heap'.
 * Precondition: 'nodeIndex' is a valid index in 'heap'
 *               'heap' is non-empty
 */
int priorityAt(MinHeap* heap, int nodeIndex) {
  if (!(isValidIndex(heap, nodeIndex))) {
    return NOTHING;
  }
  return nodeAt(heap, nodeIndex).priority;
}

/* Returns ID of node at index 'nodeIndex' in minheap 'heap'.
 * Precondition: 'nodeIndex' is a valid index in 'heap'
 *               'heap' is non-empty
 */
int idAt(MinHeap* heap, int nodeIndex) {
  if (!(isValidIndex(heap, nodeIndex))) {
    return NOTHING;
  }
  return nodeAt(heap, nodeIndex).id;
}

/* Returns index of node with ID 'id' in minheap 'heap'.
 * Precondition: 'id' is a valid ID in 'heap'
 *               'heap' is non-empty
 */
int indexOf(MinHeap* heap, int id) { return heap->indexMap[id]; }

/*********************************************************************
 * Required functions
 ********************************************************************/
/* Returns the node with minimum priority in minheap 'heap'.
 * Precondition: heap is non-empty
 */
HeapNode getMin(MinHeap* heap) { return heap->arr[ROOT_INDEX]; }

/* Removes and returns the node with minimum priority in minheap 'heap'.
 * Precondition: heap is non-empty
 */
HeapNode extractMin(MinHeap* heap) {
  HeapNode min = getMin(heap);
  int size = heap->size;
  int idlast = heap->arr[size].id;
  int idmin = heap->arr[ROOT_INDEX].id;
  heap->arr[ROOT_INDEX] = heap->arr[size];
  heap->indexMap[idlast] = heap->indexMap[idmin];
  heap->indexMap[idmin] = NOTHING;
  heap->size--;
  bubbleDown(heap);
  return min;
}

/* Inserts a new node with priority 'priority' and ID 'id' into minheap 'heap'.
 * Precondition: 'id' is unique within this minheap
 *               0 <= 'id' < heap->capacity
 *               heap->size < heap->capacity
 */
void insert(MinHeap* heap, int priority, int id) {
  heap->size++;
  int size = heap->size;
  heap->arr[size].priority = priority;
  heap->arr[size].id = id;
  heap->indexMap[id] = size;
  bubbleUp(heap, size);
}

/* Returns priority of the node with ID 'id' in 'heap'.
 * Precondition: 'id' is a valid node ID in 'heap'.
 */
int getPriority(MinHeap* heap, int id) {
  return priorityAt(heap, indexOf(heap, id));
}

/* Sets priority of node with ID 'id' in minheap 'heap' to 'newPriority', if
 * such a node exists in 'heap' and its priority is larger than
 * 'newPriority', and returns True. Has no effect and returns False, otherwise.
 * Note: this function bubbles up the node until the heap property is restored.
 */
bool decreasePriority(MinHeap* heap, int id, int newPriority) {
  if (id >= heap->capacity || id < 0) {
    return false;
  }
  int index = heap->indexMap[id];
  if (!(isValidIndex(heap, index))) {
    return false;
  }
  if (heap->arr[index].priority <= newPriority) {
    return false;
  } else {
    heap->arr[index].priority = newPriority;
    bubbleUp(heap, index);
    return true;
  }
}

/* Returns a newly created empty minheap with initial capacity 'capacity'.
 * Precondition: capacity >= 0
 */
MinHeap* newHeap(int capacity) {
  MinHeap* new = malloc(sizeof(MinHeap));
  new->size = 0;
  new->capacity = capacity;
  new->arr = malloc((capacity + 1) * sizeof(HeapNode));
  new->indexMap = malloc((capacity) * sizeof(int));
  for (int i = 0; i < capacity; i++) {
    new->indexMap[i] = NOTHING;
  }
  return new;
}

/* Frees all memory allocated for minheap 'heap'.
 */
void deleteHeap(MinHeap* heap) {
  free(heap->arr);
  free(heap->indexMap);
  free(heap);
}

/*********************************************************************
 ** Helper functions provided in the starter code
 *********************************************************************/
void printHeap(MinHeap* heap) {
  printf("MinHip with size: %d\n\tcapacity: %d\n\n", heap->size,
         heap->capacity);
  printf("index: priority [ID]\t ID: index\n");
  for (int i = 0; i < heap->capacity; i++)
    printf("%d: %d [%d]\t\t%d: %d\n", i, priorityAt(heap, i), idAt(heap, i), i,
           indexOf(heap, i));
  printf("%d: %d [%d]\t\t\n", heap->capacity, priorityAt(heap, heap->capacity),
         idAt(heap, heap->capacity));
  printf("\n\n");
}
