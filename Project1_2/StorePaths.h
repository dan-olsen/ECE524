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

void storeNnt(GATE *Node, int i, DdNode **PathSet);
void storePnt(GATE *Node, int i, int Tgat);
void storeRnT(GATE *Node, DdNode **PathSet, int Tgat);

void extractVNR(GATE *Node, DdNode **PathSet, int Npo, int Tgat);

void listToZdd(LIST *pathList, DdNode **PathSet);
DdNode* createZDD(LIST *pathList);
int checkPathSensitivity(GATE *Node, LIST *path);

void checkRobustlyTestedOffInput(GATE* Node, int i, DdNode **RobustPathSet);
int checkIfOffInput(GATE* Node, int i);
void setValidatedMark(GATE* Node, int i);
void setCheckValidateMark(GATE* Node, int i);
void setCosensitizationMark(GATE* Node, int i);

DdNode *containment(DdManager *zdd, DdNode *P, DdNode *Q);
void eliminate(DdNode **P, DdNode **Q);

#endif // STOREPATHS_H
