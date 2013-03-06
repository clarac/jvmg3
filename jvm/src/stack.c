#include <stdlib.h>
#include <stdio.h>
#include "stack.h"
#include "util.h"
#include "heap.h"
#include "main.h"
#include "classloader.h"

struct snode{
	unsigned int valor;
	char tipo;
	struct snode *prox;
};

static struct snode * topo=NULL, *base=NULL;

/**
 * Funcao que retira item do topo e retorna seu valor
 *
 * return	valor do topo da pilha
 *
 */
unsigned int pop(){
	if(topo==NULL)
		erroFatal("Stack is empty");
	if(topo==base)
		erroFatal("Stack frame is empty");
	struct snode * prox = topo->prox;
	unsigned int valor = topo->valor;
	//free(topo);
	topo=prox;
	return valor;
}

double popDbl(){
	unsigned int al,ah;
	double a;
	al=pop();
	ah=pop();
	a=toDouble(ah,al);
	return a;
}

void pushDbl(double a){
	unsigned int ah, al;
	ah=getHigh(a);
	al=getLow(a);
	push(ah,0);
	push(al,'D');
}

long long popLong(){
	unsigned int al,ah;
	al=pop();
	ah=pop();
	return toLong(ah,al);
}

void pushLong(long long l){
	push(getLHigh(l),0);
	push(getLlow(l),'J');
}
/*
 * Funcao que insere um item no topo da pilha
 *
 * param	valor	u4 a ser inserido
 *
 */

void  push(unsigned int valor, char tipo){
	struct snode *novo = calloc(1,sizeof(struct snode));
	checa(novo);
	novo->valor=valor;
	novo->tipo=tipo;
	novo->prox=topo;
	topo=novo;

}


char getTipo(){
	return topo->tipo;
}

void setTipo(char t){
	topo->tipo=t;
}

/*
 * Funcao que cria um novo frame na pilha
 *
 */

void newFrame(struct method * m, int objref){
	unsigned int * local = calloc(m->code->max_locals+1, sizeof(unsigned int));
	checa(local);
	local[0]=m->code->max_locals;

	int i,v;
	unsigned int a,b;
	i=1;
	v=local[0];
	if(objref==1){
		estatico=0;
		//v=2;
	}else{
		estatico=1;
	}
	i = strlen(m->descriptor) -2;
	while(m->descriptor[i]!=')'){
		i--;
	}
	i--;
	for(;i>0 && v>0 && m->descriptor[i]!=';' && m->descriptor[i]!='[' && m->descriptor[i]!='L';i--,v--){
		switch(m->descriptor[i]){
			case 'D':
			case 'J':
				b=pop();
				a=pop();
				local[v-1]=a;
				local[v]=b;
				//setLocalVar(v,a);
				//setLocalVar(v+1,b);
				v--;
				break;
			default:
				a=pop();
				local[v]=a;
				//setLocalVar(v,a);
				break;
		}
	}
	if(objref==1)
		local[1] = pop();

	push((unsigned int) base,0);
	push((unsigned int) local,0);
	//printf("local=0x%X\n",(unsigned int)local);
	//printf("base=0x%X\n",(unsigned int)base);
	base=topo;
}


/*
 * Funcao que remove o ultimo frame da pilha, liberando o espaco
 *
 */
void dropFrame(){
	unsigned int *local;
	struct snode * p;
	if(topo==NULL)
		return;//erroFatal("Stack is empty");
	while(topo!=NULL&&topo!=base){
		pop();
	}
	checa(topo);
	p = topo->prox;
	unsigned int valor = topo->valor;
	//free(topo);

	topo=p;
	checa(topo);
	local=(unsigned int *)valor;
	//free(local);
	p = topo->prox;
	valor = topo->valor;
	//free(topo);
	topo=p;
	base=(struct snode *)valor;
	//printf("local=0x%X\n",(unsigned int)local);
	//printf("base=0x%X\n",(unsigned int)base);
}

void pushArray(struct Array * a){
	push ((unsigned int) a,0);
}


struct Array * popArray(){
	struct Array * a = (struct Array *)pop();
	//a.length = pop();
	//a.arrayref = (unsigned int *)pop();
	return a;
}

unsigned int getLocalVar(int index){
	if(base==NULL){
		erroFatal("Stack frame is empty");
	}
	unsigned int *  local = (unsigned int *) base->valor;
	checa(local);
	if(index<0 || index >= local[0]){
		erroFatal("Access out of bounds");
	}
	return local[index+1];
}

void setLocalVar(int index, int value){
	if(base==NULL){
		erroFatal("Stack frame is empty");
	}
	unsigned int *  local = (unsigned int *) base->valor;
	checa(local);
	if(index<0 || index >= local[0]){
		erroFatal("Access out of bounds");
	}
	local[index+1]=value;
}



