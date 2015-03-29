#ifndef _STACK_H
#define _STACK_H

#define INIT_SIZE 32
#define INCREASE_SIZE 8

typedef struct {
  int *contents;
  int size;
  int top;
} stackT;

void StackInit(stackT *stackP);
void StackDestroy(stackT *stackP);
void StackPush(stackT *stackP, int element);
int StackPop(stackT *stackP);
int StackIsEmpty(stackT *stackP);
int StackIsFull(stackT *stackP);

#endif  /* not defined _STACK_H */
