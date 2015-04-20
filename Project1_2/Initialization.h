#ifndef INITIALIZATION_H
#define INITIALIZATION_H

#include "given.h"
#include "Stack.h"
#include "PathSet.h"

int *primaryInputs;
int *primaryOutputs;

void initDelay(GATE *Node, int Npi, int Npo, int Tgat);
void initInputOuputArrays(GATE *Node, int Npi, int Npo, int Tgat);

void freeInputOutputArrays();

void insertPathCount(PATH_COUNT **Cur, int delay, int count);
void buildNLongestPath(GATE *Node, int n, int NodeIndex, int PathSetIndex, int currPathDelay, int *PathIndex, int numPaths);

void freePathSet(int Npo);

#endif // INITIALIZATION_H
