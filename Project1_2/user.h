/*
 * user.h
 *
 *  Created on: Feb 19, 2015
 *      Author: dolsen
 */

#ifndef USER_H_
#define USER_H_

#include "given.h"
#include "Initialization.h"
#include "Print.h"

#include <stdio.h>
#include <stdbool.h>

#define S0 0
#define R1 1
#define F0 2
#define S1 3
#define X1 4
#define X0 5

int *getNextPattern(FILE **Pat, int Npi);
void patternSim(GATE *Node, FILE *patFile, int Npi, int Npo, int Tgat);
void applyPattern(GATE *Node, int i, int *pattern, int Tgat);

void storePaths(GATE *Node, DdNode **PathSet, int Tgat);
void storeRPaths(GATE *Node, DdNode **RPathSet);
void storeFPaths(GATE *Node, DdNode **FPathSet);

void storeNnT(GATE *Node, DdNode **PathSet);
void storeRnT(GATE *Node, DdNode **PathSet);

void ListToZdd(LIST *pathList, DdNode **PathSet);

void clearPathZDDs(DdNode **PathSet);
void clearNodeZDDs(GATE *Node, int Tgat);

int checkPathSensitivity(GATE *Node, LIST *path);
DdNode* createZDD(LIST *pathList);

#endif /* USER_H_ */
