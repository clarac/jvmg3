#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <util.h>
#include <tipos.h>
#include <classloader.h>
#include <main.h>

static char *root;
char *name;


void checa(void * ptr){
	if(ptr==NULL)
		erroFatal("OutOfMemoryError");
}

char leChar(FILE * f){
	char c;
	if(fscanf(f,"%c",&c)==EOF){
		erroFatal("ClassFormatError");
		return NULL;
	}else return c;
}

struct code * readCode(FILE * bc, unsigned int cpc, struct item ** pdc);

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
	c1=leChar(bc);
	c2=leChar(bc);
	c3=leChar(bc);
	c4=leChar(bc);
	unsigned int t = tou4(c1,c2,c3,c4);
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
	l1=leChar(bc);
	l2=leChar(bc);
	ls=tou2(l1,l2);
	if(max>0){
		tm=(unsigned short) max;
		if(ls==0 || ls>=tm){
			erroFatal("ClassFormatError");
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

struct item * getN(int n, struct item ** cpool,int cpc){
	if(n>0 && n<cpc){
		return cpool[n-1];
	}
	else
		erroFatal("ClassFormatError");
	return NULL;
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

	struct item *atual, *aux, *aux2;
	unsigned int i, tag;							// iterator para o vetor

	for(i=1;i<tam;i++){			// percorre enquanto nao chegou ao final

		atual=pdc[i-1];					// pega proximo item

		tag = atual->tag;

		switch(tag){
			case 7: //CONSTANT_Class
				atual->name=getN(atual->name_index,pdc,tam)->string;
				break;

			case 9: //CONSTANT_Fieldref
			case 10: //CONSTANT_Methodref
			case 11: //CONSTANT_InterfaceMethodref
				aux=getN(atual->class_index,pdc,tam);
				aux2=getN(aux->name_index,pdc,tam);
				atual->class=aux2->string;
				atual->name_and_type=getN(atual->name_and_type_index, pdc,tam);

				break;

			case 8: //CONSTANT_String
				aux=getN(atual->string_index,pdc,tam);
				atual->string=aux->string;
				break;

			case 3:
			case 4:
				break;

			case 5: //CONSTANT_Long
			case 6: //CONSTANT_Double
				i++;
				break;

			case 12: //CONSTANT_NameAndType
				aux=getN(atual->name_index,pdc,tam);
				atual->name=aux->string;

				aux=getN(atual->descriptor_index,pdc,tam);
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



struct attribute ** readAtts(unsigned int count, FILE * bc, unsigned int cpc, struct item ** pdc, struct code **c){
	struct attribute **atts, *att, **first;
	char *str, lido;
	unsigned int j,k;
	if(count==0)
		return NULL;
	atts=calloc(count, sizeof(struct attribute *));
	checa(atts);
	first=atts;
	if(c!=NULL)
		*c=NULL;
	for(j=0;j<count;j++){
		att=calloc(1,sizeof(struct attribute));
		checa(att);
		*atts=att;
		atts++;

		att->name_i=pru2(bc,cpc);
		att->name=getN(att->name_i,pdc,cpc)->string;
		att->length=pru4(bc);
		if(c!=NULL && strcmp(att->name, "Code")==0){
			struct code *sc = readCode(bc,cpc,pdc);
			sc->name_i=att->name_i;
			sc->length= att->length;
			*c=sc;
		}else {
			if(att->length>0){
				att->info=calloc(att->length+1,sizeof(char));
				checa(att);
				str=att->info;
			}
			for(k=0;k<att->length;k++){
				lido=leChar(bc);
				*str=lido;
				str++;
			}
			if(k>0) *str='\0';
		}

	}
	return first;
}

struct code * readCode(FILE * bc, unsigned int cpc, struct item ** pdc){
	struct code * cod;
	struct exception * exp;
	int j;
	char l;
	cod=calloc(1,sizeof(struct code));
	checa(cod);
	cod->max_stack=pru2(bc,0);
	cod->max_locals=pru2(bc,0);
	cod->code_l=pru4(bc);

	cod->code=calloc(cod->code_l,sizeof(char));
	checa(cod);
	for(j=0;j<cod->code_l;j++){
		l=leChar(bc);
		cod->code[j]=l;
	}
	cod->et_l=pru2(bc,0);
	cod->et=calloc(cod->et_l, sizeof(struct exception *));
	checa(cod->et);
	for(j=0;j<cod->et_l;j++){
		exp=calloc(1, sizeof(struct exception));
		checa(exp);
		exp->startpc=pru2(bc,0);
		exp->endpc=pru2(bc,0);
		exp->handlerpc=pru2(bc,0);
		exp->catch_t=pru2(bc,0);
		cod->et[j]=exp;
	}
	cod->att_c=pru2(bc,0);
	cod->atts=readAtts(cod->att_c,bc,cpc,pdc,NULL);
	return cod;
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


struct field * getField(struct class * c, char * name){
	int i;
	for(i=0;i<c->f_count;i++){
		if(strcmp(name,c->fields[i]->name)==0){
			return c->fields[i];
		}
	}
	//TODO tratar nao encontrado
	return NULL;
}

struct method * getMethod(struct class * c, char * name){
	int i;
	for(i=0;i<c->m_count;i++){
		if(strcmp(name,c->methods[i]->name)==0){
			return c->methods[i];
		}
	}
	//TODO tratar nao encontrado
	return NULL;
}

struct method * getMethodByCPIndex(struct class * c, unsigned int index){
	struct item * methodInfo = c->cpool[index-1];
	return getMethod(getClass(methodInfo->class),methodInfo->name_and_type->name);
}

char * getMethodNameByCPIndex(struct class * c, unsigned int index){
	struct item * methodInfo = c->cpool[index-1];
	return methodInfo->name_and_type->name;
}

char * getMethodClassName(struct class * c, unsigned int index){
	struct item * methodInfo = c->cpool[index-1];
	return methodInfo->class;
}

unsigned int getMethodCPIndex(struct class * c, char * name, char * classe){
	int i;
	for(i=0;i<c->cpc-1;i++){
		if(c->cpool[i]!=NULL && c->cpool[i]->tag ==10  && strcmp(name,c->cpool[i]->name_and_type->name)==0 && strcmp(classe, c->cpool[i]->class)==0){
			return i+1;
		}
	}
	//TODO tratar nao encontrado
	return 0;
}


struct class * getClassByName(char * pathname){
	int i;
	struct class * thisc;
	char *name = calloc(strlen(pathname),sizeof(char));
	checa(name);
	strcpy(name,pathname);
	for(i=0;name[i]!='\0';i++){
		if(name[i]=='.'){
			name[i]='\0';
			i--;
		}
	}
	for(i=0;i<c_count;i++){
		thisc=classes[i];
		if(strcmp(thisc->name,name)==0){
			return thisc;
		}
	}
	return NULL;
}

/*
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


struct class * getClass(char *pathname){

	FILE *bc;						// arquivo .class
	struct item *atual;
	unsigned int cpc, aux1, aux2,i,j, tag, *it;
	char lido, stc[4],*cpt, *caminho;
	struct item **pdc;
	struct class *thisc;
	struct field *fd, **fds;
	struct method *mtd, **mtds;


	thisc=getClassByName(pathname);
	if(thisc!=NULL)
		return thisc;


	for(i=0;pathname[i]!='\0';i++){
		if(pathname[i]=='/')
			pathname[i]='\\';
	}

	caminho=calloc(strlen(root)+strlen(pathname), sizeof(char));
	checa(caminho);
	strcpy(caminho,root);

	strcat(caminho,pathname);

	bc=fopen(caminho,"rb");

	if(bc==0)
		erroFatal("NoClassDefFoundError");

	c_count++;
	classes=realloc(classes,(c_count*sizeof(struct class *)));
	checa(classes);
	thisc= calloc(1,sizeof(struct class));
	checa(thisc);
	classes[c_count-1]=thisc;

	for(i=0;i<4;i++){
		stc[i]=leChar(bc);			// le primeiro u4 (4 bytes)
	}
	if(tou4(stc[0],stc[1],stc[2],stc[3]) != 0xCAFEBABE){	// confere se bate com CAFEBABE
		erroFatal("ClassFormatError");
	}

	//printf("O numero magico eh 0xCAFEBABE\n");		// estah ok, pode ler

	thisc->minor_v=pru2(bc,0);
	thisc->major_v=pru2(bc,0);

	if(thisc->major_v <45 || thisc->major_v >46 || (thisc->major_v ==46 && thisc->minor_v>0)){
		erroFatal("UnsupportedClassVersionError");
	}

	thisc->cpc = cpc=pru2(bc,0);

	atual=NULL;
	pdc=calloc(cpc-1,sizeof(pdc));					// aloca espaco para vetor de itens
	checa(pdc);										// de tamanho cpc-1

	thisc->cpool = pdc;

	for(i=1;i<cpc;i++){					// percorre pool de constantes
		lido=leChar(bc);
		tag = (int) lido;
		if(tag<1 || tag>12 || tag==2){					// confere se a tag eh valida
			erroFatal("ClassFormatError");					// aborta se nao for
		}

		atual=calloc(1,sizeof(struct item));			// aloca espaco para proximo item
		checa(atual);

		thisc->cpool[i-1]=atual;

		atual->tag=(unsigned int) tag;

		switch(tag){ // preenche item com propriedades lidas, de acordo com a tag especifica

			case 7: //CONSTANT_Class
				aux1=pru2(bc, cpc);
				atual->name_index=aux1;

				if(aux1==0)					//indice invalido ou EOF
					return NULL;
				break;

			case 9: //CONSTANT_Fieldref
			case 10: //CONSTANT_Methodref
			case 11: //CONSTANT_InterfaceMethodref
				aux1=pru2(bc, cpc);
				aux2=pru2(bc, cpc);
				atual->class_index=aux1;
				atual->name_and_type_index=aux2;
				break;

			case 8: //CONSTANT_String
				aux1=pru2(bc, cpc);
				atual->string_index=aux1;

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
				thisc->cpool[i-1]=NULL;
				break;

			case 12: //CONSTANT_NameAndType
				aux1=pru2(bc, cpc);
				aux2=pru2(bc, cpc);
				atual->name_index=aux1;
				atual->descriptor_index=aux2;
				break;

			case 1: //CONSTANT_Utf8
				aux1=pru2(bc, 0);
				atual->length=aux1;
				atual->string=calloc(aux1+1, sizeof(char));
				checa(atual->string);
				cpt=atual->string;

				for(j=0;j<aux1;j++){
					lido=leChar(bc);
					cpt[j]=lido;
				}
				cpt[j]='\0';
				break;
		}


	}
	setStrings(thisc->cpool, thisc->cpc);



	thisc->aflags=pru2(bc,0);
	thisc->this_c=pru2(bc,0);
	thisc->super_c=pru2(bc,0);

	thisc->super=NULL;

	if(thisc->super_c>0)
		thisc->supername=getN(thisc->super_c,thisc->cpool, thisc->cpc)->name;

	thisc->i_count=pru2(bc,0);
	thisc->name=getN(thisc->this_c,thisc->cpool, thisc->cpc)->name;
	it=calloc(thisc->i_count, sizeof(unsigned int));
	checa(it);

	thisc->interfaces=it;

	for(i=0;i<thisc->i_count;i++ ,it++){
		*it=pru2(bc,0);
	}

	thisc->f_count=pru2(bc,0);
	thisc->fields=NULL;

	if(thisc->f_count>0){
		fds=calloc(thisc->f_count,sizeof(struct field *));
		checa(fds);
		thisc->fields=fds;
	}


	for(i=0;i<thisc->f_count;i++){
		fd=calloc(1,sizeof(struct field));
		checa(fd);
		fds[i]=fd;
		fd->aflags=pru2(bc,0);
		fd->value_h=0;
		fd->value_l=0;
		fd->name_i=pru2(bc,cpc);
		fd->name=getN(fd->name_i,pdc,cpc)->string;
		fd->descriptor_i=pru2(bc,cpc);
		fd->descriptor=getN(fd->descriptor_i,pdc,cpc)->string;
		fd->a_count=pru2(bc,0);
		fd->atts=readAtts(fd->a_count,bc,thisc->cpc,thisc->cpool,NULL);
	}

	thisc->m_count=pru2(bc,0);

	if(thisc->m_count>0){
		mtds=calloc(thisc->m_count, sizeof(struct method *));
		checa(mtds);
		thisc->methods=mtds;
	}

	for(i=0;i<thisc->m_count;i++){
		//method info
		mtd=calloc(1,sizeof(struct method));
		checa(mtd);
		mtds[i]=mtd;

		mtd->aflags=pru2(bc,0);
		mtd->name_i=pru2(bc,0);
		mtd->name=getN(mtd->name_i,pdc,cpc)->string;
		mtd->descriptor_i=pru2(bc,0);
		mtd->descriptor=getN(mtd->descriptor_i,pdc,cpc)->string;
		mtd->a_count=pru2(bc,0);
		mtd->atts=readAtts(mtd->a_count,bc,thisc->cpc,thisc->cpool,&mtd->code);
	}
	thisc->a_count=pru2(bc,0);
	thisc->atts=readAtts(thisc->a_count,bc,thisc->cpc,thisc->cpool,NULL);

	printf("carregou %s com sucesso\n",thisc->name);
	//printClass(thisc);

	fclose(bc);

	if(thisc->super_c>0){
		i=strlen(root)+ strlen(thisc->supername)+6;
		if(i>500)
			pathname=(char *)realloc(pathname,i+100);
		checa(pathname);
		strcpy(pathname,thisc->supername);
		strcat(pathname,".class\0");
		thisc->super=getClass(pathname);
	}

	return thisc;
}

int getRoot(char *path){
	int i=0, max=0;
	for(;path[i]!='\0';i++){
		if(path[i]=='\\'){
			max=i;
		}
	}
	return max;

}

struct class * getFirst(char * caminho){
	int indice;
	indice=getRoot(caminho);
	indice++;
	name=caminho;
	root=calloc(indice+1,sizeof(char));
	checa(root);
	if(indice>1){
		root=strncpy(root,name,indice);
		root[indice]='\0';
	}
	//TODO checar root de acordo com 1a classe, ajustar se necessário!
	//TODO NoClassDefFoundError, ClassCircularityError,IncompatibleClassChangeError

	//TODO NoSuchFieldError, NoSuchMethodError

	else{
		indice--;
		root[0]='\0';
	}
	name=calloc(500+indice,sizeof(char));
	checa(name);
	strcpy(name,caminho);
	name+=indice;

	return getClass(name);


}

//TODO Preparation involves creating the static fields for the class or interface and initializing
//those fields to their standard default values 0/null/false
