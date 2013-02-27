#include <stdlib.h>
#include <stdio.h>
#include <stack.h>
#include <util.h>

struct snode{
	unsigned int valor;
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
	if(topo==NULL || topo==base)
		return 0;
	struct snode * prox = topo->prox;
	unsigned int valor = topo->valor;
	free(topo);
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
	push(ah);
	push(al);
}

/*
 * Funcao que insere um item no topo da pilha
 *
 * param	valor	u4 a ser inserido
 *
 */

void  push(unsigned int valor){
	struct snode *novo = calloc(1,sizeof(struct snode));
	novo->valor=valor;
	novo->prox=topo;
	topo=novo;

}

/*
 * Funcao que cria um novo frame na pilha
 *
 */

void newFrame(unsigned int lv_size){
	unsigned int * local = calloc(lv_size, sizeof(unsigned int));
	push((unsigned int) base);
	push((unsigned int) local);
	base=topo;
}


/*
 * Funcao que remove o ultimo frame da pilha, liberando o espaco
 *
 */
void dropFrame(){
	unsigned int *local;

	if(topo == NULL)
		return;
	while(topo!=NULL&&topo!=base){
		pop();
	}
	struct snode * prox = topo->prox;
	unsigned int valor = topo->valor;
	free(topo);
	topo=prox;
	local=(struct snode *)valor;

	free(local);

	prox = topo->prox;
	valor = topo->valor;
	free(topo);
	topo=prox;
	base=(struct snode *)valor;

}

unsigned int getLocalIndex(int index){
	if(base==NULL){
		printf("Erro: array de variaveis locais inexistente. Retornando 0.\n");
		return 0;
	}
	unsigned int *  local = (unsigned int *) base->valor;
	return local[index];
}

void setLocalIndex(int index, int value){
	if(base==NULL){
		printf("Erro: array de variaveis locais inexistente.\n");
		return;
	}
	unsigned int *  local = (unsigned int *) base->valor;
	local[index]=value;
}

int mainTeste(){
	unsigned int i;


	push(0);

	for(i=1;i<21;i++){
		push(i);
	}

	newFrame(82);
	setLocalIndex(12,871);
	printf("%u\n",getLocalIndex(12));
	for(;i<50;i++){
		push(i);
	}

	//dropFrame();


	printf("base: %X\n",(unsigned int)base);
	while(topo!=NULL && topo!=base){
		printf("%u\n",topo->valor);
		printf("topo: %X\n",(unsigned int)topo);
		pop();
	}

	return 0;
}

