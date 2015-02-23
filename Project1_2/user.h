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

#define STABLE_0 0
#define RISING_1 1
#define FALLING_0 2
#define STABLE_1 3
#define X_1 4
#define X_0 5

int readPatternFile(int*, FILE*, int);

void initDelay(GATE *Node, int Tgat);

void patternSim(GATE *Node, int *Patterns, int Tgat, int Npi, int Npo);

#endif /* USER_H_ */
