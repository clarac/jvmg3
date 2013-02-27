
/*
 * funcao que converte um u4 em float
 *
 * param 	bits	u4 contendo os bits para o float
 *
 * retorna float correspondente
 *
 */

float toFloat(unsigned int bits){
	float r, *pt;
	pt = (float *) &bits;
	r=*pt;
	return r;

}

/*
 * funcao que converte um float em u4
 *
 * param 	f	float a ser convertido
 *
 * retorna u2 correspondente
 *
 */

unsigned int getBytes(float f){
	unsigned int *pt;
	pt= (unsigned int *) &f;
	return *pt;
}

/*
 * funcao que converte dois u4 em um double
 *
 * param 	high	u4 contendo os bits altos
 * param	low		u4 contendo os bits baixos
 *
 * retorna double correspondente
 *
 */

double toDouble(unsigned int high, unsigned int low){
	double r, *pt;
	unsigned int *ipt;
	pt=&r;
	ipt=(unsigned int *) pt;
	*ipt=low;
	ipt++;
	*ipt=high;
	return *pt;
}

/*
 * funcao que converte o valor alto de um double em u4
 *
 * param 	d	double fonte
 *
 * retorna u4 correspondente ao valor alto (32 1os bits)
 *
 */

unsigned int getHigh(double d){
	unsigned int *pt;
	pt = (unsigned int *) &d;
	pt++;
	return *pt;
}

/*
 * funcao que converte o valor baixo de um double em u4
 *
 * param 	d	double fonte
 *
 * retorna u4 correspondente ao valor baixo (32 ultimos bits)
 *
 */

unsigned int getLow(double d){
	unsigned int *pt;
	pt = (unsigned int *) &d;
	return *pt;
}


/*
*	funcao que transforma um par de chars (bytes)
*	em um u2 (unsigned short, 16 bits)
*	param 	a1	byte de maior significancia
*	param	a2	byte de menor significancia
*
*	retorna unsigned short (u2)
*/

unsigned short tou2(char a1, char a2){
	unsigned short r, r2;
	r2=a2;
	r2&=0xFF;	// r2 = a2, com primeiro byte zerado
	r=a1;
	r<<=8;		// r = a1 no byte 1
	r|=r2;		// r = a1 no byte um ou (mais) a2 no byte 0
	return r;
}


/*
*	funcao que transforma quatro chars (bytes)
*	em um u4 (unsigned int, 32 bits)
*	param 	a1	byte de maior significancia (3)
*	param	a2	byte intermediario (2)
*	param 	a3	byte intermediario (1)
*	param	a2	de menor significancia (0)
*
*	retorna unsigned int (u4)
*
*/

unsigned int tou4(char a1, char a2, char a3, char a4){
	unsigned short s1, s2;
	unsigned int r, r2;
	s1=tou2(a1,a2);	// s1 = a1 no byte 1, a2 no byte 0
	s2=tou2(a3,a4);	// s2 = a3 no byte 1, a4 no byte 0
	r2=s2;
	r2&=0xFFFF;	// r2 = s2, com dois primeiros bytes zerados
	r=s1;
	r<<=16;		// r = s1 nos bytes 2 e 3
	r|=r2;		// r = a1 a2 a3 a4
	return r;
}

int isNaN(double d){

	unsigned int dh = getHigh(d);
	unsigned int t=0x7FF00000;
	dh&=t;
	if(dh == t){
		return 1;
	}
	else return 0;
}

