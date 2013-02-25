#include <stdlib.h>
#include <stdio.h>
#include <stack.h>

/**
 * Funcao que retira item do topo e retorna seu valor
 *
 * return	valor do topo da pilha
 *
 */
unsigned int pop(){
	if(topo==NULL)
		return 0;
	struct snode * prox = topo->prox;
	unsigned int valor = topo->valor;
	free(topo);
	topo=prox;
	return valor;
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
	if(base==NULL)
		base=topo;
}

/*
 * Funcao que cria um novo frame na pilha
 *
 */

void newFrame(){
	push((unsigned int)base);
	base=topo;
}


/*
 * Funcao que remove o ultimo frame da pilha, liberando o espaco
 *
 */
void dropFrame(){
	struct snode * aux;
	if(topo == NULL || base == NULL)
		return;
	while(topo!=NULL&&topo!=base){
		pop();
	}
	if(topo!=NULL){
		base=pop();
	}

}

int mainTeste(){
	unsigned int i;


	push(0);

	for(i=1;i<21;i++){
		push(i);
	}
	newFrame();


	for(;i<50;i++){
		push(i);
	}

	dropFrame();


	printf("base: %X\n",(unsigned int)base);
	while(topo!=NULL && topo!=base){
		printf("%u\n",topo->valor);
		printf("topo: %X\n",(unsigned int)topo);
		pop();
	}
	printf("%u\n",topo->valor);
	printf("topo: %X\n",(unsigned int)topo);
	return 0;
}

