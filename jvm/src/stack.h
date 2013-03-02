/*
 * stack
 *
 *  Created on: 25/02/2013
 *      Author: Clara
 */

#ifndef STACK_
#define STACK_




#include <stdint.h>


unsigned int pop();
void  push(unsigned int valor);
void newFrame();
void dropFrame();
double popDbl();
void pushDbl(double a);
unsigned int getLocalVar(int index);
void setLocalVar(int index, int value);
long long popLong();
void pushLong(long long l);
struct Array popArray();
void pushArray(struct Array a);

#endif /* STACK_ */
