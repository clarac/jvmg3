#include <stack.h>
#include <util.h>
#include <stdio.h>
#include <stdlib.h>
#include <heap.h>
#include <main.h>
#include <string.h>
#include <classloader.h>
#include <instrucoes.h>

struct code * cptr;

unsigned int getByte(){
	int p = pc + pcInc -1;
	unsigned int a = (unsigned int) cptr->code[p+1];
	pcInc++;
	return a;
}

unsigned int getShort(){
	int p = pc + pcInc -1;
	unsigned int a = (unsigned int) cptr->code[p+1];
	unsigned int b = (unsigned int) cptr->code[p+2];
	a<<=8;
	a|=b;
	pcInc+=2;
	return a;
}

unsigned int getWord(){
	int p = pc + pcInc -1;
	unsigned int a = (unsigned int) cptr->code[p+1];
	unsigned int b = (unsigned int) cptr->code[p+2];
	unsigned int c = (unsigned int) cptr->code[p+3];
	unsigned int d = (unsigned int) cptr->code[p+4];
	a<<=8;
	a|=b;
	a<<=8;
	a|=c;
	a<<=8;
	a|=d;
	pcInc+=4;
	return a;
}

int getSignedByte(){
	int p = pc + pcInc -1;
	int a = (int) cptr->code[p+1];
	pcInc++;
	return a;
}

int getSignedShort(){
	int p = pc + pcInc -1;
	int a = (int) cptr->code[pc+1];
	unsigned int b = (unsigned int) cptr->code[p+2] & 0xFF;
	a<<=8;
	a|=b;
	if((a&0x8000)!=0){
		a|=0xFFFF0000;
	}

	pcInc+=2;

	return a;
}

int getSignedWord(){
	int p = pc + pcInc -1;
	int a = (int) cptr->code[p+1];
	unsigned int b = (unsigned int) cptr->code[p+2] & 0xFF;
	unsigned int c = (unsigned int) cptr->code[p+3] & 0xFF;
	unsigned int d = (unsigned int) cptr->code[p+4] & 0xFF;
	a<<=8;
	a|=b;
	a<<=8;
	a|=c;
	a<<=8;
	a|=d;
	pcInc+=4;
	return a;
}

void anewarray(){
	unsigned int index = getShort();
	struct item * a = current->cpool[index-1];
	struct class *c;
	c = getClass(a->class);
	unsigned int i = pop();
	struct ReferenceArray *n;
	n = createNewObjectArray(i, c->name);
	push((unsigned int)a,'A');
}

void new(){
	unsigned int index = getShort();
	struct item * a = current->cpool[index-1];
	struct class *c;
	struct Object *n;
	c = getClass(a->name);

	n = newObject(c);
	push((unsigned int)n,'O');
}

/* TODO Tenho minhas duvidas a respeito dessa instrucao... */
void multianewarray(){
	unsigned int index = getShort();
	unsigned int dimensions = getByte();
	int i=0;
	int temp=1;
	while (i<dimensions){
		temp = temp * pop();
		i++;
	}
	struct item * k = current->cpool[index-1];
	struct class *c;
	c = getClass(k->class);
	struct ReferenceArray *n;
	n = createNewObjectArray(temp, c->name);
	push((unsigned int)n,'A');
}

void aaload(){
	unsigned int i=pop();
	struct ReferenceArray *a;
	a = (struct ReferenceArray *)pop();
	if (a->arrayref == NULL){
		printf("NullPointerException\n");
		exit(EXIT_FAILURE);
	}
	if (i>=(a->length)){

		printf("ArrayVarOutOfBoundsException\n");
		exit(EXIT_FAILURE);
	}
	push(((unsigned int*)(a->arrayref))[i],'A');

}

void aastore(){
	struct Object *l;
	l = (struct Object *) pop();
	unsigned int i = pop();
	struct ReferenceArray *a;
	a = (struct ReferenceArray *)pop();
	if (a->arrayref == NULL){
		printf("NullPointerException\n");
		exit(EXIT_FAILURE);
	}
	if (i>=a->length){
		printf("ArrayVarOutOfBoundsException\n");
		exit(EXIT_FAILURE);
	}
	a->arrayref[i] = *l;

}


void aconst_null (){
	push(0,'C');
}

void dadd(){
	double a, b, r;
	b=popDbl();
	a=popDbl();
	r=a+b;
	pushDbl(r);
}


void dsub(){
	double a, b, r;
	b=popDbl();
	a=popDbl();
	r=a-b;
	pushDbl(r);
}

void dcmpg (){
	double a,b;
	b=popDbl();
	a=popDbl();
	if(isNaN(a)!=0 || isNaN(b)!=0){
		push(1,'T');
	}
	else{
		if(a>b)
			push(1,'T');
		else if(a==b)
			push(0,'T');
		else
			push(-1,'T');
	}
}

void dcmpl (){
	double a,b;
	b=popDbl();
	a=popDbl();
	if(isNaN(a)!=0 || isNaN(b)!=0){
		push(-1,'T');
	}
	else{
		if(a>b)
			push(1,'T');
		else if(a==b)
			push(0,'T');
		else
			push(-1,'T');
	}
}

void dconst_0(){
	double a=0.0;
	pushDbl(a);
}

void dconst_1(){
	double a=1.0;
	pushDbl(a);
}

void ddiv (){
	double a, b, r;
	b=popDbl();
	a=popDbl();
	r=a/b;
	pushDbl(r);
}

void dmul(){
	double a, b, r;
	b=popDbl();
	a=popDbl();
	r=a*b;
	pushDbl(r);
}
/*
 * In the remaining cases, where neither operand is an infinity, a zero, or NaN, the
 * floating-point remainder result from a dividend value1' and a divisor value2' is
 *  defined by the mathematical relation result = value1' - (value2' * q), where q
 *  is an integer that is negative only if value1' / value2' is negative, and
 *  positive only if value1' / value2' is positive, and whose magnitude is as
 *  large as possible without exceeding the magnitude of the true mathematical
 *  quotient of value1' and value2'.
 */
void drem_(){
	double a, b, r;
	int q;
	b=popDbl();
	a=popDbl();
	if(isNaN(a)||isNaN(b)||b==0.0){
		pushDbl(a);
	} else if(a==0){
		pushDbl(0.0);
	}else{
		q=(int)a/b;
		r=a-b*q;
		pushDbl(r);
	}

}

void dneg(){
	double a=popDbl();
	a=0.0-a;
	pushDbl(a);
}

void dup(){
	char t = getTipo();
	unsigned int a = pop();
	push(a,t);
	push(a,t);
}

void swap(){
	char at, bt;
	at = getTipo();
	unsigned int a = pop();
	bt=getTipo();
	unsigned int b = pop();
	push(a,at);
	push(b,bt);
}

void dup_x1(){
	char at, bt;
	at = getTipo();
	unsigned int a = pop();
	bt=getTipo();
	unsigned int b = pop();
	push(a,at);
	push(b,bt);
	push(a,at);
}

void dup_x2(){
	char at, bt,ct;
	at = getTipo();
	unsigned int a = pop();
	bt = getTipo();
	unsigned int b = pop();
	ct = getTipo();
	unsigned int c = pop();
	push(a,at);
	push(c,ct);
	push(b,bt);
	push(a,at);
}

void dup2(){
	char at = getTipo();
	unsigned int a = pop();
	char bt = getTipo();
	unsigned int b = pop();
	push(b,bt);
	push(a,at);
	push(b,bt);
	push(a,at);
}

void dup2_x1(){
	char at = getTipo();
	unsigned int a = pop();
	char bt = getTipo();
	unsigned int b = pop();
	char ct = getTipo();
	unsigned int c = pop();
	push(b,bt);
	push(a,at);
	push(c,ct);
	push(b,bt);
	push(a,at);
}

void dup2_x2(){
	char at = getTipo();
	unsigned int a = pop();
	char bt = getTipo();
	unsigned int b = pop();
	char ct = getTipo();
	unsigned int c = pop();
	char dt = getTipo();
	unsigned int d = pop();
	push(b,bt);
	push(a,at);
	push(d,dt);
	push(c,ct);
	push(b,bt);
	push(a,at);
}
/*
void aaload(){
	unsigned int *arrayref = (unsigned int *) pop();
	if (arrayref == NULL){
		printf("NullPointerException");
		exit(EXIT_FAILURE);
	}
	unsigned int index = pop();
	push((unsigned int )arrayref[index],'A');
}
*/

void aload(){
	unsigned int index = getByte();
	unsigned int a = getLocalVar(index);
	push(a,'C');
}

void aload_0(){
	unsigned int a = getLocalVar(0);
	push(a,'C');
}

void aload_1(){
	unsigned int a = getLocalVar(1);
	push(a,'C');
}

void aload_2(){
	unsigned int a = getLocalVar(2);
	push(a,'C');
}

void aload_3(){
	unsigned int a = getLocalVar(3);
	push(a,'C');
}

void astore(){
	unsigned int index = getByte();
	unsigned int a = pop();
	setLocalVar(index,a);
}

void astore_0(){
	unsigned int a = pop();
	setLocalVar(0,a);
}

void astore_1(){
	unsigned int a = pop();
	setLocalVar(1,a);
}

void astore_2(){
	unsigned int a = pop();
	setLocalVar(2,a);
}

void astore_3(){
	unsigned int a = pop();
	setLocalVar(3,a);
}

void bipush(){
	int byte = getSignedByte();
	push(byte,'I');
}

void dload(){
	unsigned int index = getByte();
	unsigned int ah, al;
	ah=getLocalVar(index);
	al=getLocalVar((index)+1);
	double a = toDouble(ah,al);
	pushDbl(a);
}

void dload_0(){
	unsigned int ah, al;
	ah=getLocalVar(0);
	al=getLocalVar(1);
	double a = toDouble(ah,al);
	pushDbl(a);
}

void dload_1(){
	unsigned int ah, al;
	ah=getLocalVar(1);
	al=getLocalVar(2);
	double a = toDouble(ah,al);
	pushDbl(a);
}

void dload_2(){
	unsigned int ah, al;
	ah=getLocalVar(2);
	al=getLocalVar(3);
	double a = toDouble(ah,al);
	pushDbl(a);
}

void dload_3(){
	unsigned int ah, al;
	ah=getLocalVar(3);
	al=getLocalVar(4);
	double a = toDouble(ah,al);
	pushDbl(a);
}

void dstore(){
	unsigned int index = getByte();
	unsigned int ah, al;
	double a = popDbl();
	ah=getHigh(a);
	al=getLow(a);
	setLocalVar(index,ah);
	setLocalVar((index)+1,al);
}

void dstore_0(){
	unsigned int ah, al;
	double a = popDbl();
	ah=getHigh(a);
	al=getLow(a);
	setLocalVar(0,ah);
	setLocalVar(1,al);
}

void dstore_1(){
	unsigned int ah, al;
	double a = popDbl();
	ah=getHigh(a);
	al=getLow(a);
	setLocalVar(1,ah);
	setLocalVar(2,al);
}

void dstore_2(){
	unsigned int ah, al;
	double a = popDbl();
	ah=getHigh(a);
	al=getLow(a);
	setLocalVar(2,ah);
	setLocalVar(3,al);
}

void dstore_3(){
	unsigned int ah, al;
	double a = popDbl();
	ah=getHigh(a);
	al=getLow(a);
	setLocalVar(3,ah);
	setLocalVar(4,al);
}

void fadd(){
	float f1, f2;
	f1=toFloat(pop());
	f2=toFloat(pop());
	f1+=f2;
	push(getBytes(f1),'F');
}

void fcmpg (){
	float f1, f2;
	f2=toFloat(pop());
	f1=toFloat(pop());
	if(isNaNF(f1)||isNaNF(f2)){
		push(1,'T');
	} else{
		if(f1>f2)
			push(1,'T');
		else if (f1==f2)
			push(0,'T');
		else
			push(-1,'T');
	}
}


void fcmpl (){
	float f1, f2;
	f2=toFloat(pop());
	f1=toFloat(pop());
	if(isNaNF(f1)||isNaNF(f2)){
		push(-1,'T');
	} else{
		if(f1>f2)
			push(1,'T');
		else if (f1==f2)
			push(0,'T');
		else
			push(-1,'T');
	}
}

void fconst_0 (){
	push(0.0F,'F');
}

void fconst_1 (){
	push(getBytes(1.0),'F');
}

void fconst_2 (){
	push(getBytes(2.0),'F');
}

void fdiv(){
	float f1, f2;
	f2=toFloat(pop());
	f1=toFloat(pop());
	f1/=f2;
	push(getBytes(f1),'F');
}

void fload (){
	unsigned int index = getByte();
	unsigned int a = getLocalVar(index);
	push(a,'F');
}

void fload_0 (){
	unsigned int a = getLocalVar(0);
	push(a,'F');
}

void fload_1 (){
	unsigned int a = getLocalVar(1);
	push(a,'F');
}

void fload_2 (){
	unsigned int a = getLocalVar(2);
	push(a,'F');
}

void fload_3 (){
	unsigned int a = getLocalVar(3);
	push(a,'F');
}

void fmul(){
	float f1, f2;
	f2=toFloat(pop());
	f1=toFloat(pop());
	f1*=f2;
	push(getBytes(f1),'F');
}

void fneg(){
	float f1;
	f1=toFloat(pop());
	push(getBytes(0-f1),'F');
}

void frem(){
	float a, b, r;
	int q;
	b=toFloat(pop());
	a=toFloat(pop());
	if(isNaN(a)||isNaN(b)||b==0.0){
		push(getBytes(a),'F');
	} else if(a==0){
		push(getBytes(0.0),'F');
	}else{
		q=(int)a/b;
		r=a-b*q;
		push(getBytes(r),'F');
	}

}

void fsub(){
	float f1, f2;
	f2=toFloat(pop());
	f1=toFloat(pop());
	f1-=f2;
	push(getBytes(f1),'F');
}

void fstore (){
	astore();
	setTipo('F');
}

void fstore_0 (){
	astore_0();
	setTipo('F');
}

void fstore_1 (){
	astore_1();
	setTipo('F');
}

void fstore_2 (){
	astore_2();
	setTipo('F');
}

void fstore_3 (){
	astore_3();
	setTipo('F');
}

void arraylength(){
	struct Array *a;
	a = popArray();
	push(a->length,'I');
}

void caload(){
	unsigned int i=pop();
	struct Array *a;
	a = popArray();
	if (a->arrayref == NULL){
		printf("NullPointerException\n");
		exit(EXIT_FAILURE);
	}
	if (i>=a->length){

		printf("ArrayVarOutOfBoundsException\n");
		exit(EXIT_FAILURE);
	}
	push(a->arrayref[i],'A');

}

void castore(){
	char c=pop();
	unsigned int i=pop();
	struct Array *a;
	a = popArray();
	if (a->arrayref == NULL){
		printf("NullPointerException\n");
		exit(EXIT_FAILURE);
	}
	if (i>=a->length){
		printf("ArrayVarOutOfBoundsException\n");
		exit(EXIT_FAILURE);
	}
	a->arrayref[i] = c;

}

void newarray(){
	int type = getSignedByte();
	if(type<0){
		erroFatal("NegativeArraySizeException");
	}
	struct Array *a;
	unsigned int i = pop();
	a = createNewArray(i,type);
	pushArray(a);
}
/*
void multianewarray(){
	int type, n;
	type = getShort();
	n=getByte();
	struct Array *a;
	int i=0;
	int temp=1;
	while (i<n){
		temp = temp * pop();
		i++;
	}
	a = createNewArray(temp,type);
	pushArray(a);
}*/

void iaload(){
	unsigned int i=pop();
	struct Array *a;
	a = popArray();
	if (a->arrayref == NULL){
		printf("NullPointerException\n");
		exit(EXIT_FAILURE);
	}
	if (i>=a->length){

		printf("ArrayVarOutOfBoundsException\n");
		exit(EXIT_FAILURE);
	}
	push(a->arrayref[i],'A');

}

void iastore(){
	unsigned int c=pop();
	unsigned int i=pop();
	struct Array *a;
	a = popArray();
	if (a->arrayref == NULL){
		printf("NullPointerException\n");
		exit(EXIT_FAILURE);
	}
	if (i>=a->length){
		printf("ArrayVarOutOfBoundsException\n");
		exit(EXIT_FAILURE);
	}
	a->arrayref[i] = c;

}

void faload(){
	unsigned int i=pop();
	struct Array *a;
	a = popArray();
	if (a->arrayref == NULL){
		printf("NullPointerException\n");
		exit(EXIT_FAILURE);
	}
	if (i>=a->length){

		printf("ArrayVarOutOfBoundsException\n");
		exit(EXIT_FAILURE);
	}
	push(a->arrayref[i],'A');

}

void fastore(){
	unsigned int c=pop();
	unsigned int i=pop();
	struct Array *a;
	a = popArray();
	if (a->arrayref == NULL){
		printf("NullPointerException\n");
		exit(EXIT_FAILURE);
	}
	if (i>=a->length){
		printf("ArrayVarOutOfBoundsException\n");
		exit(EXIT_FAILURE);
	}
	a->arrayref[i] = c;

}


void daload(){
	unsigned int i=pop();
	struct Array *a;
	a = popArray();
	if (a->arrayref == NULL){
		printf("NullPointerException\n");
		exit(EXIT_FAILURE);
	}
	if (i>=a->length){

		printf("ArrayVarOutOfBoundsException\n");
		exit(EXIT_FAILURE);
	}
	pushDbl(((double *)a->arrayref)[i]);

}

void dastore(){
	double d=popDbl();
	printf("%lf\n", d);
	unsigned int i=pop();
	struct Array *a;
	a = popArray();
	if (a->arrayref == NULL){
		printf("NullPointerException\n");
		exit(EXIT_FAILURE);
	}
	if (i>=a->length){
		printf("ArrayVarOutOfBoundsException\n");
		exit(EXIT_FAILURE);
	}
	((double *)a->arrayref)[i] = d;

}


void i2l(){
	unsigned int i = pop();
	pushLong((long long)i);
}

void i2f(){
	unsigned int i = pop();
	float f = i+0.0;
	push(getBytes(f),'F');
}

void i2d(){
	unsigned int i = pop();
	double d = i+0.0;
	pushDbl(d);
}

void l2i(){
	long long l = popLong();
	push((unsigned int)l,'I');
}

void l2f(){
	long long l = popLong();
	float f = l+0.0;
	push(getBytes(f),'F');
}

void l2d(){
	long long l = popLong();
	double d = l+0.0;
	pushDbl(d);
}

void f2i(){
	float f = toFloat(pop());
	int i = (int) f;
	push(i,'I');
}

void f2l(){
	float f = toFloat(pop());
	long long l = (long long) f;
	pushLong(l);
}

void f2d(){
	float f = toFloat(pop());
	double d = f + 0.0;
	pushDbl(d);
}

void d2i(){
	double d = popDbl();
	push((int)d,'I');
}

void d2l(){
	double d = popDbl();
	pushLong((long long)d);
}

void d2f(){
	double d = popDbl();
	push(getBytes(d+0.0),'F');
}

void i2b(){
	char c = pop();
	push((unsigned int)c,'I');
}

void i2c(){
	i2b();
}

void i2s(){
	short s = pop();
	push((unsigned int)s,'s');
}

void iadd(){
	int a = pop();
	int b = pop();
	push(a+b,'I');
}

void isub(){
	int b = pop();
	int a = pop();
	push(a-b,'I');
}

void iand(){
	int a = pop();
	int b = pop();
	push(a&b,'I');
}

void ixor(){
	int a = pop();
	int b = pop();
	int x,y;
	x = a&b;
	y = ~a & ~b;
	push(~y & ~x,'I');
}

void ior(){
	int a = pop();
	int b = pop();
	push(a|b,'I');
}

void iconst_m1 (){
	push(-1,'I');
}

void iconst_0 (){
	push(0,'I');
}

void iconst_1 (){
	push(1,'I');
}

void iconst_2 (){
	push(2,'I');
}

void iconst_3 (){
	push(3,'I');
}

void iconst_4 (){
	push(4,'I');
}

void iconst_5 (){
	push(5,'I');
}

void idiv(){
	int b = pop();
	int a = pop();
	push(a/b,'I');
}

void imul(){
	int a = pop();
	int b = pop();
	push(a*b,'I');
}

void ineg(){
	int a = pop();
	push(0-a,'I');
}

void iinc(){
	unsigned int index = getByte();
	int cte = getSignedByte();
	int v = getLocalVar(index);
	v+=cte;
	setLocalVar(index,v);
	//pcInc=3;
}

void iload(){
	aload();
	setTipo('I');
}

void iload_0(){
	aload_0();
	setTipo('I');
}

void iload_1(){
	aload_1();
	setTipo('I');
}

void iload_2(){
	aload_2();
	setTipo('I');
}

void iload_3(){
	aload_3();
	setTipo('I');
}

void irem(){
	int b = pop();
	int a = pop();
	push(a%b,'I');
}

void ishl(){
	int b = pop();
	int a = pop();
	b&=0x1F;
	push(a<<b,'I');
}

void ishr(){
	int b = pop();
	int a = pop();
	b&=0x1F;
	push(a>>b,'I');
}

void iushr(){
	unsigned int b = pop();
	unsigned int a = pop();
	b&=0x1F;
	push(a>>b,'I');
}

void istore(){
	astore();
}

void istore_0(){
	astore_0();
}
void istore_1(){
	astore_1();
}
void istore_2(){
	astore_2();
}
void istore_3(){
	astore_3();
}

void ladd(){
	long long a,b;
	b=popLong();
	a=popLong();
	pushLong(a+b);
}

void lsub(){
	long long a,b;
	b=popLong();
	a=popLong();
	pushLong(a-b);
}

void land(){
	long long a,b;
	b=popLong();
	a=popLong();
	pushLong(a&b);
}


void lcmp(){
	long long a,b;
	b=popLong();
	a=popLong();
	if(a>b)
		push(1,'T');
	else if(a==b)
		push(0,'T');
	else
		push(-1,'T');
}

void lconst_0 (){
	pushLong(0L);
}

void lconst_1 (){
	pushLong(1L);
}

void ldiv_(){
	long long a,b;
	b=popLong();
	a=popLong();
	pushLong(a/b);
}

void lload (){
	unsigned int index = getByte();
	unsigned int ah, al;
	ah=getLocalVar(index);
	al=getLocalVar((index)+1);
	long long l = toLong(ah,al);
	pushLong(l);
}

void lload_0 (){
	unsigned int ah, al;
	ah=getLocalVar(0);
	al=getLocalVar((0)+1);
	long long l = toLong(ah,al);
	pushLong(l);
}

void lload_1 (){
	unsigned int ah, al;
	ah=getLocalVar(1);
	al=getLocalVar((1)+1);
	long long l = toLong(ah,al);
	pushLong(l);
}

void lload_2 (){
	unsigned int ah, al;
	ah=getLocalVar(2);
	al=getLocalVar((2)+1);
	long long l = toLong(ah,al);
	pushLong(l);
}

void lload_3 (){
	unsigned int ah, al;
	ah=getLocalVar(3);
	al=getLocalVar((3)+1);
	long long l = toLong(ah,al);
	pushLong(l);
}

void lstore(){
	unsigned int index = getByte();
	long long a = popLong();
	unsigned int ah, al;
	ah=getLHigh(a);
	al=getLlow(a);
	setLocalVar(index,ah);
	setLocalVar((index)+1,al);
}

void lstore_0(){
	long long a = popLong();
	unsigned int ah, al;
	ah=getLHigh(a);
	al=getLlow(a);
	setLocalVar(0,ah);
	setLocalVar((0)+1,al);
}

void lstore_1(){
	long long a = popLong();
	unsigned int ah, al;
	ah=getLHigh(a);
	al=getLlow(a);
	setLocalVar(1,ah);
	setLocalVar((1)+1,al);
}

void lstore_2(){
	long long a = popLong();
	unsigned int ah, al;
	ah=getLHigh(a);
	al=getLlow(a);
	setLocalVar(2,ah);
	setLocalVar((2)+1,al);
}

void lstore_3(){
	long long a = popLong();
	unsigned int ah, al;
	ah=getLHigh(a);
	al=getLlow(a);
	setLocalVar(3,ah);
	setLocalVar((3)+1,al);
}


void lmul(){
	long long a, b;
	b=popLong();
	a=popLong();
	pushLong(a*b);
}

void lneg(){
	long long b;
	b=popLong();
	pushLong(0LL-b);
}

void lor(){
	long long a, b;
	b=popLong();
	a=popLong();
	pushLong(a|b);
}

void lrem(){
	long long a, b;
	b=popLong();
	a=popLong();
	pushLong(a%b);
}

void lshl(){
	long long a;
	int b;
	b=pop();
	a=popLong();
	pushLong(a<<b);
}

void lshr(){
	long long a;
	int b;
	b=pop();
	a=popLong();
	pushLong(a>>b);
}

void lushr(){
	unsigned long long a;
	unsigned int b;
	b=pop();
	a=popLong();
	pushLong(a>>b);
}

void lxor(){
	long long a, b;
	unsigned int ah, al, bh, bl, xh, xl;
	b=popLong();
	a=popLong();
	ah=getLHigh(a);
	al=getLlow(a);
	bh=getLHigh(b);
	bl=getLlow(b);
	push(ah,0);
	push(bh,0);
	ixor();
	xh=pop();
	push(al,0);
	push(bl,0);
	ixor();
	xl=pop();
	push(xh,0);
	push(xl,'J');
}

void nop(){

}

void pop_(){
	pop();
}

void pop2(){
	popDbl();
}

void sipush(){
	int a = getSignedShort();
	push(a,'s');
}


void putstatic(){
	unsigned int index = getShort();
	struct item * a = mainClass->cpool[index-1];
	struct class *c;
	struct field *f;
	if(a->tag==9){ //field
		c = getClass(a->class);
		f = getField(c,a->name_and_type->name);
		if((f->aflags & 0x8)!=0 ){
			f->value_l = pop();
			if(strcmp(f->descriptor,"D")==0 || strcmp(f->descriptor,"J")==0 ){ //double ou long?
				f->value_h = pop();
			}
		}
	}
	//TODO invalid index?
}

void getstatic(){
	unsigned int index = getShort();
	struct item * a = mainClass->cpool[index-1];
	struct class *c;
	struct field *f;
	if(a->tag==9){ //field
		if(strcmp(a->name_and_type->name,"out")==0 &&  (strcmp(a->class,"java/lang/System")==0 || strcmp(a->class,"java\\lang\\System")==0))
			return; // nao ir pegar se for pra impressao
		c = getClass(a->class);
		f = getField(c,a->name_and_type->name);
		if((f->aflags & 0x8)!=0 ){ //testa se eh estatica
			if(strcmp(f->descriptor,"D")==0 || strcmp(f->descriptor,"J")==0 ){ //double ou long?
				push(f->value_h,0);
			}
			push(f->value_l,f->descriptor[0]);

		}
	}
	//TODO invalid index?
}

void ldc(){
/*
 * The index is an unsigned byte that must be a valid index into the run-time constant pool of the
 * current class (§2.6). The run-time constant pool entry at index either must be a run-time constant
 * of type int or float, or a reference to a string literal, or a symbolic reference to a class,
 * method type, or method handle (§5.1).
If the run-time constant pool entry is a run-time constant of type int or float, the numeric value
of that run-time constant is pushed onto the operand stack as an int or float, respectively.
Otherwise, if the run-time constant pool entry is a reference to an instance of class String
representing a string literal (§5.1), then a reference to that instance, value, is pushed onto
the operand stack.
Otherwise, if the run-time constant pool entry is a symbolic reference to a class (§5.1), then the
named class is resolved (§5.4.3.1) and a reference to the Class object representing that class, value,
is pushed onto the operand stack.
Otherwise, the run-time constant pool entry must be a symbolic reference to a method type or a
method handle (§5.1). The method type or method handle is resolved (§5.4.3.5) and a reference to
the resulting instance of java.lang.invoke.MethodType or java.lang.invoke.MethodHandle, value,
is pushed onto the operand stack.
 *
 */	unsigned int index = getByte();
	struct item *a;
	struct class *c;
	a=current->cpool[index-1];
	//TODO checar com tamanho (ldc_w, ldc2_w)
	switch(a->tag){
		case 3: //int
			push(a->bytes,'I');
			break;
		case 4: //float
			push(a->bytes,'F');
			break;
		case 8: //string
			push((unsigned int)a->string,'S');
			break;
		case 7: //class
			c=getClass(a->name);
			push((unsigned int)c,'C');
			break;
	} //TODO method handles ?
}

void ldc_w(){
	struct item *a;
	struct class *c;
	unsigned int index = getShort();

	a=current->cpool[index-1];

	switch(a->tag){
		case 3: //int
		case 4: //float
			push(a->bytes,'F');
			break;
		case 8: //string
			push((unsigned int)a->string,'S');
			break;
		case 7: //class
			c=getClass(a->name);
			push((unsigned int)c,'C');
			break;
	} //TODO method handles ?
}

void ldc2_w(){
	/*
	 * The unsigned indexbyte1 and indexbyte2 are assembled into an unsigned 16-bit index into
	 * the run-time constant pool of the current class (§2.6), where the value of the index is
	 * calculated as (indexbyte1 << 8) | indexbyte2. The index must be a valid index into the
	 * run-time constant pool of the current class. The run-time constant pool entry at the
	 * index must be a run-time constant of type long or double (§5.1). The numeric value of
	 * that run-time constant is pushed onto the operand stack as a long or double, respectively.
	 */
	struct item *a;

	unsigned int index = getShort();

	a=current->cpool[index-1];

	switch(a->tag){
		case 5: //long
			push(a->high_bytes,0);
			push(a->low_bytes,'J');
			break;
		case 6: //double
			push(a->high_bytes,0);
			push(a->low_bytes,'D');
			break;
	}
}

void goto_(){
	int offset = getSignedShort();
	pcInc = offset;
}

void goto_w(){
	int offset = getSignedWord();
	pcInc = offset;
}

void if_acmpeq (){
	unsigned int a,b;
	b=pop();
	a=pop();
	if(a==b)
		goto_();
}

void if_acmpne (){
	unsigned int a,b;
	b=pop();
	a=pop();
	if(a!=b)
		goto_();
}

void if_icmpeq (){
	if_acmpeq();
}

void if_icmpne (){
	if_acmpne();
}

void if_icmplt (){
	unsigned int a,b;
	b=pop();
	a=pop();
	if(a<b)
		goto_();
}

void if_icmple (){
	unsigned int a,b;
	b=pop();
	a=pop();
	if(a<=b)
		goto_();
}

void if_icmpgt (){
	unsigned int a,b;
	b=pop();
	a=pop();
	if(a>b)
		goto_();
}

void if_icmpge (){
	unsigned int a,b;
	b=pop();
	a=pop();
	if(a>=b)
		goto_();
}

void ifeq (){
	int a = pop();
	if(a==0)
		goto_();
}

void ifne (){
	int a = pop();
	if(a!=0)
		goto_();
}

void iflt (){
	int a = pop();
	if(a<0)
		goto_();
}

void ifgt (){
	int a = pop();
	if(a>0)
		goto_();
}

void ifle (){
	int a = pop();
	if(a<=0)
		goto_();
}

void ifge (){
	int a = pop();
	if(a>=0)
		goto_();
}

void ifnonnull (){
	ifne();
}

void ifnull (){
	ifeq();
}

void baload(){
	unsigned int i=pop();
	struct Array *a;
	a = popArray();
	if (a->arrayref == NULL){
		printf("NullPointerException\n");
		exit(EXIT_FAILURE);
	}
	if (i>=a->length){

		printf("ArrayVarOutOfBoundsException\n");
		exit(EXIT_FAILURE);
	}
	push(a->arrayref[i],'A');

}

void bastore(){
	unsigned int c=pop();
	unsigned int i=pop();
	struct Array *a;
	a = popArray();
	if (a->arrayref == NULL){
		printf("NullPointerException\n");
		exit(EXIT_FAILURE);
	}
	if (i>=a->length){
		printf("ArrayVarOutOfBoundsException\n");
		exit(EXIT_FAILURE);
	}
	a->arrayref[i] = c;

}

void saload(){
	unsigned int i=pop();
	struct Array *a;
	a = popArray();
	if (a->arrayref == NULL){
		printf("NullPointerException\n");
		exit(EXIT_FAILURE);
	}
	if (i>=a->length){

		printf("ArrayVarOutOfBoundsException\n");
		exit(EXIT_FAILURE);
	}
	push(((short *)a->arrayref)[i],'A');

}

void sastore(){
	unsigned short c=pop();
	unsigned int i=pop();
	struct Array *a;
	a = popArray();
	if (a->arrayref == NULL){
		printf("NullPointerException\n");
		exit(EXIT_FAILURE);
	}
	if (i>=a->length){
		printf("ArrayVarOutOfBoundsException\n");
		exit(EXIT_FAILURE);
	}
	((short *)a->arrayref)[i] = c;

}

void laload(){
	unsigned int i=pop();
	struct Array *a;
	a = popArray();
	if (a->arrayref == NULL){
		printf("NullPointerException\n");
		exit(EXIT_FAILURE);
	}
	if (i>=a->length){

		printf("ArrayVarOutOfBoundsException\n");
		exit(EXIT_FAILURE);
	}
	pushLong(((long long *)a->arrayref)[i]);


}

void lastore(){
	long long l=popLong();
	unsigned int i =pop();
	struct Array *a;
	a = popArray();
	if (a->arrayref == NULL){
		printf("NullPointerException\n");
		exit(EXIT_FAILURE);
	}
	if (i>=a->length){
		printf("ArrayVarOutOfBoundsException\n");
		exit(EXIT_FAILURE);
	}
	((long long*) a->arrayref)[i] = l;


}

void invokespecial(){

	unsigned int index = getShort();
	struct method * m;
	char * className;

	struct Object * obj =(struct Object *) pop();

	m = getMethodByCPIndex(mainClass,index);
	if(m->code->code_l<2)
		return;
	className=getMethodClassName(mainClass,index);

	push((unsigned int)curObj,0);
	push(pc,0);
	push((unsigned int)current,0);
	//printf("curobj=0x%X\n",(unsigned int)curObj);
	//printf("pc=%d\n",pc);
	//printf("curennt=0x%X\n",(unsigned int)current);
	current = getClass(className);
	curObj = obj;
	executaMetodo(m);

	pcInc=2;
}

void invokestatic(){
	//TODO args??
	unsigned int index = getShort();
	struct method * m;
	char * className;
	m = getMethodByCPIndex(current,index);
	if(m->code->code_l<2)
		return;
	className=getMethodClassName(current,index);
	//push((unsigned int)curObj,0);
	//push((unsigned int)pc,0);
	//push((unsigned int)current,0);
	current = getClass(className);
	executaMetodo(m);
	pcInc=2;
}

void return_(){

	dropFrame();
	current=(struct class *)pop();
	//printf("curennt=0x%X\n",(unsigned int)current);
	pc=pop();
	//printf("pc=%d\n",pc);
	if(estatico==0)
		curObj = (struct Object *)pop();
	//printf("curobj=0x%X\n",(unsigned int)curObj);
	pcInc=1;
}

void ireturn(){
	unsigned int a = pop();
	return_();
	push(a,'I');
}
void lreturn(){
	long long l = popLong();
	return_();
	pushLong(l);
}
void freturn(){
	ireturn();
	setTipo('F');
}
void dreturn(){
	lreturn();
	setTipo('D');
}
void areturn(){
	ireturn();
	setTipo('A');
}

void getfield(){
	unsigned int index = getShort();
	unsigned int value;
	double dv;
	long long lv;
	struct Object * obj;
	obj = (struct Object *) pop();
	char * descriptor = current->cpool[index-1]->name_and_type->descriptor;
	char * name = obj->instance->cpool[index-1]->name_and_type->name;
	if(strcmp(descriptor,"D")==0){
		dv = getDoubleFieldValue(obj,name);
		pushDbl(dv);
	}else if ( strcmp(descriptor,"J")==0){
		lv = getLongFieldValue(obj,name);
		pushLong(lv);
	}
	else{
		value = getFieldValue(obj,name);
		push(value,descriptor[0]);
	}

}


void putfield(){
	unsigned int index = getShort();
	unsigned int value = pop();
	unsigned int vh;
	struct Object * obj;
	//char * descriptor = curObj->instance->cpool[index-1]->name_and_type->descriptor;
	char * descriptor = current->cpool[index-1]->name_and_type->descriptor;
	if(strcmp(descriptor,"D")==0 || strcmp(descriptor,"J")==0 ){ // double ou long
		vh = pop();
		obj = (struct Object *) pop();
		char * name = current->cpool[index-1]->name_and_type->name;
		setDoubleFieldValue(obj,name,vh,value);
	} else {
		obj = (struct Object *) pop();
		char * name = current->cpool[index-1]->name_and_type->name;
		setFieldValue(obj,name,value);
	}
}

void invokevirtual(){
	//TODO args??
	unsigned int index = getShort();
	unsigned int a;
	struct method * m;
	char * className, *mName, tipo, *desc;
	char * str;
	struct item * i = mainClass->cpool[index-1];
	//className=getMethodClassName(current,index);
	//mName=getMethodNameByCPIndex(current,index);
	className = i->class;
	mName = i->name_and_type->name;
	desc = i->name_and_type->descriptor;
	if(strcmp(className,"java/lang/StringBuilder")==0 || strcmp(className,"java\\lang\\StringBuilder")==0){

	}
	else if(strcmp(className,"java/io/PrintStream")==0 || strcmp(className,"java\\io\\PrintStream")==0){
		tipo = getTipo();
		switch (tipo){
			case 'S':
				str = (char *) pop();
				printf("%s",str);
				break;
			case 'I':
				a = pop();
				printf("%d",a);
				break;
			case 'F':
				a = pop();
				printf("%f",toFloat(a));
				break;
			case 'D':
				printf("%f",popDbl());
				break;
			case 'J':
				printf("%lld",popLong());
				break;
			case 'B':
				a = pop();
				if(a==0)
					printf("false");
				else
					printf("true");
				break;
		}
		if(strcmp(mName,"println")==0)
			printf("\n");
	} else{
		m = getMethod(getClass(className),mName,desc);
		if(m->code->code_l<2)
			return;
		//push((unsigned int)curObj,0);
		push(pc,0);
		push((unsigned int)current,0);
		current = getClass(className);
		executaMetodo(m); //TODO arrumar!!
		pcInc=2;
	}
}

void jsr(){
	int offset = getSignedShort();
	push(pc+3,0);
	pcInc = offset;
}
void jsr_w(){
	int offset = getSignedWord();
	push(pc+5,0);
	pcInc = offset;
}

void ret(){
	unsigned int retAdress = getByte();
	pc = retAdress;
}

void lookupswitch(){
	int pad = (pc+1)%4;
	if(pad>0)
		pad = 4 - pad;
	pcInc+=pad;
	int def = getSignedWord();
	int n = getSignedWord();
	int i, match, offset;
	int key = pop();
	for(i=0;i<n;i++){
		match = getSignedWord();
		offset = getSignedWord();
		if(key==match){
			pcInc = offset;
			return;
		}
	}
	pcInc = def;
}

void tableswitch(){
	int pad = (pc+1)%4;
	if(pad>0)
		pad = 4 - pad;
	pcInc+=pad;
	int def = getSignedWord();
	int low = getSignedWord();
	int high = getSignedWord();
	int i, offset, tboff;
	int index = pop();

	if(index < low || index > high){
		pcInc = def;
	} else{
		tboff = index - low;
		pcInc += tboff*4;
		offset = getSignedWord();
		pcInc = offset;
	}




}


void invokeinterface(){
	//TODO args??
	unsigned int index = getShort();
	unsigned int b = getShort();
	unsigned int a;
	struct method * m;
	char * className, *mName, tipo, *desc;
	char * str;
	struct item * i = mainClass->cpool[index-1];
	//className=getMethodClassName(current,index);
	//mName=getMethodNameByCPIndex(current,index);
	className = i->class;
	mName = i->name_and_type->name;
	desc = i->name_and_type->descriptor;
	if(strcmp(className,"java/lang/StringBuilder")==0 || strcmp(className,"java\\lang\\StringBuilder")==0){

	}
	else if(strcmp(className,"java/io/PrintStream")==0 || strcmp(className,"java\\io\\PrintStream")==0){
		tipo = getTipo();
		switch (tipo){
			case 'S':
				str = (char *) pop();
				printf("%s",str);
				break;
			case 'I':
				a = pop();
				printf("%d",a);
				break;
			case 'F':
				a = pop();
				printf("%f",toFloat(a));
				break;
			case 'D':
				printf("%f",popDbl());
				break;
			case 'J':
				printf("%lld",popLong());
				break;
			case 'B':
				a = pop();
				if(a==0)
					printf("false");
				else
					printf("true");
				break;
		}
		if(strcmp(mName,"println")==0)
			printf("\n");
	} else{
		m = getMethod(getClass(className),mName,desc);
		if(m->code->code_l<2)
			return;
		//push((unsigned int)curObj,0);
		push(pc,0);
		push((unsigned int)current,0);
		current = getClass(className);
		executaMetodo(m); //TODO arrumar!!
		pcInc=2;
	}


}


void invokedynamic(){
	erroFatal("Instrucao nao implementada: invokedynamic");
}
void athrow(){
	erroFatal("Instrucao nao implementada: athrow");
}
void checkcast(){
	erroFatal("Instrucao nao implementada: checkcast");
}
void instanceof(){
	erroFatal("Instrucao nao implementada: instanceof");
}
void monitorenter(){
	erroFatal("Instrucao nao implementada: monitorenter");
}
void monitorexit(){
	erroFatal("Instrucao nao implementada: monitorexit");
}
void wide(){
	erroFatal("Instrucao nao implementada: wide");
}

void nope(int opcode){
	printf("opcode 0x%X :",opcode);
	erroFatal("Instrucao nao implementada");
}



/*
int main(){
	int i;
	double a, b;
	unsigned int r, ah, al;
	float f1,f2;
	newFrame(5);
	long long lo;
	pushLong(4123LL);
	pushLong(0x10000000FLL);

	sipush(2,4);

	printf("%d\n",pop());
	//printf("%lld\n",popLong());
	//printf("%d\n",(pop()));
	return 0;
}
*/

void chamaInst(int op, struct code * c){
	cptr=c;
	switch(op){
		case 0 : nop (); break;
		case 1 : aconst_null (); break;
		case 2 : iconst_m1 (); break;
		case 3 : iconst_0 (); break;
		case 4 : iconst_1 (); break;
		case 5 : iconst_2 (); break;
		case 6 : iconst_3 (); break;
		case 7 : iconst_4 (); break;
		case 8 : iconst_5 (); break;
		case 9 : lconst_0 (); break;
		case 10 : lconst_1 (); break;
		case 11 : fconst_0 (); break;
		case 12 : fconst_1 (); break;
		case 13 : fconst_2 (); break;
		case 14 : dconst_0 (); break;
		case 15 : dconst_1 (); break;
		case 16 : bipush (); break;
		case 17 : sipush (); break;
		case 18 : ldc (); break;
		case 19 : ldc_w (); break;
		case 20 : ldc2_w (); break;
		case 21 : iload (); break;
		case 22 : lload (); break;
		case 23 : fload (); break;
		case 24 : dload (); break;
		case 25 : aload (); break;
		case 26 : iload_0 (); break;
		case 27 : iload_1 (); break;
		case 28 : iload_2 (); break;
		case 29 : iload_3 (); break;
		case 30 : lload_0 (); break;
		case 31 : lload_1 (); break;
		case 32 : lload_2 (); break;
		case 33 : lload_3 (); break;
		case 34 : fload_0 (); break;
		case 35 : fload_1 (); break;
		case 36 : fload_2 (); break;
		case 37 : fload_3 (); break;
		case 38 : dload_0 (); break;
		case 39 : dload_1 (); break;
		case 40 : dload_2 (); break;
		case 41 : dload_3 (); break;
		case 42 : aload_0 (); break;
		case 43 : aload_1 (); break;
		case 44 : aload_2 (); break;
		case 45 : aload_3 (); break;
		case 46 : iaload (); break;
		case 47 : laload (); break;
		case 48 : faload (); break;
		case 49 : daload (); break;
		case 50 : aaload (); break;
		case 51 : baload (); break;
		case 52 : caload (); break;
		case 53 : saload (); break;
		case 54 : istore (); break;
		case 55 : lstore (); break;
		case 56 : fstore (); break;
		case 57 : dstore (); break;
		case 58 : astore (); break;
		case 59 : istore_0 (); break;
		case 60 : istore_1 (); break;
		case 61 : istore_2 (); break;
		case 62 : istore_3 (); break;
		case 63 : lstore_0 (); break;
		case 64 : lstore_1 (); break;
		case 65 : lstore_2 (); break;
		case 66 : lstore_3 (); break;
		case 67 : fstore_0 (); break;
		case 68 : fstore_1 (); break;
		case 69 : fstore_2 (); break;
		case 70 : fstore_3 (); break;
		case 71 : dstore_0 (); break;
		case 72 : dstore_1 (); break;
		case 73 : dstore_2 (); break;
		case 74 : dstore_3 (); break;
		case 75 : astore_0 (); break;
		case 76 : astore_1 (); break;
		case 77 : astore_2 (); break;
		case 78 : astore_3 (); break;
		case 79 : iastore (); break;
		case 80 : lastore (); break;
		case 81 : fastore (); break;
		case 82 : dastore (); break;
		case 83 : aastore (); break;
		case 84 : bastore (); break;
		case 85 : castore (); break;
		case 86 : sastore (); break;
		case 87 : pop (); break;
		case 88 : pop2 (); break;
		case 89 : dup (); break;
		case 90 : dup_x1 (); break;
		case 91 : dup_x2 (); break;
		case 92 : dup2 (); break;
		case 93 : dup2_x1 (); break;
		case 94 : dup2_x2 (); break;
		case 95 : swap (); break;
		case 96 : iadd (); break;
		case 97 : ladd (); break;
		case 98 : fadd (); break;
		case 99 : dadd (); break;
		case 100 : isub (); break;
		case 101 : lsub (); break;
		case 102 : fsub (); break;
		case 103 : dsub (); break;
		case 104 : imul (); break;
		case 105 : lmul (); break;
		case 106 : fmul (); break;
		case 107 : dmul (); break;
		case 108 : idiv (); break;
		case 109 : ldiv_ (); break;
		case 110 : fdiv (); break;
		case 111 : ddiv (); break;
		case 112 : irem (); break;
		case 113 : lrem (); break;
		case 114 : frem (); break;
		case 115 : drem_ (); break;
		case 116 : ineg (); break;
		case 117 : lneg (); break;
		case 118 : fneg (); break;
		case 119 : dneg (); break;
		case 120 : ishl (); break;
		case 121 : lshl (); break;
		case 122 : ishr (); break;
		case 123 : lshr (); break;
		case 124 : iushr (); break;
		case 125 : lushr (); break;
		case 126 : iand (); break;
		case 127 : land (); break;
		case 128 : ior (); break;
		case 129 : lor (); break;
		case 130 : ixor (); break;
		case 131 : lxor (); break;
		case 132 : iinc (); break;
		case 133 : i2l (); break;
		case 134 : i2f (); break;
		case 135 : i2d (); break;
		case 136 : l2i (); break;
		case 137 : l2f (); break;
		case 138 : l2d (); break;
		case 139 : f2i (); break;
		case 140 : f2l (); break;
		case 141 : f2d (); break;
		case 142 : d2i (); break;
		case 143 : d2l (); break;
		case 144 : d2f (); break;
		case 145 : i2b (); break;
		case 146 : i2c (); break;
		case 147 : i2s (); break;
		case 148 : lcmp (); break;
		case 149 : fcmpl (); break;
		case 150 : fcmpg (); break;
		case 151 : dcmpl (); break;
		case 152 : dcmpg (); break;
		case 153 : ifeq (); break;
		case 154 : ifne (); break;
		case 155 : iflt (); break;
		case 156 : ifge (); break;
		case 157 : ifgt (); break;
		case 158 : ifle (); break;
		case 159 : if_icmpeq (); break;
		case 160 : if_icmpne (); break;
		case 161 : if_icmplt (); break;
		case 162 : if_icmpge (); break;
		case 163 : if_icmpgt (); break;
		case 164 : if_icmple (); break;
		case 165 : if_acmpeq (); break;
		case 166 : if_acmpne (); break;
		case 167 : goto_(); break;
		case 168 : jsr (); break;
		case 169 : ret (); break;
		case 170 : tableswitch (); break;
		case 171 : lookupswitch (); break;
		case 172 : ireturn (); break;
		case 173 : lreturn (); break;
		case 174 : freturn (); break;
		case 175 : dreturn (); break;
		case 176 : areturn (); break;
		case 177 : return_(); break;
		case 178 : getstatic (); break;
		case 179 : putstatic (); break;
		case 180 : getfield (); break;
		case 181 : putfield (); break;
		case 182 : invokevirtual (); break;
		case 183 : invokespecial (); break;
		case 184 : invokestatic (); break;
		case 185 : invokeinterface (); break;
		case 186 : invokedynamic (); break;
		case 187 : new (); break;
		case 188 : newarray (); break;
		case 189 : anewarray (); break;
		case 190 : arraylength (); break;
		case 191 : athrow (); break;
		case 192 : checkcast (); break;
		case 193 : instanceof (); break;
		case 194 : monitorenter (); break;
		case 195 : monitorexit (); break;
		case 196 : wide (); break;
		case 197 : multianewarray (); break;
		case 198 : ifnull (); break;
		case 199 : ifnonnull (); break;
		case 200 : goto_w (); break;
		case 201 : jsr_w (); break;
		case 202 : nope(op); break;
		case 203 : nope(op); break;
		case 204 : nope(op); break;
		case 205 : nope(op); break;
		case 206 : nope(op); break;
		case 207 : nope(op); break;
		case 208 : nope(op); break;
		case 209 : nope(op); break;
		case 210 : nope(op); break;
		case 211 : nope(op); break;
		case 212 : nope(op); break;
		case 213 : nope(op); break;
		case 214 : nope(op); break;
		case 215 : nope(op); break;
		case 216 : nope(op); break;
		case 217 : nope(op); break;
		case 218 : nope(op); break;
		case 219 : nope(op); break;
		case 220 : nope(op); break;
		case 221 : nope(op); break;
		case 222 : nope(op); break;
		case 223 : nope(op); break;
		case 224 : nope(op); break;
		case 225 : nope(op); break;
		case 226 : nope(op); break;
		case 227 : nope(op); break;
		case 228 : nope(op); break;
		case 229 : nope(op); break;
		case 230 : nope(op); break;
		case 231 : nope(op); break;
		case 232 : nope(op); break;
		case 233 : nope(op); break;
		case 234 : nope(op); break;
		case 235 : nope(op); break;
		case 236 : nope(op); break;
		case 237 : nope(op); break;
		case 238 : nope(op); break;
		case 239 : nope(op); break;
		case 240 : nope(op); break;
		case 241 : nope(op); break;
		case 242 : nope(op); break;
		case 243 : nope(op); break;
		case 244 : nope(op); break;
		case 245 : nope(op); break;
		case 246 : nope(op); break;
		case 247 : nope(op); break;
		case 248 : nope(op); break;
		case 249 : nope(op); break;
		case 250 : nope(op); break;
		case 251 : nope(op); break;
		case 252 : nope(op); break;
		case 253 : nope(op); break;
		case 254 : nope(op); break;
		case 255 : nope(op); break;
	}
}
