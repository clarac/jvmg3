#ifndef HEAP_H_
#define HEAP_H_





#include "tipos.h"
#include "util.h"
struct Array{
	unsigned int *arrayref;
	unsigned int length;
};

struct ReferenceArray{
	unsigned int length;
	struct Object *arrayref;
	char *type;

};

struct Array_type{
	unsigned int id;
	struct Array *array_element;
	unsigned int size;
};



struct ObjectField{
	unsigned int index;
	unsigned int element, highv;
	char * name, * descriptor;

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

struct Array * createNewArray(unsigned int num_itens, unsigned int type);
struct Object* newObject(struct class *instance);
void verifyHeapSpace();
void createHeap();
void setFieldValue(struct Object * obj, char * name, unsigned int value);
unsigned int getFieldValue(struct Object * obj, char * name);
long long getLongFieldValue(struct Object * obj, char * name);
double getDoubleFieldValue(struct Object * obj, char * name);
void setDoubleFieldValue(struct Object * obj, char * name, unsigned int highvalue, unsigned int lowvalue);
struct ReferenceArray * createNewObjectArray(unsigned int num_itens, char *type);
#endif /* HEAP_H_ */
