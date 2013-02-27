/*
 * fpfunc.h
 *
 *  Created on: 15/02/2013
 *      Author: Clara
 */

#ifndef FPFUNC_H_
#define FPFUNC_H_



#endif /* FPFUNC_H_ */

float toFloat(unsigned int bits);

unsigned int getBytes(float f);

double toDouble(unsigned int high, unsigned int low);

unsigned int getHigh(double d);

unsigned int getLow(double d);

unsigned short tou2(char a1, char a2);

unsigned int tou4(char a1, char a2, char a3, char a4);

int isNaN(double d);

int isNaNF(float f);
