/*
 * user.h
 *
 *  Created on: Feb 19, 2015
 *      Author: dolsen
 */

#ifndef USER_H_
#define USER_H_

#include "given.h"

#include <stdio.h>
#include <stdbool.h>

#define S0 0
#define R1 1
#define F0 2
#define S1 3
#define X1 4
#define X0 5

typedef struct PathSet_type {
	DdNode *node;

} PathSet;

typedef struct Robust_type {
	DdNode *Rpath, *Fpath;

} RobustSet;

typedef struct LongestPath_type {
	int **longest, **secondLongest;
	int *lengthLongest, *lengthSecondLongest;
	int numLongest, numSecondLongest;

} LongestPathsSet;

int Npi, Npo, Tgat, Tpat;		//Tot no of PIs,Pos,Maxid,Tot no of patterns in.vec,.fau
GATE Node[Mnod];				//Structure to store the ckt given in .isc file
PathSet suspectSet;
PathSet goodPaths;
RobustSet robustPaths;
int *patterns;

void readPatternFile(FILE*);

void initDelay();

void patternSim();

LongestPathsSet *findLongestPaths(int outputGate);

void storeRobustPaths();

DdNode *createZDD(int *pathArray, int length);

#endif /* USER_H_ */
