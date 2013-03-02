#include <stdio.h>
#include <classloader.h>
#include <stack.h>
#include <instrucoes.h>
#include <util.h>
#include <main.h>

void executaMetodo(struct method * m){
	//pc = primeira instrucao do metodo
	//combina com opcode
	//checa se tem parametro, manda bytes correspondentes OU faz as instrucoes cuidarem disso
	//testa se estah chamando println ou algo do tipo, simula a funcionalidade
}

int main(int argc, char *argv[]){


	if(argc<2){						// confere se foi fornecido o nome do .class
		printf("Informe o arquivo .class\n");
		return 1;
	}
	current=getFirst(argv[1]);
	printf("%d\n",getMethodCPIndex(current, "<init>", "java/lang/StringBuilder"));
	printf("%d\n",getMethodCPIndex(current, "main", "bat/tre/Hw"));
	//TODO inicializacao: chama metodo <init> depois chama metodo main
}


