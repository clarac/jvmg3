#include <stdlib.h>
#include <stdio.h>

struct snode{
	unsigned int valor;
	struct snode *prox;
};

struct snode * pop(struct snode * topo){
	if(topo==NULL)
		return NULL;
	struct snode * prox = topo->prox;
	free(topo);
	return prox;
}

struct snode *  push(struct snode * topo, unsigned int valor){
	struct snode *novo = calloc(1,sizeof(struct snode));
	novo->valor=valor;
	novo->prox=topo;
	return novo;
}

struct snode * newFrame(struct snode * topo, struct snode * base){
	struct snode *novo;
	novo=push(topo, (unsigned int)base);
	novo=push(novo, (unsigned int)topo);

	return novo;
}

struct snode * dropFrame(struct snode * topo, struct snode * base){
	struct snode * aux;
	if(topo == NULL || base == NULL)
		return NULL;
	while(topo!=NULL&&topo!=base){
		aux=topo->prox;
		free(topo);
		topo=aux;
	}
	if(topo!=NULL){
		aux=topo->prox;
		free(topo);
		topo=aux;
	}
	return topo;
}

int mainTeste(){
	unsigned int i;
	struct snode * topo=NULL,* base=NULL;


	topo=push(topo,0);
	base=topo;
	for(i=1;i<21;i++){
		topo=push(topo,i);
	}
	topo=newFrame(topo,base);
	base=topo;

	for(;i<50;i++){
		topo=push(topo,i);
	}

	topo=dropFrame(topo,base);
	base=(struct snode *)topo->valor;
	topo=pop(topo);

	printf("base: %X\n",(unsigned int)base);
	while(topo!=NULL && topo!=base){
		printf("%u\n",topo->valor);
		printf("topo: %X\n",(unsigned int)topo);
		topo=pop(topo);
	}

	return 0;
}

