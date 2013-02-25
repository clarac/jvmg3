#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <util.h>
#include <tipos.h>
#include <classloader.h>
#include <stack.h>

char  *name;

/*
*	funcao que le um u4 de um arquivo e o retorna
*	
*	param	bc	ponteiro para o arquivo fonte
*	param	nome	nome do campo sendo lido (nao usado)
*
*	retorna u4 dos 4 proximos bytes do arquivo (big-endian)
*	encerra com erro se o arquivo acabar antes de ler 4 bytes
*/

unsigned int pru4(FILE *bc){
	char c1, c2, c3, c4;
	int s;
	s=fscanf(bc,"%c",&c1);
	if(s==EOF)
		exit(EXIT_FAILURE);

	s=fscanf(bc,"%c",&c2);
	if(s==EOF)
		exit(EXIT_FAILURE);

	s=fscanf(bc,"%c",&c3);
	if(s==EOF)
		exit(EXIT_FAILURE);

	s=fscanf(bc,"%c",&c4);
	if(s==EOF)
		exit(EXIT_FAILURE);

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
*	encerra com erro se o arquivo acabar antes de ler 2 bytes ou
*		se o numero lido for invalido (zero ou >= max)
*/

unsigned int pru2(FILE *bc, unsigned int max){
	unsigned short ls, tm;
	char l1, l2;
	int s;
	s=fscanf(bc,"%c",&l1);
	if(s==EOF)
		exit(EXIT_FAILURE);

	s=fscanf(bc,"%c",&l2);
	if(s==EOF)
		exit(EXIT_FAILURE);

	ls=tou2(l1,l2);
	if(max>0){
		tm=(unsigned short) max;
		if(ls==0 || ls>=tm){
			printf("erro: indice fora de alcance\n");
			exit(EXIT_FAILURE);
		}
	}

	return  (unsigned int) ls;
}

/*
*	funcao que retorna o item de posicao n de um array de itens
*	assume que n eh um indice valido ( 1 <= n < tam da pdc)
*	
*	param	n	indice
*	param	pdc	ponteiro para primeiro item
*
*	retorna o enesimo item do array
*
*/

struct item * getN(int n, struct item **pdc){
	pdc+=n-1;
	return *pdc;
}

/*
*	funcao que imprime toda a colecao de itens da pool de constantes
*	
*	param	pdc		ponteiro para primeiro item
*	param	tam		numero de itens, considerando que os de tipo long e double ocupam 2 posicoes
*
*	encerra se houve problema
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
			exit(EXIT_FAILURE);						// aborta se nao for
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

/*
 * Funcao que preenche propriedades das structs pool de constantes
 * com as strings correspondentes, apos o final da leitura de toda
 * a pdc
 *
 * param	pdc		ponteiro para pool de constantes
 * param 	tam		tamanho do pool de constantes
 *
 */

void setStrings(struct item **pdc, int tam){

	struct item *atual, *aux, *aux2;
	unsigned int i, tag;							// iterator para o vetor

	for(i=1;i<tam;i++){			// percorre enquanto nao chegou ao final

		atual=pdc[i-1];					// pega proximo item

		tag = atual->tag;

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

/*
 * Funcao que imprime todos os atributos em um array
 *
 *
 * param	count	tamanho (quantidade de atributos)
 * param	atts	ponteiro para primeira posicao
 */


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

/*
 * Funcao que le atributos do arquivo .class
 *
 * param	count	quantidade de atributos
 * param	bc		ponteiro para arquivo .class
 * param	cpc		tanho do pool de constantes
 * param	pdc		ponteiro para pool de constantes
 *
 * return	vetor de atributos lidos
 *
 */

struct attribute ** readAtts(unsigned int count, FILE * bc, unsigned int cpc, struct item ** pdc, struct attribute **code){
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
		if(code!=NULL && strcmp(att->name, "Code")==0){

			*code=att;
		}
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
		if(k>0) *str='\0';
	}
	return first;
}

/*
 * Funcao que imprime um vetor de fields
 *
 * param	count	tamanho do vetor
 * param	fds		ponteiro para vetor
 *
 */

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

/*
 * Funcao que imprime um vetor de metodos
 *
 * param	count	tamanho
 * param	mtds	ponteiro para vetor
 *
 */

void printMethods(unsigned int count, struct method ** mtds){
	int i, k;
	struct method *mtd;
	for(i=0;i<count;i++, mtds++){
		mtd=*mtds;
		printf(">method #%d\n",i);
		printf(">>access_flags : %u\n",mtd->aflags);

		printf(">>name_index : %u (%s)\n",mtd->name_i,mtd->name);

		printf(">>descriptor_index : %u (%s)\n",mtd->descriptor_i,mtd->descriptor);
		printf(">>attributes_count : %u\n",mtd->a_count);

		printAtts(mtd->a_count,mtd->atts);

		if(mtd->code!=NULL){
			printf("Code : %d ;  ",mtd->code->length);
			for(k=0;k<mtd->code->length;k++)
				printf("%X ",mtd->code->info[k]);

			printf("\n");
		}


	}
}

/*
 * Funcao que imprime uma classe
 *
 * param	thisc	ponteiro para classe a imprimir
 *
 */

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
 * Funcao que procura classe no vetor de classes carregadas
 *
 * param 	pathname	nome da classe, incluindo pacote
 *
 * return	ponteiro para classe se estiver carregada, NULL caso contrario
 *
 */

struct class * getClassByName(char * pathname){
	int i;
	struct class * thisc;
	char *name = calloc(strlen(pathname),sizeof(char));
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
*	Funcao que retorna ponteiro para classe
*	Procura no vetor de classes carregadas, retorna ponteiro para
*	classe se jah existir. Caso contrario,
*	confere se o arquivo .class eh valido (CAFEBABE)	
*	le arquivo .class e armazena em struct, adicionando ao
*	vetor de classes carregadas
*
*	param	pathname	nome da classe
*
*	return	ponteiro para classe carregada
*
*	encerra com erro se o arquivo for invalido (i.e., se encontrar
*	indices de valor 0 ou >= constant_pool_count, se encontrar tags
*	desconhecidas, se achar EOF antes do termino da leitura) ou se
*	nao conseguir alocar memoria para guardar tudo o que precisar
*
*/


struct class * getClass(char *pathname){

	FILE *bc;						// arquivo .class
	struct item *atual;
	unsigned int cpc, aux1, aux2,i,j, tag, *it;
	int s;
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


	printf("%d %d\n",strlen(root)+strlen(pathname), sizeof(char));
	//return NULL;
	caminho=calloc(strlen(root)+strlen(pathname), sizeof(char));
	return NULL;

	strcpy(caminho,root);

	strcat(caminho,pathname);
	//printf("%s\n",caminho);

	bc=fopen(caminho,"rb");

	if(bc==0){
		printf("erro ao abrir o arquivo %s\n",caminho);	// confere se o arquivo foi aberto com sucesso
		return NULL;
	}

	if(c_count%39==0)
		i=0;
	c_count++;
	classes=realloc(classes,(c_count*sizeof(struct class *)));
	thisc= malloc(sizeof(struct class));
	classes[c_count-1]=thisc;

	for(i=0;i<4;i++){
		s=fscanf(bc,"%c",&stc[i]);			// le primeiro u4 (4 bytes)		
		if(s==EOF)
			exit(EXIT_FAILURE);
	}
	if(tou4(stc[0],stc[1],stc[2],stc[3]) != 0xCAFEBABE){	// confere se bate com CAFEBABE
		printf("erro: arquivo invalido, nao bate com o numero magico 0xCAFEBABE\n");
		fclose(bc);
		exit(EXIT_FAILURE);
	}

	//printf("O numero magico eh 0xCAFEBABE\n");		// estah ok, pode ler

	thisc->minor_v=pru2(bc,0);
	thisc->major_v=pru2(bc,0);
	thisc->cpc = cpc=pru2(bc,0);

	atual=NULL;
	pdc=calloc(cpc-1,sizeof(pdc));					// aloca espaco para vetor de itens
											// de tamanho cpc-1

	thisc->cpool = pdc;

	if(pdc==NULL){
		printf("OOM error!\n");
		exit(EXIT_FAILURE);
	}

	for(i=1;i<cpc;i++){					// percorre pool de constantes
		s=fscanf(bc,"%c",&lido);
		if(s==EOF)
			exit(EXIT_FAILURE);
		tag = (int) lido;
		if(tag<1 || tag>12 || tag==2){					// confere se a tag eh valida
			printf("tag invalida e/ou desconhecida: %d\n cancelando leitura...\n",tag);
			fclose(bc);
			exit(EXIT_FAILURE);						// aborta se nao for
		}

		atual=malloc(sizeof(struct item));			// aloca espaco para proximo item
		if(atual==NULL){
				printf("OOM error!\n");
				exit(EXIT_FAILURE);
		}

		thisc->cpool[i-1]=atual;
		//*paux=atual;					// escreve ponteiro para atual na posicao atual do vetor
		//paux++;							// passa para proxima posicao


		atual->tag=(unsigned int) tag;

		switch(tag){ // preenche item com propriedades lidas, de acordo com a tag especifica

			case 7: //CONSTANT_Class
				aux1=pru2(bc, cpc);
				atual->name_index=aux1;	

				if(aux1==0)					//indice invalido ou EOF
					exit(EXIT_FAILURE);
				break;

			case 9: //CONSTANT_Fieldref	
			case 10: //CONSTANT_Methodref	
			case 11: //CONSTANT_InterfaceMethodref		
				aux1=pru2(bc, cpc);
				aux2=pru2(bc, cpc);
				atual->class_index=aux1;
				atual->name_and_type_index=aux2;

				if(aux1==0 || aux2==0)					//indice invalido ou EOF
					exit(EXIT_FAILURE);

				break;

			case 8: //CONSTANT_String
				aux1=pru2(bc, cpc);
				atual->string_index=aux1;	

				if(aux1==0)					//indice invalido ou EOF
					exit(EXIT_FAILURE);
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
				if(aux1==0 || aux2==0)					//indice invalido ou EOF
					return NULL;
				break;

			case 1: //CONSTANT_Utf8
				aux1=pru2(bc, 0);
				atual->length=aux1;	
				atual->string=calloc(aux1+1, sizeof(char));
				cpt=atual->string;
				if(cpt==NULL)					// falha para alocar
					exit(EXIT_FAILURE);
				for(j=0;j<aux1;j++){
					if(fscanf(bc,"%c",&lido)==EOF){
						exit(EXIT_FAILURE);
					}
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
		thisc->supername=getN(thisc->super_c,thisc->cpool)->name;

	thisc->i_count=pru2(bc,0);
	thisc->name=getN(thisc->this_c,thisc->cpool)->name;
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
		fds[i]=fd;
		//fds++;

		fd->aflags=pru2(bc,0);
		fd->name_i=pru2(bc,cpc);
		fd->name=getN(fd->name_i,pdc)->string;
		fd->descriptor_i=pru2(bc,cpc);
		fd->descriptor=getN(fd->descriptor_i,pdc)->string;
		fd->a_count=pru2(bc,0);
		fd->atts=readAtts(fd->a_count,bc,thisc->cpc,thisc->cpool,NULL);
	}

	thisc->m_count=pru2(bc,0);

	if(thisc->m_count>0){
		mtds=calloc(thisc->m_count, sizeof(struct method *));
		thisc->methods=mtds;
	}


	for(i=0;i<thisc->m_count;i++){
		//method info
		mtd=malloc(sizeof(struct method));
		mtds[i]=mtd;
		//mtds++;
		mtd->code=NULL;
		mtd->aflags=pru2(bc,0);
		mtd->name_i=pru2(bc,0);
		mtd->name=getN(mtd->name_i,pdc)->string;
		mtd->descriptor_i=pru2(bc,0);
		mtd->descriptor=getN(mtd->descriptor_i,pdc)->string;
		mtd->a_count=pru2(bc,0);
		mtd->atts=readAtts(mtd->a_count,bc,thisc->cpc,thisc->cpool,&mtd->code);

	}
	thisc->a_count=pru2(bc,0);
	thisc->atts=readAtts(thisc->a_count,bc,thisc->cpc,thisc->cpool,NULL);

	printf("carregou %s com sucesso\n",pathname);
	//printClass(thisc);

	fclose(bc);

	if(c_count==1 ){//&& ((strlen(thisc->name)+6)>strlen(pathname))){
		printf("oops\n");
		//return NULL;
		//fixRoot(strlen(thisc->name)-strlen(pathname));
	}

	if(thisc->super_c>98342){
		i=strlen(root)+ strlen(thisc->supername)+6;
		if(i>500)
			pathname=(char *)realloc(pathname,i);

		strcpy(pathname,thisc->supername);
		strcat(pathname,".class\0");

		thisc->super=getClass(pathname);
	}

	return thisc;
}

/*
 * Funcao que extrai caminho base da classe carregada
 *
 * param	path	caminho passado como parametro
 *
 * return 	indice da ultima barra contraria encontrada
 */

int getRoot(char *path){
	int i=0, max=0;
	for(;path[i]!='\0';i++){
		if(path[i]=='\\'){
			max=i;
		}
	}
	return max;

}

/*
 * Funcao principal
 */

int main(int argc, char *argv[]){
	classes = NULL;
	topo=NULL;
	base=NULL;
	int indice;
	if(argc<2){						// confere se foi fornecido o nome do .class
		printf("Informe o arquivo .class\n");
		return 1;
	}
	indice=getRoot(argv[1]);
	indice++;
	name=argv[1];
	root=calloc(indice+1,sizeof(char));
	if(indice>1){
		root=strncpy(root,name,indice);
		root[indice]='\0';
	}
	//TODO checar root de acordo com 1a classe, ajustar se necess�rio!
	else{
		indice--;
		root[0]='\0';
	}
	name=calloc(500+indice,sizeof(char));
	strcpy(name,argv[1]);
	name+=indice;

	getClass(name);

	printf("Total de %d classes carregadas\n",c_count);
	return 0;
}
