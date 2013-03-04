/*
 * main.h
 *
 *  Created on: 02/03/2013
 *      Author: Clara
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <tipos.h>
#include <heap.h>
#include <stdlib.h>

struct class * current;
struct Object * curObj;
int pc, pcInc;

void executaMetodo();
void erroFatal(char * mensagem);
#endif /* MAIN_H_ */
