/*
 * stack
 *
 *  Created on: 25/02/2013
 *      Author: Clara
 */

#ifndef STACK_
#define STACK_




#include <stdint.h>
#include <heap.h>

unsigned int pop();
void  push(unsigned int valor, char c);
void newFrame();
void dropFrame();
double popDbl();
void pushDbl(double a);
unsigned int getLocalVar(int index);
void setLocalVar(int index, int value);
long long popLong();
void pushLong(long long l);
void pushArray(struct Array * a);
struct Array * popArray ();
void mainTeste();
char getTipo();
void setTipo(char t);
#endif /* STACK_ */
