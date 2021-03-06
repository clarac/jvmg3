#include <stdio.h>
#include <classloader.h>
#include <stack.h>
#include <instrucoes.h>
#include <util.h>
#include <main.h>

void erroFatal(char * mensagem){
	printf("%s\n",mensagem);
	if(erroMsg!=NULL)
		printf("%s",erroMsg);
	exit(EXIT_FAILURE);
}


void executaMetodo(struct method * m){
	struct code *cod = m->code;
	pc = 0;
	int op;

	newFrame(cod->max_locals);

	if((m->aflags & 0x8) ==0){
		estatico=0;
		setLocalVar(0,(unsigned int) curObj);
	}else{
		estatico=1;
	}

	while(pc<cod->code_l){
		//if(pc>2)
			//return;
		pcInc = 1;
		op=getLastByte(cod->code[pc]);
		//printf("chamando instrucao 0x%X (%d)\n",op,op);
		chamaInst(op,cod);
		pc+=pcInc;

	}

}

void iniciaClasse(struct class * c){
	checa(c);
	if(mainClass==NULL)
		return;
	struct method * m = getMethod(c,"<init>","()V");
	struct Object * o = newObject(c);
	struct Object * oaux;
	struct class * aux;
	oaux=curObj;

	push(pcInc,0);
	push(pc,0);
	push((unsigned int)curObj,0);
	push(m->code->code_l,0);
	push((unsigned int)current,0);
	current = c;
	curObj=o;
	executaMetodo(m);
	if(strcmp(c->name,"java/lang/String")==0){
		pc=pop();
		pcInc=pop();
		return;
	}
	if(c->has_static==1){
		aux = mainClass;
		mainClass=c;
		m = getMethod(c,"<clinit>","()V");
		push(m->code->code_l,0);
		push((unsigned int)current,0);
		current=c;
		executaMetodo(m);
		mainClass=aux;
	}
	pc=pop();
	pcInc=pop();
}

int main(int argc, char *argv[]){
	if(argc<2){						// confere se foi fornecido o nome do .class
		printf("Informe o arquivo .class\n");
		return 1;
	}
	createHeap();
	erroMsg=NULL;
	current=getFirst(argv[1]);
	mainClass=current;
	checa(current);
	struct method * m;
	iniciaClasse(current);

	m = getMethod(current,"main","([Ljava/lang/String;)V");
	push((unsigned int)curObj,0);
	push(m->code->code_l,0);
	push((unsigned int)current,0);
	executaMetodo(m);
	return 0;
}


