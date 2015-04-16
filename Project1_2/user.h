/*
 * user.h
 *
 *  Created on: Feb 19, 2015
 *      Author: dolsen
 */

#ifndef USER_H_
#define USER_H_

#include "given.h"
#include "stack.h"

#include <stdio.h>
#include <stdbool.h>

#define S0 0
#define R1 1
#define F0 2
#define S1 3
#define X1 4
#define X0 5

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

int Npi, Npo, Tgat, Tpat;        //Tot no of PIs,Pos,Maxid,Tot no of patterns in.vec,.fau
int *patterns;
PATH_SET *pathSet;
stackT pathStack;

void readPatternFile(FILE*);
void patternSim(GATE *Node);

void storePaths(GATE *Node, DdNode **PathSet);
void storeRPaths(GATE *Node, DdNode **RPathSet);
void storeFPaths(GATE *Node, DdNode **FPathSet);

void storeNnT(GATE *Node, DdNode **PathSet);
void storeRnT(GATE *Node, DdNode **PathSet);

void ListToZdd(LIST *pathList, DdNode **PathSet);

void clearPathZDDs(DdNode **PathSet);
void clearNodeZDDs(GATE *Node);
void freePathSet();

void initDelay(GATE *Node);

void applyPattern(GATE *Node, int i, int *patIndex, int *tmpVal);

int checkPathSensitivity(GATE *Node, LIST *path);
DdNode* createZDD(LIST *pathList);

void InsertPathCount(PATH_COUNT **Cur, int delay, int count);
void buildNLongestPath(GATE *Node, int n, int NodeIndex, int PathSetIndex, int currPathDelay, int *PathIndex, int numPaths);

void printInputVector(char *input);
void printPattern(int patIndex);

#endif /* USER_H_ */
