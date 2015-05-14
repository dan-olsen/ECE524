#ifndef CLEANUP_H
#define CLEANUP_H

#include "given.h"

void clearPathZDDs(DdNode **PathSet);
void clearNodeZDDs(GATE *Node, int Tgat);

void clearNodeRnT(GATE *Node, int Tgat);
void clearNodeNnt(GATE *Node, int Tgat);
void clearNodePnt(GATE *Node, int Tgat);

#endif // CLEANUP_H
