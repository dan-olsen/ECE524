#ifndef STOREPATHS_H
#define STOREPATHS_H

#include "given.h"
#include "PathSet.h"
#include "Cleanup.h"

void storeSensitizedPaths(GATE *Node, DdNode **PathSet, int Tgat);
void storeSensitizedRPaths(GATE *Node, DdNode **RPathSet);
void storeSensitizedFPaths(GATE *Node, DdNode **FPathSet);

void storeLSPaths(GATE *Node, int Npo, DdNode **GoodPaths, DdNode **SuspectSet);

void storeNnT(GATE *Node, DdNode **PathSet, int Tgat);
void storeRnT(GATE *Node, DdNode **PathSet, int Tgat);

void listToZdd(LIST *pathList, DdNode **PathSet);
DdNode* createZDD(LIST *pathList);
int checkPathSensitivity(GATE *Node, LIST *path);

#endif // STOREPATHS_H
