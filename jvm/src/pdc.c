/*
* 0826685 - Pool de Constantes
* 
*  Programa que recebe um arquivo .class como parametro
* verifica se o seu numero magico (4 primeiros bytes) eh 0xCAFEBABE,
* imprime o valor de minor_version, major_version, e
* de constant_pool_count e le e imprime os valores lidos 
* da pool de constantes, ignorando o resto do arquivo
*
* termina a operacao se o numero magico nao bater, se encontrar uma
* tag desconhecida (menor do que 1, maior do que 12 ou igual a 2)
* ou se tiver problemas ao abrir o arquivo classe
*
* para compilar:
*
* 	gcc -o pdc pdc.c
*
* recebe o arquivo .classe:
*
* 	./pdc Hw.class
*
* escrito e executado no Ubuntu 12.10 32 bit
* gcc 4.6.1 
*
*
* imprime constantes de tag: 
*
* CONSTANT_Class = 7
* CONSTANT_Fieldref = 9
* CONSTANT_Methodref = 10
* CONSTANT_InterfaceMethodref = 11
* CONSTANT_String = 8
* CONSTANT_Integer = 3
* CONSTANT_Float = 4
* CONSTANT_Long = 5
* CONSTANT_Double = 6
* CONSTANT_NameAndType = 12
* CONSTANT_Utf8 = 1
*
* e suas propriedades correspondentes
* 
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fpfunc.h>


char pool[13][30];			// vetor de strings, guarda nomes das tags nos seus indices correspondentes (inicializada na funcao main)



struct item{				// estrutura que guarda valor de um item do pool de constantes
	unsigned int tag, index;
	unsigned int name_index, class_index, name_and_type_index, descriptor_index, length, string_index, bytes,
	high_bytes, low_bytes;
	char *string;			// ponteiro para char, alocado durante a leitura para permitir um tamanho variavel
	char *name, *descriptor, *class;
	struct item *name_and_type;
};

struct attribute{
	unsigned int name_i, length;
	char *info, *name;
};

struct field{
	unsigned int aflags, name_i, descriptor_i, a_count;
	struct attribute **atts;
	char *name, *descriptor;
};



struct method{
	unsigned int aflags, name_i, descriptor_i, a_count;
	struct attribute **atts;
	char *name, *descriptor;
};



struct class{
	unsigned int minor_v, major_v, cpc, aflags, this_c, super_c, i_count, f_count, m_count, a_count;
	struct item **cpool;
	struct field **fields;
	struct method **methods;
	struct attribute **atts;
	unsigned int *interfaces;
	char *name;
	struct class *super;
};


/*
*	funcao que le um u4 de um arquivo e o retorna
*	
*	param	bc	ponteiro para o arquivo fonte
*	param	nome	nome do campo sendo lido (nao usado)
*
*	retorna u4 dos 4 proximos bytes do arquivo (big-endian)
*	retorna 0 se o arquivo acabar antes de ler 4 bytes
*/

unsigned int pru4(FILE *bc){
	char c1, c2, c3, c4;
	int s;
	s=fscanf(bc,"%c",&c1);
	if(s==EOF)
		return 0;

	s=fscanf(bc,"%c",&c2);
	if(s==EOF)
		return 0;

	s=fscanf(bc,"%c",&c3);
	if(s==EOF)
		return 0;

	s=fscanf(bc,"%c",&c4);
	if(s==EOF)
		return 0;

	unsigned int t = tou4(c1,c2,c3,c4);
	//printf("%s : %u\n",nome,t);
	return t;
}

/*
*	funcao que le um u2 de um arquivo e o retorna
*	
*	param	bc	ponteiro para o arquivo fonte
*	param	max	valor maximo para o valor lido (nao incluindo o proprio)
*			(se max = 0, considera que nao ha valor maximo)
*
*	retorna u2 dos 2 proximos bytes do arquivo (big-endian)
*	retorna 0 se o arquivo acabar antes de ler 2 bytes ou
*		se o numero lido for invalido (zero ou >= max)
*/

unsigned int pru2(FILE *bc, unsigned int max){
	unsigned short ls, tm;
	char l1, l2;
	int s;
	s=fscanf(bc,"%c",&l1);
	if(s==EOF)
		return 0;

	s=fscanf(bc,"%c",&l2);
	if(s==EOF)
		return 0;

	ls=tou2(l1,l2);
	if(max>0){
		tm=(unsigned short) max;
		if(ls==0 || ls>=tm){
			printf("erro: indice fora de alcance\n");
			return 0;
		}
	}

	return  (unsigned int) ls;
}

/*
*	funcao que retorna o item de posicao n de uma colecao de itens
*	assume que n eh um indice valido ( 1 <= n < tam da pdc)
*	
*	param	n	indice
*	param	pdc	ponteiro para primeiro item
*
*	retorna o enesimo item da lista
*
*/

struct item * getN(int n, struct item **pdc){
	pdc+=n-1;
	return *pdc;
}

/*
*	funcao que imprime toda a colecao de itens
*	
*	param	pdc		ponteiro para primeiro item
*	param	tam		numero de itens, considerando que os de tipo long e double ocupam 2 posicoes
*
*	retorna 1 se houve problema
*	retorna 0 se nao houve problema
*
*	assume que os indices gravados sao validos (checados durante a leitura do arquivo)
*
*/


int printItems(struct item **pdc, int tam){

	struct item *atual, *aux,**paux;
	int i, tag;
	unsigned int ui;

	paux=pdc;							// iterator para o vetor

	for(i=1;i<tam;i++,paux++){			// percorre enquanto nao chegou ao final

		atual=*paux;					// pega proximo item

		tag = (int) atual->tag;

		if(tag<1 || tag>12 || tag==2){					// confere se a tag eh valida
			printf("tag invalida e/ou desconhecida: %d\n",tag);
			return 1;						// aborta se nao for
		}


		printf("tag : %d (%s) \n", tag,pool[tag]);	// imprime a tag do item e seu nome correspondente

		switch(tag){
			case 7: //CONSTANT_Class

				printf("name_index : %d ",atual->name_index);	// indice do nome
				printf("(%s)\n",atual->name);			// imprime nome
				break;

			case 9: //CONSTANT_Fieldref
			case 10: //CONSTANT_Methodref
			case 11: //CONSTANT_InterfaceMethodref

				ui=atual->class_index;
				printf("class_index : %u ", ui);		// indice da classe
				printf("(%s)\n",atual->class);			// imprime nome da classe

				printf("name_and_type_index : %u ",atual->name_and_type_index); // indice de nome e tipo
				aux=atual->name_and_type;	// busca item de nome e tipo

				printf("(%s,",aux->name);			// imprime nome
				printf(" %s)\n",aux->descriptor);			// imprime tipo

				break;

			case 8: //CONSTANT_String
				printf("string_index : %d ",atual->string_index);// indice da string
				printf("(%s)\n",atual->string);			// imprime string

				break;

			case 3: //CONSTANT_Integer
				printf("bytes : 0x%X\n",atual->bytes);		// imprime valor em hexadecimal
				printf("int value : %d\n",atual->bytes); // valor int
				break;

			case 4: //CONSTANT_Float
				printf("bytes : 0x%X\n",atual->bytes);		// imprime valor em hexadecimal
				printf("float value : %.3f\n",toFloat(atual->bytes)); // valor float
				break;

			case 5: //CONSTANT_Long
				printf("high_bytes : 0x%X\n",atual->high_bytes);// imprime valor alto em hexadecimal
				printf("low_bytes : 0x%X\n",atual->low_bytes);	// imprime valor baixo em hexadecimal
				i++;
				paux++;
				break;

			case 6: //CONSTANT_Double
				printf("high_bytes : 0x%X\n",atual->high_bytes);// imprime valor alto em hexadecimal
				printf("low_bytes : 0x%X\n",atual->low_bytes);	// imprime valor baixo em hexadecimal
				double d=toDouble(atual->high_bytes,atual->low_bytes);
				printf("double value : %.3f\n",d); // valor double
				i++;
				paux++;
				break;

			case 12: //CONSTANT_NameAndType

				printf("name_index : %d ",atual->name_index);	// imprime indice do nome
				printf("(%s)\n",atual->name);			// imprime nome

				printf("descriptor_index : %d ",atual->descriptor_index); //indice do descritor
				printf("(%s)\n",atual->descriptor);			// imprime descritor
				break;

			case 1: //CONSTANT_Utf8
				printf("length : %u\n",atual->length);		// tamanho
				printf("bytes : %s\n",atual->string);		// conteudo
				break;
		}
		printf("\n");

	}
	return 0;
}

void setStrings(struct item **pdc, int tam){

	struct item *atual, *aux, *aux2, **paux;
	int i, tag;
	paux=pdc;							// iterator para o vetor

	for(i=1;i<tam;i++,paux++){			// percorre enquanto nao chegou ao final

		atual=*paux;					// pega proximo item

		tag = (int) atual->tag;

		switch(tag){
			case 7: //CONSTANT_Class
				atual->name=getN(atual->name_index,pdc)->string;
				break;

			case 9: //CONSTANT_Fieldref
			case 10: //CONSTANT_Methodref
			case 11: //CONSTANT_InterfaceMethodref
				aux=getN(atual->class_index,pdc);
				aux2=getN(aux->name_index,pdc);
				atual->class=aux2->string;

				atual->name_and_type=getN(atual->name_and_type_index, pdc);
				break;

			case 8: //CONSTANT_String
				aux=getN(atual->string_index,pdc);
				atual->string=aux->string;
				break;

			case 3:
			case 4:
				break;

			case 5: //CONSTANT_Long
			case 6: //CONSTANT_Double
				i++;
				paux++;
				break;

			case 12: //CONSTANT_NameAndType
				aux=getN(atual->name_index,pdc);
				atual->name=aux->string;

				aux=getN(atual->descriptor_index,pdc);
				atual->descriptor=aux->string;
				break;

			case 1: //CONSTANT_Utf8
				break;
		}
	}
}

void printAtts(unsigned int count, struct attribute ** atts){
	int j,k;
	struct attribute *att;
	char *str;
	for(j=0;j<count;j++,atts++){
		att=*atts;

		printf(">>>attribute #%d : %u (%s)\n",j,att->name_i,att->name);
		printf(">>>attribute_length : %u\n",att->length);
		printf(">>>info : ");
		str=att->info;
		for(k=0;k<att->length;k++,str++){
			printf("%X ",*str);
		}
		printf("\n");
	}
}

struct attribute ** readAtts(unsigned int count, FILE * bc, unsigned int cpc, struct item ** pdc){
	struct attribute **atts, *att, **first;
	char *str, lido;
	unsigned int j,k;
	if(count==0)
		return NULL;
	atts=calloc(count, sizeof(struct attribute *));
	first=atts;
	for(j=0;j<count;j++){
		att=malloc(sizeof(struct attribute));
		*atts=att;
		atts++;

		att->name_i=pru2(bc,cpc);
		att->name=getN(att->name_i,pdc)->string;
		att->length=pru4(bc);

		if(att->length>0){
			att->info=calloc(att->length+1,sizeof(char));
			str=att->info;
		}
		for(k=0;k<att->length;k++){
			fscanf(bc,"%c",&lido);
			*str=lido;
			str++;
		}
		*str='\0';
	}
	return first;
}

void printFields(unsigned int count, struct field ** fds){
	int i;
	struct field *fd;
	for(i=0;i<count;i++, fds++){
		fd=*fds;
		printf(">field #%d\n",i);
		printf(">>access_flags : %u\n",fd->aflags);
		printf(">>name_index : %u (%s)\n",fd->name_i,fd->name);
		printf(">>descriptor_index : %u (%s)\n",fd->descriptor_i,fd->descriptor);
		printf(">>attributes_count : %u\n",fd->a_count);

		printAtts(fd->a_count,fd->atts);

	}
}

void printMethods(unsigned int count, struct method ** mtds){
	int i;
	struct method *mtd;
	for(i=0;i<count;i++, mtds++){
		mtd=*mtds;
		printf(">method #%d\n",i);
		printf(">>access_flags : %u\n",mtd->aflags);

		printf(">>name_index : %u (%s)\n",mtd->name_i,mtd->name);

		printf(">>descriptor_index : %u (%s)\n",mtd->descriptor_i,mtd->descriptor);
		printf(">>attributes_count : %u\n",mtd->a_count);

		printAtts(mtd->a_count,mtd->atts);

	}
}

void printClass(struct class * thisc){
	unsigned int i, *it;
	printf("minor_version : %u\n",thisc->minor_v);
	printf("major_version : %u\n",thisc->major_v);
	printf("constant_pool_count : %u\n\n",thisc->cpc);
	printf("access_flags : 0x%X\n",thisc->aflags);
	printf("this_class : %u (%s)\n",thisc->this_c, thisc->name);
	printf("super_class : %u (-)\n",thisc->super_c);

	printItems(thisc->cpool, thisc->cpc);


	printf("interfaces_count : %u\n",thisc->i_count);

	it=thisc->interfaces;

	for(i=0;i<thisc->i_count;i++,it++){
		printf(">interface #%d : %u",i,*it);
	}

	printf("fields_count : %u\n",thisc->f_count);
	printFields(thisc->f_count,thisc->fields);

	printf("methods_count : %u\n",thisc->m_count);
	printMethods(thisc->m_count,thisc->methods);
	printf("attributes_count : %u\n",thisc->a_count);
	printAtts(thisc->a_count,thisc->atts);
}


/*
*	funcao principal
*	confere se o arquivo .class eh valido (CAFEBABE)	
*	le do arquivo e imprime na tela:
*		minor_version, major_version e constant_pool_count
*	le pool de constantes e armazena em uma lista
*	imprime os itens da pool de constantes na tela, na ordem de leitura
*
*	encerra e retorna 1 se o arquivo for invalido (i.e., se encontrar
*	indices de valor 0 ou >= constant_pool_count, se encontrar tags
*	desconhecidas, se achar EOF antes do termino da leitura) ou se
*	nao conseguir alocar memoria para guardar todas as constantes
*
*/


int main(int argc, char *argv[]){

	FILE *bc;						// arquivo .class
	struct item *atual;
	unsigned int cpc, aux1, aux2, aux3,i,j,k, tag, *it;
	int s;
	char lido, stc[4],*cpt;
	struct item **pdc, **paux;
	struct class *thisc = malloc(sizeof(struct class));
	struct attribute *att, **atts;
	struct field *fd, **fds;
	struct method *mtd, **mtds;

	strcpy(pool[0],"");					// preenche vetor de strings com os
	strcpy(pool[1],"CONSTANT_Utf8");			// tipos de constantes considerados
	strcpy(pool[2],"");
	strcpy(pool[3],"CONSTANT_Integer");
	strcpy(pool[4],"CONSTANT_Float");
	strcpy(pool[5],"CONSTANT_Long");
	strcpy(pool[6],"CONSTANT_Double");
	strcpy(pool[7],"CONSTANT_Class");
	strcpy(pool[8],"CONSTANT_String");
	strcpy(pool[9],"CONSTANT_Fieldref");
	strcpy(pool[10],"CONSTANT_Methodref");
	strcpy(pool[11],"CONSTANT_InterfaceMethodref");
	strcpy(pool[12],"CONSTANT_NameAndType");

	if(argc<2){						// confere se foi fornecido o nome do .class
		printf("Informe o arquivo .class\n");
		return 1;
	}
	bc=fopen(argv[1],"rb");	

	if(bc==0){
 		printf("erro ao abrir o arquivo %s\n",argv[1]);	// confere se o arquivo foi aberto com sucesso
		return 1;
	}

	for(i=0;i<4;i++){
		s=fscanf(bc,"%c",&stc[i]);			// le primeiro u4 (4 bytes)		
		if(s==EOF)
			return 1;
	}
	if(tou4(stc[0],stc[1],stc[2],stc[3]) != 0xCAFEBABE){	// confere se bate com CAFEBABE
		printf("erro: arquivo invalido, nao bate com o numero magico 0xCAFEBABE\n");
		fclose(bc);
		return 1;
	}

	printf("O numero magico eh 0xCAFEBABE\n");		// estah ok, pode ler

	thisc->minor_v=pru2(bc,0);
	thisc->major_v=pru2(bc,0);
	thisc->cpc = cpc=pru2(bc,0);

	atual=NULL;
	pdc=calloc(cpc-1,sizeof(pdc));					// aloca espaco para vetor de itens
	paux=pdc;										// de tamanho cpc-1

	thisc->cpool = pdc;

	if(pdc==NULL){
		printf("OOM error!\n");
		return 1;
	}

	for(i=1;i<cpc;i++){					// percorre pool de constantes
		s=fscanf(bc,"%c",&lido);
		if(s==EOF)
			return 1;
		tag = (int) lido;
		if(tag<1 || tag>12 || tag==2){					// confere se a tag eh valida
			printf("tag invalida e/ou desconhecida: %d\n cancelando leitura...\n",tag);
			fclose(bc);
			return 1;						// aborta se nao for
		}

		atual=malloc(sizeof(struct item));			// aloca espaco para proximo item
		if(atual==NULL){
				printf("OOM error!\n");
				return 1;
		}


		*paux=atual;					// escreve ponteiro para atual na posicao atual do vetor
		paux++;							// passa para proxima posicao


		atual->tag=(unsigned int) tag;

		switch(tag){ // preenche item com propriedades lidas, de acordo com a tag especifica

			case 7: //CONSTANT_Class
				aux1=pru2(bc, cpc);
				atual->name_index=aux1;	

				if(aux1==0)					//indice invalido ou EOF
					return 1;
				break;

			case 9: //CONSTANT_Fieldref	
			case 10: //CONSTANT_Methodref	
			case 11: //CONSTANT_InterfaceMethodref		
				aux1=pru2(bc, cpc);
				aux2=pru2(bc, cpc);
				atual->class_index=aux1;
				atual->name_and_type_index=aux2;

				if(aux1==0 || aux2==0)					//indice invalido ou EOF
					return 1;

				break;

			case 8: //CONSTANT_String
				aux1=pru2(bc, cpc);
				atual->string_index=aux1;	

				if(aux1==0)					//indice invalido ou EOF
					return 1;
				break;

			case 3: //CONSTANT_Integer
			case 4: //CONSTANT_Float
				aux1=pru4(bc);
				atual->bytes=aux1;

				break;

			case 5: //CONSTANT_Long
			case 6: //CONSTANT_Double
				aux1=pru4(bc);
				aux2=pru4(bc);
				atual->high_bytes=aux1;	
				atual->low_bytes=aux2;	
				i++;
				*paux=NULL;
				paux++;
				break;

			case 12: //CONSTANT_NameAndType
				aux1=pru2(bc, cpc);
				aux2=pru2(bc, cpc);
				atual->name_index=aux1;	
				atual->descriptor_index=aux2;	
				if(aux1==0 || aux2==0)					//indice invalido ou EOF
					return 1;
				break;

			case 1: //CONSTANT_Utf8
				aux1=pru2(bc, 0);
				atual->length=aux1;	
				atual->string=calloc(aux1+1, sizeof(char));
				cpt=atual->string;
				if(cpt==NULL)					// falha para alocar
					return 1;
				for(j=0;j<aux1;j++){
					fscanf(bc,"%c",&lido);
					*cpt=lido;
					cpt++;
				}
				*cpt='\0';
				break;	
		}


	}
	setStrings(thisc->cpool, thisc->cpc);

	thisc->super=NULL;

	thisc->aflags=pru2(bc,0);
	thisc->this_c=pru2(bc,0);
	thisc->super_c=pru2(bc,cpc);
	thisc->i_count=pru2(bc,0);
	thisc->name=getN(thisc->this_c,thisc->cpool);
	it=calloc(thisc->i_count, sizeof(unsigned int));

	thisc->interfaces=it;

	for(i=0;i<thisc->i_count;i++ ,it++){
		*it=pru2(bc,0);
	}

	thisc->f_count=pru2(bc,0);
	thisc->fields=NULL;

	if(thisc->f_count>0){
		fds=calloc(thisc->f_count,sizeof(struct field *));
		thisc->fields=fds;
	}


	for(i=0;i<thisc->f_count;i++){
		fd=malloc(sizeof(struct field));
		*fds=fd;
		fds++;

		fd->aflags=pru2(bc,0);
		fd->name_i=pru2(bc,cpc);
		fd->name=getN(fd->name_i,pdc)->string;
		fd->descriptor_i=pru2(bc,cpc);
		fd->descriptor=getN(fd->descriptor_i,pdc)->string;
		fd->a_count=pru2(bc,0);
		fd->atts=readAtts(fd->a_count,bc,thisc->cpc,thisc->cpool);
	}

	thisc->m_count=pru2(bc,0);

	if(thisc->m_count>0){
		mtds=calloc(thisc->m_count, sizeof(struct method *));
		thisc->methods=mtds;
	}

	for(i=0;i<thisc->m_count;i++){
		//method info
		mtd=malloc(sizeof(struct method));
		*mtds=mtd;
		mtds++;

		mtd->aflags=pru2(bc,0);
		mtd->name_i=pru2(bc,0);
		mtd->name=getN(mtd->name_i,pdc)->string;
		mtd->descriptor_i=pru2(bc,0);
		mtd->descriptor=getN(mtd->descriptor_i,pdc)->string;
		mtd->a_count=pru2(bc,0);
		mtd->atts=readAtts(mtd->a_count,bc,thisc->cpc,thisc->cpool);
	}
	thisc->a_count=pru2(bc,0);
	thisc->atts=readAtts(thisc->a_count,bc,thisc->cpc,thisc->cpool);


	printClass(thisc);

	fclose(bc);
	return 0;
}
