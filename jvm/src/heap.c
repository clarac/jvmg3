#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <classloader.h>
#include "heap.h"


//Definicao do tamanho maximo da heap, o padrao do Java eh 128mb, dividi por 10 pra demorar menos pra compilar, depois retorna ao valor anterior.
#define HEAP_SIZE (134217728/5)

//Esses valores serao os padroes, mas eh necessario verificar se foram definidos outros valores
unsigned long long heap_min_size = HEAP_SIZE;
unsigned long long heap_max_size = HEAP_SIZE * 1.5;
unsigned long long used_heap_size;

unsigned int heap_count;
unsigned int objects_count;
unsigned int array_count;


void createHeap(){
	heap = calloc(1,heap_min_size);
	checa(heap);
	heap_count = 0;
	objects_count = 0;
	array_count = 0;
	used_heap_size = 0;
	printf("Heap was created.\n");
}
void verifyHeapSpace(){
	if (used_heap_size >= heap_min_size){
		if(used_heap_size >= heap_max_size){
			printf("ERROR: Out of heap memory.");
			exit(EXIT_FAILURE);
		}
		else{
			printf("Allocating more heap memory.");
			heap = realloc(heap, used_heap_size + sizeof(struct Heap));
			checa(heap);
		}
	}

}

//essa funcao nao foi testada ainda, possivel que ocorram erros
struct Object* newObject(struct class *instance){
	struct Object *obj;
	struct Heap *heap_item;
	struct ObjectField obj_field;
	unsigned int cont_variables = 0;
	unsigned int total_fields, j = 0;

	obj = calloc(1,sizeof(struct Object));
	checa(heap);
	verifyHeapSpace();
	obj->id = objects_count;
	objects_count++;

	total_fields = instance->f_count;
	obj->variables = calloc(total_fields, sizeof(struct ObjectField));
	checa(obj->variables);
	obj->instance = instance;
	while (j<total_fields){
		if (((instance->fields[j]->aflags) & 0x0008)==0){ //Static, nao eh variavel da instancia
			obj_field.index = instance->fields[j]->name_i;
			obj_field.element = 0;
			obj_field.name = instance->fields[j]->name;
			obj_field.descriptor = instance->fields[j]->descriptor;
			obj->variables[cont_variables] = obj_field;
			cont_variables++;
		}
		j++;
	}
	if(j!=cont_variables)
		obj->instance->has_static=1;
	obj->fields_count = cont_variables;

	obj->super = NULL;

	if (instance->super!=NULL && strcmp(instance->supername, "java/lang/Object") != 0){
		obj->super = newObject(instance->super);
	}

	heap_item = calloc(1,sizeof(struct Heap));
	checa(heap_item);
	heap_item->id = heap_count;
	heap_item->item.obj_item = obj;

	heap[heap_count] = heap_item;
	used_heap_size = used_heap_size + sizeof(struct Heap);
	heap_count++;


	return obj;

}



struct Array * createNewArray(unsigned int num_itens, unsigned int type){
	void *new_array;
	struct Array_type arrays;
	struct Array * array_item = calloc(1, sizeof(struct Array));
	struct Heap *heap_item;
	unsigned int size = 0;

	switch(type){
		case 4: //boolean
		case 5: //char
		case 8: //byte
			size = 1;
			break;
		case 9: //short
			size = 2;
			break;
		case 6: //float
		case 10: //int
			size = 4;
			break;
		case 11: //long
		case 7: //double
			size = 8;
			break;
	}

	verifyHeapSpace();

	new_array = calloc(num_itens, size);
	checa(new_array);
	arrays.id = array_count;
	array_item->arrayref = new_array;
 	array_item->length = num_itens;
 	arrays.array_element = &array_item;
 	arrays.size = size;
 	array_count++;

 	heap_item = calloc(1,sizeof(struct Heap));
 	checa(heap_item);
 	heap_item->id = heap_count;
 	heap_item->item.array_item = arrays;
 	heap[heap_count] = heap_item;
 	used_heap_size = used_heap_size + sizeof(struct Heap);
 	heap_count++;

	return array_item;
}

void setFieldValue(struct Object * obj, char * name, unsigned int value){
	int i;
	for(i=0;i<obj->fields_count;i++){
		if(strcmp(obj->variables[i].name,name)==0){
			obj->variables[i].element=value;
			return;
		}
	}
	if(obj->super!=NULL)
		return setFieldValue(obj->super,name,value);
	erroFatal("NoSuchFieldError");
}

void setDoubleFieldValue(struct Object * obj, char * name, unsigned int highvalue, unsigned int lowvalue){
	int i;
	for(i=0;i<obj->fields_count;i++){
		if(strcmp(obj->variables[i].name,name)==0){
			obj->variables[i].element=lowvalue;
			obj->variables[i].highv=highvalue;
			return;
		}
	}
	if(obj->super!=NULL)
		return setDoubleFieldValue(obj->super,name,highvalue,lowvalue);
	erroFatal("NoSuchFieldError");
}

unsigned int getFieldValue(struct Object * obj, char * name){
	int i;
	for(i=0;i<obj->fields_count;i++){
		if(strcmp(obj->variables[i].name,name)==0){
			return obj->variables[i].element;
		}
	}
	if(obj->super!=NULL)
		return getFieldValue(obj->super,name);
	erroFatal("NoSuchFieldError");
	return 0;
}

long long getLongFieldValue(struct Object * obj, char * name){
	int i;
	for(i=0;i<obj->fields_count;i++){
		if(strcmp(obj->variables[i].name,name)==0){
			return toLong(obj->variables[i].highv,obj->variables[i].element);
		}
	}
	if(obj->super!=NULL)
		return getLongFieldValue(obj->super,name);
	erroFatal("NoSuchFieldError");
	return 0;
}

//TODO arrumar (retornando valor errado?)
double getDoubleFieldValue(struct Object * obj, char * name){
	int i;
	double d=0;
	for(i=0;i<obj->fields_count;i++){
		if(strcmp(obj->variables[i].name,name)==0){
			d= toDouble(obj->variables[i].highv,obj->variables[i].element);
			return d;
		}
	}
	if(obj->super!=NULL)
		return getDoubleFieldValue(obj->super,name);
	erroFatal("NoSuchFieldError");
	return 0;
}

struct ReferenceArray * createNewObjectArray(unsigned int num_itens, char *type){
	struct Object **new_array;
	struct Array_type arrays;
	struct ReferenceArray * array_item;
	struct Heap *heap_item;

	verifyHeapSpace();

	new_array = calloc(num_itens, sizeof(struct Object));
	array_item->arrayref = *new_array;
 	array_item->length = num_itens;
 	array_item->type = type;

	return array_item;
}

/*
int main(){
	createHeap();
	printf("Created heap\n");
	struct Array array_test;
	array_test = newArray(20,10);
 	printf("%d\n", array_test.length);
	array_test.arrayref[0] = 5;
	printf("%d\n", array_test.arrayref[0]);

	printf("Created array\n");

	return 0;
}*/
