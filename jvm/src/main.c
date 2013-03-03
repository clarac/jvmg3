#include <stdio.h>
#include <classloader.h>
#include <stack.h>
#include <instrucoes.h>
#include <util.h>
#include <main.h>



void executaMetodo(struct method * m, struct Object * obj){
	struct code *cod = m->code;
	int i=0;
	pc = 0;
	int op;
	newFrame(cod->max_locals);
	if(obj!=NULL){
		setLocalVar(0,(unsigned int)obj);
		i=1;
	}
	/* parametros??
	for(;i<cod->max_locals;i++){
		setLocalVar(i,pop());
	}*/

	while(pc<cod->code_l){
		pcInc = 1;
		op=getLastByte(cod->code[pc]);
		printf("chamando instrucao 0x%X (%d)\n",op,op);
		chamaInst(op,cod);
		pc+=pcInc;
	}

	//checa se tem parametro, manda bytes correspondentes OU >>>>faz as instrucoes cuidarem disso
	//testa se estah chamando println ou algo do tipo, simula a funcionalidade
}

int main(int argc, char *argv[]){


	if(argc<2){						// confere se foi fornecido o nome do .class
		printf("Informe o arquivo .class\n");
		return 1;
	}
	createHeap();
	current=getFirst(argv[1]);
	struct method * m = getMethod(current,"<init>");
	push(m->code->code_l);
	push(current);
	struct Object * o = newObject(current);
	executaMetodo(m,o);
	double d=getDoubleFieldValue(o,"d");
	long long l=getLongFieldValue(o,"e");

	printf("%d\n",getFieldValue(o,"b"));
	printf("%f\n",toFloat(getFieldValue(o,"c")));
	printf("%f\n",d);
	printf("%lld\n",l);
	//TODO inicializacao: chama metodo <init> depois chama metodo main
}


