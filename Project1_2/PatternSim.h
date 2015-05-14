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
#include "Transitions.h"

#include <stdio.h>
#include <stdbool.h>

int *getNextPattern(FILE **Pat, int Npi);
void applyPatternRobust(GATE *Node, int *pattern, int Tgat);
void applyPatternNonRobust(GATE *Node, int *pattern, int Tgat);
void applyPatternFunctional(GATE *Node, int *pattern, int Tgat);
void printPattern(int *pattern, int Npi);

#endif /* USER_H_ */
