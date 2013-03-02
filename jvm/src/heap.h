#ifndef HEAP_H_
#define HEAP_H_





#include "tipos.h"
struct Array{
	unsigned int *arrayref;
	unsigned int length;
};

struct Array_type{
	unsigned int id;
	struct Array *array_element;
	unsigned int size;
};



struct ObjectField{
	unsigned int index;
	unsigned int element;
};

struct Object {
	unsigned int id;
	struct class *instance;
	struct Object *super;
	unsigned int fields_count;
	struct ObjectField *variables;
};

struct Heap{
	unsigned int id;
	union{
		struct Object *obj_item;
		struct Array_type array_item;
	}item;
};

struct Heap **heap;

struct Array createNewArray(unsigned int num_itens, unsigned int type);
struct Object* newObject(struct class *instance);
void verifyHeapSpace();
void createHeap();

#endif /* HEAP_H_ */
