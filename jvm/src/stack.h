/*
 * stack
 *
 *  Created on: 25/02/2013
 *      Author: Clara
 */

#ifndef STACK_
#define STACK_



#endif /* STACK_ */

struct snode{
	unsigned int valor;
	struct snode *prox;
};

static struct snode * topo=NULL, *base=NULL;

unsigned int pop();
void  push(unsigned int valor);
void newFrame();
void dropFrame();
