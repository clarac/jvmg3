#include <stdio.h>
#include "classloader.h"
#include "stack.h"
#include "instrucoes.h"
#include "util.h"
#include "main.h"
#include <string.h>

void erroFatal(char * mensagem){
	printf("%s\n",mensagem);
	if(erroMsg!=NULL)
		printf("%s",erroMsg);
	exit(EXIT_FAILURE);
}


void executaMetodo(struct method * m){
	struct code *cod = m->code;
	pc = 0;
	int op, i, v;

//	newFrame(m);

	encerrou = 0;

	while(pc<cod->code_l && encerrou == 0 ){
		encerrou = 0;
		pcInc = 1;
		op=getLastByte(cod->code[pc]);
		//printf("chamando instrucao 0x%X (%d)\n",op,op);
		chamaInst(op,cod);
		pc+=pcInc;

	}
	encerrou=0;

}

void iniciaClasse(struct class * c){
	checa(c);
	if(mainClass==NULL)
		return;
	struct method * m ;
	struct Object * o ;
	struct Object * oaux;
	struct class * aux;
	/*
	struct method * m = getMethod(c,"<init>","()V");
	struct Object * o = newObject(c);
	struct Object * oaux;
	struct class * aux;
	oaux=curObj;
	if(strcmp(c->name,"java/lang/String")==0){
		return;
	}


	current = c;
	curObj=o;
	newFrame(m);
	push((unsigned int)curObj,0);
	push(m->code->code_l,0);
	push((unsigned int)current,0);
	executaMetodo(m);
	*/
	m = tryMethod(c,"<clinit>","()V");
	if(m!=NULL){
		aux = mainClass;
		mainClass=c;
		push(pc,0);
		push(pcInc,0);

		newFrame(m,0);
		push((unsigned int)curObj,0);
		push(pc,0);
		push((unsigned int)current,0);
		current=c;

		executaMetodo(m);
		mainClass=aux;

		pcInc=pop();
		pc=pop();
	}

}

int main(int argc, char *argv[]){
	if(argc<2){						// confere se foi fornecido o nome do .class
		printf("Informe o arquivo .class\n");
		return 1;
	}
	if(argc>2){
		if(strcmp(argv[2],"view")==0){
			imprimeClasse=1;
			getFirst(argv[1]);
			return 0;
		}
	}
	imprimeClasse=0;
	createHeap();
	erroMsg=NULL;
	current=getFirst(argv[1]);
	//mainClass=current;
	checa(current);
	struct method * m;
	//iniciaClasse(current);

	m = getMethod(current,"main","([Ljava/lang/String;)V");
	newFrame(m,0);
	push((unsigned int)curObj,0);
	push(m->code->code_l,0);
	push((unsigned int)current,0);
	executaMetodo(m);
	return 0;
}
