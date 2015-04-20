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

PATH_SET *pathSet;
int *primaryInputs;
int *primaryOutputs;

void InitDelay(GATE *Node, int Npi, int Npo, int Tgat);
void InitInputOuputArrays(GATE *Node, int Npi, int Npo, int Tgat);

void FreeInputOutputArrays();

void InsertPathCount(PATH_COUNT **Cur, int delay, int count);
void BuildNLongestPath(GATE *Node, int n, int NodeIndex, int PathSetIndex, int currPathDelay, int *PathIndex, int numPaths);

void FreePathSet(int Npo);

#endif // INITIALIZATION_H
