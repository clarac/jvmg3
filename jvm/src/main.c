#include <stdio.h>
#include <classloader.h>

int main(int argc, char *argv[]){


	if(argc<2){						// confere se foi fornecido o nome do .class
		printf("Informe o arquivo .class\n");
		return 1;
	}
	getFirst(argv[1]);
}


