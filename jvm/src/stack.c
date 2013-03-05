#include <stdlib.h>
#include <stdio.h>
#include <stack.h>
#include <util.h>
#include <heap.h>
#include <main.h>
#include <classloader.h>

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

void newFrame(unsigned int lv_size){
	unsigned int * local = calloc(lv_size+1, sizeof(unsigned int));
	checa(local);
	local[0]=lv_size;
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
		erroFatal("Stack is empty");
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

void pushArray(struct Array a){
	push ((unsigned int) &a,0);
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

void mainTeste(){
	unsigned int i;

	newFrame(2);
	push(0,0);

	for(i=1;i<21;i++){
		push(i,0);
	}

	newFrame(82);
	//setLocalVar(12,871);
	//printf("%u\n",getLocalVar(12));
	for(;i<50;i++){
		push(i,0);
	}

	dropFrame();


	printf("base: %X\n",(unsigned int)base);
	while(topo!=NULL && topo!=base){
		printf("%u\n",topo->valor);
		printf("topo: %X\n",(unsigned int)topo);
		pop();
	}

}


