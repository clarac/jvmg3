#include <stdlib.h>
#include <stdio.h>
#include <stack.h>
#include <util.h>
#include <heap.h>

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
	push(ah,0);
	pushTipo(al,'D');
}

long long popLong(){
	unsigned int al,ah;
	al=pop();
	ah=pop();
	return toLong(ah,al);
}

void pushLong(long long l){
	push(getLHigh(l),0);
	pushTipo(getLlow(l),'J');
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

void  pushTipo(unsigned int valor, char tipo){
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
	unsigned int * local = calloc(lv_size, sizeof(unsigned int));
	checa(local);
	push((unsigned int) base,0);
	push((unsigned int) local,0);
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

void pushArray(struct Array a){
	push (a.arrayref,'A');
	push (a.length,'I');
}


unsigned int getLocalVar(int index){
	if(base==NULL){
		printf("Erro: array de variaveis locais inexistente. Retornando 0.\n");
		return 0;
	}
	unsigned int *  local = (unsigned int *) base->valor;
	return local[index];
}

void setLocalVar(int index, int value){
	if(base==NULL){
		printf("Erro: array de variaveis locais inexistente.\n");
		return;
	}
	unsigned int *  local = (unsigned int *) base->valor;
	local[index]=value;
}
/*
int mainTeste(){
	unsigned int i;


	push(0);

	for(i=1;i<21;i++){
		push(i);
	}

	newFrame(82);
	setLocalVar(12,871);
	printf("%u\n",getLocalVar(12));
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

*/
