#include <stdio.h>
#include <stdlib.h>

#include "stack.h"

void StackInit(stackT *stackP)
{
    if(stackP->contents == NULL){
        stackP->contents = (int *)malloc(sizeof(int) * INIT_SIZE);

        if (stackP->contents == NULL) {
            fprintf(stderr, "Insufficient memory to initialize stack.\n");
            exit(1);  /* Exit, returning error code. */
        }

    } else {
        fprintf(stderr, "Stack Init called and contents not NULL.\n");
        exit(1);  /* Exit, returning error code. */

    }

    stackP->capacity = INIT_SIZE;
    stackP->contentSize = 0;
    stackP->top = -1;  /* I.e., empty */
}

void StackDestroy(stackT *stackP)
{
    /* Get rid of array. */
    free(stackP->contents);

    stackP->contents = NULL;
    stackP->capacity = 0;
    stackP->contentSize = 0;
    stackP->top = -1;  /* I.e., empty */
}

void StackPush(stackT *stackP, int element)
{
    if (StackIsFull(stackP)) {
        stackP->contents = (int *)realloc(stackP->contents, sizeof(int) * INCREASE_SIZE);

        if (stackP->contents == NULL) {
            fprintf(stderr, "Insufficient memory to initialize stack.\n");
            exit(1);  /* Exit, returning error code. */
        }

        stackP->capacity += INCREASE_SIZE;
    }

    /* Put information in array; update top. */
    stackP->contentSize++;
    stackP->top++;
    stackP->contents[stackP->top] = element;
}

void StackPop(stackT *stackP)
{
    if (StackIsEmpty(stackP)) {
        fprintf(stderr, "Can't pop element from stack: stack is empty.\n");
        exit(1);  /* Exit, returning error code. */
    }
    stackP->contentSize--;
    stackP->top--;

    stackP->contents[stackP->top];
}

int StackIsEmpty(stackT *stackP)
{
    return stackP->top < 0;
}

int StackIsFull(stackT *stackP)
{
    return stackP->top >= stackP->capacity - 1;
}

void StackCopyToList(stackT *stackP, LIST **list)
{
    int i;

    for(i = stackP->top; i > -1; i--)
    {
        InsertEle(list, stackP->contents[i]);
    }

}

void PrintStack(stackT *stackP)
{
    int i;

    for(i = stackP->top; i > -1; i--)
    {
    	printf("%d ", stackP->contents[i]);
    }

    printf("\n");
}
