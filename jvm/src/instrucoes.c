#include <stack.h>
#include <util.h>
#include <stdio.h>



void aconst_null (){
	push(0);
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
		push(1);
	}
	else{
		if(a>b)
			push(1);
		else if(a==b)
			push(0);
		else
			push(-1);
	}
}

void dcmpl (){
	double a,b;
	b=popDbl();
	a=popDbl();
	if(isNaN(a)!=0 || isNaN(b)!=0){
		push(-1);
	}
	else{
		if(a>b)
			push(1);
		else if(a==b)
			push(0);
		else
			push(-1);
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

void dneg(){
	double a=popDbl();
	a=0.0-a;
	pushDbl(a);
}

void dup(){
	unsigned int a = pop();
	push(a);
	push(a);
}

void dup_x1(){
	unsigned int a = pop();
	unsigned int b = pop();
	push(a);
	push(b);
	push(a);
}

void dup_x2(){
	unsigned int a = pop();
	unsigned int b = pop();
	unsigned int c = pop();
	push(a);
	push(c);
	push(b);
	push(a);
}

void dup2(){
	double a = popDbl();
	pushDbl(a);
	pushDbl(a);
}

void dup2_x1(){
	double a = popDbl();
	unsigned int ui = pop();
	pushDbl(a);
	push(ui);
	pushDbl(a);
}

void dup2_x2(){
	double a = popDbl();
	double b = popDbl();
	pushDbl(a);
	pushDbl(b);
	pushDbl(a);
}

void aload(unsigned int index){
	unsigned int a = getLocalIndex(index);
	push(a);
}

void aload_0(){
	unsigned int a = getLocalIndex(0);
	push(a);
}

void aload_1(){
	unsigned int a = getLocalIndex(1);
	push(a);
}

void aload_2(){
	unsigned int a = getLocalIndex(2);
	push(a);
}

void aload_3(){
	unsigned int a = getLocalIndex(3);
	push(a);
}

void astore(int index){
	unsigned int a = pop();
	setLocalIndex(index,a);
}

void astore_0(){
	unsigned int a = pop();
	setLocalIndex(0,a);
}

void astore_1(){
	unsigned int a = pop();
	setLocalIndex(1,a);
}

void astore_2(){
	unsigned int a = pop();
	setLocalIndex(2,a);
}

void astore_3(){
	unsigned int a = pop();
	setLocalIndex(3,a);
}

void bipush(unsigned int byte){
	push(byte);
}

void dload(unsigned int index){
	unsigned int ah, al;
	ah=getLocalIndex(index);
	al=getLocalIndex(index+1);
	double a = toDouble(ah,al);
	pushDbl(a);
}

void dload_0(){
	unsigned int ah, al;
	ah=getLocalIndex(0);
	al=getLocalIndex(1);
	double a = toDouble(ah,al);
	pushDbl(a);
}

void dload_1(){
	unsigned int ah, al;
	ah=getLocalIndex(1);
	al=getLocalIndex(2);
	double a = toDouble(ah,al);
	pushDbl(a);
}

void dload_2(){
	unsigned int ah, al;
	ah=getLocalIndex(2);
	al=getLocalIndex(3);
	double a = toDouble(ah,al);
	pushDbl(a);
}

void dload_3(){
	unsigned int ah, al;
	ah=getLocalIndex(3);
	al=getLocalIndex(4);
	double a = toDouble(ah,al);
	pushDbl(a);
}

void dstore(unsigned int index){
	unsigned int ah, al;
	double a = popDbl();
	ah=getHigh(a);
	al=getLow(a);
	setLocalIndex(index,ah);
	setLocalIndex(index+1,al);
}

void dstore_0(){
	unsigned int ah, al;
	double a = popDbl();
	ah=getHigh(a);
	al=getLow(a);
	setLocalIndex(0,ah);
	setLocalIndex(1,al);
}

void dstore_1(){
	unsigned int ah, al;
	double a = popDbl();
	ah=getHigh(a);
	al=getLow(a);
	setLocalIndex(1,ah);
	setLocalIndex(2,al);
}

void dstore_2(){
	unsigned int ah, al;
	double a = popDbl();
	ah=getHigh(a);
	al=getLow(a);
	setLocalIndex(2,ah);
	setLocalIndex(3,al);
}

void dstore_3(){
	unsigned int ah, al;
	double a = popDbl();
	ah=getHigh(a);
	al=getLow(a);
	setLocalIndex(3,ah);
	setLocalIndex(4,al);
}

void fadd(){
	float f1, f2;
	f1=toFloat(pop());
	f2=toFloat(pop());
	f1+=f2;
	push(getBytes(f1));
}

void fcmpg (){
	float f1, f2;
	f2=toFloat(pop());
	f1=toFloat(pop());
	if(isNaNF(f1)||isNaNF(f2)){
		push(1);
	} else{
		if(f1>f2)
			push(1);
		else if (f1==f2)
			push(0);
		else
			push(-1);
	}
}


void fcmpl (){
	float f1, f2;
	f2=toFloat(pop());
	f1=toFloat(pop());
	if(isNaNF(f1)||isNaNF(f2)){
		push(-1);
	} else{
		if(f1>f2)
			push(1);
		else if (f1==f2)
			push(0);
		else
			push(-1);
	}
}

void fconst_0 (){
	push(0);
}

void fconst_1 (){
	push(getBytes(1.0));
}

void fconst_2 (){
	push(getBytes(2.0));
}

void fdiv(){
	float f1, f2;
	f2=toFloat(pop());
	f1=toFloat(pop());
	f1/=f2;
	push(getBytes(f1));
}

void fload (unsigned int index){
	aload(index);
}

void fload_0 (){
	aload_0();
}

void fload_1 (){
	aload_1();
}

void fload_2 (){
	aload_2();
}

void fload_3 (){
	aload_3();
}

void fmul(){
	float f1, f2;
	f2=toFloat(pop());
	f1=toFloat(pop());
	f1*=f2;
	push(getBytes(f1));
}

void fneg(){
	float f1;
	f1=toFloat(pop());
	push(getBytes(0-f1));
}

void fsub(){
	float f1, f2;
	f2=toFloat(pop());
	f1=toFloat(pop());
	f1-=f2;
	push(getBytes(f1));
}

void fstore (unsigned int index){
	astore(index);
}

void fstore_0 (){
	astore_0();
}

void fstore_1 (){
	astore_1();
}

void fstore_2 (){
	astore_2();
}

void fstore_3 (){
	astore_3();
}

int mainT(){
	int i;
	double a, b;
	unsigned int r, ah, al;
	float f1,f2;
	newFrame(5);
	push(getBytes(34.112));
	push(getBytes(923.12332));
	push(getBytes(0.12332));
	push(getBytes(-3.1));

	fstore(0);
	fstore(1);
	fstore(2);
	fstore(3);
	for(i=0;i<4;i++)
		printf("%f\n",toFloat(getLocalIndex(i)));
	return 0;
}


