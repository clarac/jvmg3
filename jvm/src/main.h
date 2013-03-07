/*
 * main.h
 *
 *  Created on: 02/03/2013
 *      Author: Clara
 */

#ifndef MAIN_H_
#define MAIN_H_

#include "tipos.h"
#include "heap.h"
#include <stdlib.h>

struct class * current, *mainClass;
struct Object * curObj;
int pc, pcInc, estatico, encerrou;
char * erroMsg;
int imprimeClasse;

void executaMetodo();
void erroFatal(char * mensagem);
void iniciaClasse(struct class * c);
#endif /* MAIN_H_ */
