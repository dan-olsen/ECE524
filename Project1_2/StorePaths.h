#ifndef STOREPATHS_H
#define STOREPATHS_H

#include "given.h"
#include "PathSet.h"
#include "Cleanup.h"
#include "Initialization.h"
#include "Transitions.h"

#define RO 1
#define NR 2

void extractPDFs(GATE *Node, DdNode **PathSet, int Npo, int Tgat);
void extractRFPDFs(GATE *Node, DdNode **PathSet, int Npo, int Tgat);

void storeLSPaths(GATE *Node, int Npo, DdNode **GoodPaths, DdNode **SuspectSet);

void storeNnT(GATE *Node, int i, DdNode **PathSet);
void storePnT(GATE *Node, int i, DdNode **PathSet);
void storeRnT(GATE *Node, DdNode **PathSet, int Tgat);

void extractVNR(GATE *Node, DdNode **PathSet, int Npo);

void listToZdd(LIST *pathList, DdNode **PathSet);
DdNode* createZDD(LIST *pathList);
int checkPathSensitivity(GATE *Node, LIST *path);

void checkRobustlyTestedOffInput(GATE* Node, int i, DdNode **RobustPathSet);
void setValidateMark(GATE* Node, int i);

DdNode *containment(DdManager *zdd, DdNode *P, DdNode *Q);
void eliminate(DdNode **P, DdNode **Q);

#endif // STOREPATHS_H
