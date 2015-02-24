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

int* readPatternFile(int*, FILE*, int);

void initDelay(GATE *Node, int Tgat);

void patternSim(GATE *Node, int *Patterns, int Tpat, int Tgat, int Npi, int Npo);

#endif /* USER_H_ */
