#ifndef INITIALIZATION_H
#define INITIALIZATION_H

#include "given.h"
#include "Stack.h"

typedef struct PATH_type
{
    LIST *Path;
    int suspect;

} PATH;

typedef struct PATH_SET_type {
    int Id;
    int numLongestPath;
    int numSecondLongestPath;
    PATH *longestPath;
    PATH *secondLongestPath;

} PATH_SET;

//Tot no of PIs,Pos,Maxid,Tot no of patterns in.vec,.fau
int Npi, Npo, Tgat;
PATH_SET *pathSet;
stackT pathStack;
int *primaryInputs;
int *primaryOutputs;

void InitDelay(GATE *Node);
void InitInputOuputArrays(GATE *Node);

void FreeInputOutputArrays();

void InsertPathCount(PATH_COUNT **Cur, int delay, int count);
void BuildNLongestPath(GATE *Node, int n, int NodeIndex, int PathSetIndex, int currPathDelay, int *PathIndex, int numPaths);

void freePathSet();

#endif // INITIALIZATION_H
