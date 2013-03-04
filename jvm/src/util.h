/*
 * fpfunc.h
 *
 *  Created on: 15/02/2013
 *      Author: Clara
 */

#ifndef FPFUNC_H_
#define FPFUNC_H_




#include <stdint.h>
float toFloat(unsigned int bits);

unsigned int getBytes(float f);

double toDouble(unsigned int high, unsigned int low);

unsigned int getHigh(double d);

unsigned int getLow(double d);

unsigned short tou2(char a1, char a2);

unsigned int tou4(char a1, char a2, char a3, char a4);

int isNaN(double d);

int isNaNF(float f);

double LtoD(long long l);

long long DtoL(double d);

long long toLong(unsigned int h, unsigned int l);

unsigned int getLHigh(long long  l);

unsigned int getLlow(long long  l);

unsigned int getLastByte(int a);

#endif /* FPFUNC_H_ */
