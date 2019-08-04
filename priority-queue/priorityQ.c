#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compare.h"
#include "priorityQ.h"

TypeQ create_element(int prio, char *val)
{
	TypeQ elem;

	elem.prio = prio;
	elem.val = (char *)calloc(strlen(val)+1, sizeof(char));

	if (elem.val == NULL)
		exit(ENOMEM);

	strcpy(elem.val, val);

	return elem;
}

void reorder(TypeQ **pq, int pos, int size)
{
	int left = 2*pos+1, right = 2*pos+2, parent = (pos-1)/2;
	int posmax = pos;

	if ((parent >= 0) &&
	(compare((*pq + pos)->prio, (*pq + parent)->prio)) > 0) {
		SWAP(*(*pq + parent), *(*pq + pos));
		reorder(pq, parent, size);
		return;
	}

	if ((left < size) &&
	(compare((*pq + left)->prio, (*pq + pos)->prio)) > 0)
		posmax = left;

	if ((right < size) &&
	(compare((*pq + right)->prio, (*pq + posmax)->prio)) > 0)
		posmax = right;

	if (pos != posmax) {
		SWAP(*(*pq + pos), *(*pq + posmax));
		reorder(pq, posmax, size);
	}
}

void pq_insert(TypeQ **pq, int prio, char *val, int last)
{
	if ((*pq + last)->val != NULL)
		free((*pq + last)->val);

	*(*pq + last) = create_element(prio, val);
	reorder(pq, last, last);
}

void pq_pop(TypeQ **pq, int last)
{
	SWAP((**pq), (*(*pq + last)));
	reorder(pq, 0, last);
}

char *pq_top(TypeQ *pq, int last)
{
	if (last > 0)
		return (*pq).val;
	else
		return "";
}

int main(int argc, char *argv[])
{

	int file = 1, params, prio, last = 0;
	char command[BUFSIZE], val[BUFSIZE], leftovers[BUFSIZE];
	char *line = (char *)calloc(BUFSIZE, sizeof(char));
	size_t i, size = QSIZE;
	FILE *f = NULL;
	TypeQ *tmp, *pQ = (TypeQ *)calloc(size, sizeof(TypeQ));

	if (pQ == NULL)
		exit(ENOMEM);

	if (argc == 1) {
		/* read from stdin: */
		f = stdin;
	} else {

another_file:

		/* read from files: */
		while ((f == NULL) && (file < argc))
			f = freopen(argv[file++], "r+", stdin);

		/* no more valid files */
		if ((f == NULL) && (file == argc)) {

			/* free memory */
			for (i = 0 ; i < size ; i++)
				if (pQ[i].val != NULL)
					free(pQ[i].val);

			free(pQ);
			free(line);

			return 0;
		}
	}

	/* fread from stream f until EOF */
	while (fgets(line, BUFSIZE, f) != NULL) {

		params = sscanf(line, "%s %s %d %s", command, val, &prio, leftovers);

		if (strcmp(command, "top") == 0) {

			/* invalid command - wrong parameters */
			if (params != 1)
				continue;

			printf("%s\n", pq_top(pQ, last));

		} else if (strcmp(command, "pop") == 0) {

			/* invalid command - wrong parameters */
			if (params != 1)
				continue;

			/* if queue is not empy, pop element */
			if (last > 0)
				pq_pop(&pQ, --last);

		} else if (strcmp(command, "insert") == 0) {

			/* invalid command - wrong parameters */
			if (params != 3)
				continue;

			/* double queue size, if needed */
			if (last == size) {
				size *= 2;
				tmp = (TypeQ *)realloc(pQ, sizeof(TypeQ)*size);

				if (tmp == NULL)
					exit(ENOMEM);
				else
					pQ = tmp;

				memset(pQ + last, 0, sizeof(TypeQ)*(size/2));
			}

			/* insert element */
			pq_insert(&pQ, prio, val, last++);

		}

	}

	/* close stream */
	fclose(f);
	f = NULL;
	goto another_file;

	return 0;

}
