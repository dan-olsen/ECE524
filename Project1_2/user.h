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

#define S0 0
#define R1 1
#define F0 2
#define S1 3
#define X1 4
#define X0 5

int Npi, Npo, Tgat, Tpat;		//Tot no of PIs,Pos,Maxid,Tot no of patterns in.vec,.fau
GATE Node[Mnod];		//Structure to store the ckt given in .isc file
int *patterns;

void readPatternFile(FILE*);

void initDelay();

void patternSim();

void storeRobustPaths(int ouputGate);

#endif /* USER_H_ */
