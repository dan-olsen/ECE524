#include <stdio.h>
#include <stdlib.h>

#include "stack.h"       

void StackInit(stackT *stackP)
{
	int *newContents;

	/* Allocate a new array to hold the contents. */

	newContents = (int *)malloc(sizeof(int) * INIT_SIZE);

	if (newContents == NULL) {
		fprintf(stderr, "Insufficient memory to initialize stack.\n");
		exit(1);  /* Exit, returning error code. */
	}

	stackP->contents = newContents;
	stackP->size = INIT_SIZE;
	stackP->top = -1;  /* I.e., empty */
}

void StackDestroy(stackT *stackP)
{
	/* Get rid of array. */
	free(stackP->contents);

	stackP->contents = NULL;
	stackP->size = 0;
	stackP->top = -1;  /* I.e., empty */
}

void StackPush(stackT *stackP, int element)
{
	if (StackIsFull(stackP)) {
		stackP->contents = (int *)realloc(stackP->contents, sizeof(int) * INCREASE_SIZE);
	}

	/* Put information in array; update top. */

	stackP->contents[++stackP->top] = element;
}

int StackPop(stackT *stackP)
{
	if (StackIsEmpty(stackP)) {
		fprintf(stderr, "Can't pop element from stack: stack is empty.\n");
		exit(1);  /* Exit, returning error code. */
	}

	return stackP->contents[stackP->top--];
}

int StackIsEmpty(stackT *stackP)
{
	return stackP->top < 0;
}

int StackIsFull(stackT *stackP)
{
	return stackP->top >= stackP->size - 1;
}
