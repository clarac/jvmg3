/*
 * classloader.h
 *
 *  Created on: 25/02/2013
 *      Author: Clara
 */

#ifndef CLASSLOADER_H_
#define CLASSLOADER_H_





static char pool[13][30]={"","CONSTANT_Utf8","","CONSTANT_Integer","CONSTANT_Float",
		"CONSTANT_Long","CONSTANT_Double","CONSTANT_Class","CONSTANT_String",
		"CONSTANT_Fieldref","CONSTANT_Methodref","CONSTANT_InterfaceMethodref",
		"CONSTANT_NameAndType"};			// vetor de strings, guarda nomes das tags nos seus indices correspondentes





static struct class **classes;
static int c_count=0;
struct class * getClass(char *pathname);
struct class * getFirst(char *pathname);
struct field * getField(struct class * c, char * name);
void checa(void * ptr);
struct method * getMethod(struct class * c, char * name, char * descriptor);
struct method * tryMethod(struct class * c, char * name, char * descriptor);
struct method * getMethodByCPIndex(struct class * c, unsigned int index);
char * getMethodNameByCPIndex(struct class * c, unsigned int index);
char * getMethodClassName(struct class * c, unsigned int index);
unsigned int getMethodCPIndex(struct class * c, char * name, char * classe, char * descriptor);
#endif /* CLASSLOADER_H_ */
