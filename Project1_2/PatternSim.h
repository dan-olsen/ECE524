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
#include "Cleanup.h"

#include <stdio.h>
#include <stdbool.h>

#define S0 0
#define R1 1
#define F0 2
#define S1 3
#define X1 4
#define X0 5

int *getNextPattern(FILE **Pat, int Npi);
void applyPattern(GATE *Node, int *pattern, int Tgat);
void printPattern(int *pattern, int Npi);

#endif /* USER_H_ */
