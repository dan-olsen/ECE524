#ifndef _STACK_H
#define _STACK_H

#define INIT_SIZE 32
#define INCREASE_SIZE 8

#include "given.h"

typedef struct {
    int *contents;
    int contentSize;
    int capacity;
    int top;
} stackT;

void StackInit(stackT *stackP);
void StackDestroy(stackT *stackP);
void StackPush(stackT *stackP, int element);
void StackPop(stackT *stackP);
int StackIsEmpty(stackT *stackP);
int StackIsFull(stackT *stackP);
void StackCopyToList(stackT *stackP, LIST **list);

#endif  /* not defined _STACK_H */
