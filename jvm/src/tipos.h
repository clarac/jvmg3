/*
 * tipos.h
 *
 *  Created on: 18/02/2013
 *      Author: Clara
 */

#ifndef TIPOS_H_
#define TIPOS_H_





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
	unsigned int aflags, name_i, descriptor_i, a_count, value_h, value_l;
	struct attribute **atts;
	char *name, *descriptor;

};

struct exception{
	unsigned int startpc, endpc, handlerpc, catch_t;
};

struct code{
	unsigned int name_i,length, max_stack, max_locals, code_l, et_l, att_c;
	char *code;
	struct attribute **atts;
	struct exception **et;
};

struct method{
	unsigned int aflags, name_i, descriptor_i, a_count;
	struct attribute **atts;
	char *name, *descriptor;
	struct code  *code;
};



struct class{
	unsigned int minor_v, major_v, cpc, aflags, this_c, super_c, i_count, f_count, m_count, a_count;
	struct item **cpool;
	struct field **fields;
	struct method **methods;
	struct attribute **atts;
	unsigned int *interfaces;
	char *name, *supername;
	struct class *super;
};

#endif /* TIPOS_H_ */
