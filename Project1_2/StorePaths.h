#ifndef STOREPATHS_H
#define STOREPATHS_H

#include "given.h"
#include "PathSet.h"
#include "Cleanup.h"
#include "Initialization.h"
#include "Transitions.h"

void extractPDFs(GATE *Node, DdNode **PathSet, int Npo, int Tgat);
void extractRFPDFs(GATE *Node, DdNode **RPathSet, DdNode **FPathSet, int Npo, int Tgat);

void storeLSPaths(GATE *Node, int Npo, DdNode **GoodPaths, DdNode **SuspectSet);

void storeNnT(GATE *Node, DdNode **PathSet, int Tgat);
void storeRnT(GATE *Node, DdNode **PathSet, int Tgat);
void storePnT(GATE *Node, DdNode **PathSet, int Tgat);

void extractVNR(GATE *Node, DdNode **PathSet, int Npo);

void listToZdd(LIST *pathList, DdNode **PathSet);
DdNode* createZDD(LIST *pathList);
int checkPathSensitivity(GATE *Node, LIST *path);

DdNode *containment(DdManager *zdd, DdNode *P, DdNode *Q);
void eliminate(DdNode **P, DdNode **Q);

#endif // STOREPATHS_H
