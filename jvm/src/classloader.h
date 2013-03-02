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
struct field * getField(struct class * c, char * name);

#endif /* CLASSLOADER_H_ */
