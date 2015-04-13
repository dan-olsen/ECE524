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

} PATH;

typedef struct PATH_SET_type {
    int Id;
    int numLongestPath;
    int numSecondLongestPath;
    PATH *longestPath;
    PATH *secondLongestPath;

} PATH_SET;

int Npi, Npo, Tgat, Tpat;        //Tot no of PIs,Pos,Maxid,Tot no of patterns in.vec,.fau
int verbose;
int *patterns;
PATH_SET *pathSet;
stackT pathStack;

void readPatternFile(FILE*);
void patternSim(GATE *Node);
void storeRobustPaths(GATE *Node, DdNode **RobustPathSet);
void clearPathZDDs(DdNode **RpathSet, DdNode **FpathSet, DdNode **RobustPathSet, DdNode **GoodPaths, DdNode **SuspectSet);
void clearNodeZDDs();
void freePathSet();

void applyPattern(GATE *Node, int i, int *patIndex, int *tmpVal);
void printInputVector(char *input);
void printPattern(int patIndex);
void initDelay(GATE *Node);
DdNode* createZDD(LIST *pathList);
void buildNLongestPath(GATE *Node, int n, int NodeIndex, int PathSetIndex, int currPathDelay, int *PathIndex, int numPaths);
void InsertPathCount(PATH_COUNT **Cur, int delay, int count);
int checkPathSensitivity(GATE *Node, LIST *path);
void storeGoodPaths(LIST *pathList, DdNode **GoodPaths);
void storeSuspectPaths(LIST *pathList, DdNode **SuspectSet);

#endif /* USER_H_ */
