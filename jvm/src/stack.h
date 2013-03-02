/*
 * stack
 *
 *  Created on: 25/02/2013
 *      Author: Clara
 */

#ifndef STACK_
#define STACK_



#endif /* STACK_ */
#include <stdint.h>


unsigned int pop();
void  push(unsigned int valor);
void newFrame();
void dropFrame();
double popDbl();
void pushDbl(double a);
unsigned int getLocalIndex(int index);
void setLocalIndex(int index, int value);
long long popLong();
void pushLong( l);
