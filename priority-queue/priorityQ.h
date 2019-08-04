#ifndef __TEMA1_H_
#define __TEMA1_H_

#define SWAP(x, y) do { TypeQ swap = x; x = y; y = swap; } while (0)
#define ENOMEM		12
#define BUFSIZE		2000
#define QSIZE		20

/**
 * Structure for the type of an element
 * in a priority queue (priority and value)
 */

typedef struct {
	int prio;
	char *val;
} TypeQ;

/**
 * Function to create an element for inserting
 * in a priority queue
 * @param prio - element's priority
 * @param *val - element's value
 */
TypeQ create_element(int prio, char *val);


/**
 * Function that places the element at position 'pos'
 * in its right place according to the heap structure
 * @param **pq - the address of priority queue
 * @param pos - the position of the element to be reordered
 * @param size - the size of priority queue
 */
void reorder(TypeQ **pq, int pos, int size);


/**
 * Function to insert one element in priority queue
 * @param **pq - the address of priority queue
 * @param prio - the priority of the element
 * @param *val - the value of the element
 * @param last - index of the last element in the queue
 */
void pq_insert(TypeQ **pq, int prio, char *val, int last);


/**
 * Function to pop one element from priority queue
 * @param **pq - the address of priority queue
 * @param last - index of the last element in the queue
 */
void pq_pop(TypeQ **pq, int last);


/**
 * Function for accessing the head of the priority queue
 * @param **pq - the address of priority queue
 * @param last - index of the last element in the queue
 * @return value - head's value if the queue is not empty,
 *			empty string ("") otherwise
 */
char *pq_top(TypeQ *pq, int last);

#endif
