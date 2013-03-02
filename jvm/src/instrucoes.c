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
/*
 * In the remaining cases, where neither operand is an infinity, a zero, or NaN, the
 * floating-point remainder result from a dividend value1' and a divisor value2' is
 *  defined by the mathematical relation result = value1' - (value2' * q), where q
 *  is an integer that is negative only if value1' / value2' is negative, and
 *  positive only if value1' / value2' is positive, and whose magnitude is as
 *  large as possible without exceeding the magnitude of the true mathematical
 *  quotient of value1' and value2'.
 */
void drem(){
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

void frem(){
	float a, b, r;
	int q;
	b=toFloat(pop());
	a=toFloat(pop());
	if(isNaN(a)||isNaN(b)||b==0.0){
		push(getBytes(a));
	} else if(a==0){
		push(getBytes(0.0));
	}else{
		q=(int)a/b;
		r=a-b*q;
		push(getBytes(r));
	}

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

void i2l(){
	unsigned int i = pop();
	pushLong((long long)i);
}

void i2f(){
	unsigned int i = pop();
	float f = i+0.0;
	push(getBytes(f));
}

void i2d(){
	unsigned int i = pop();
	double d = i+0.0;
	pushDbl(d);
}

void l2i(){
	long long l = popLong();
	push((unsigned int)l);
}

void l2f(){
	long long l = popLong();
	float f = l+0.0;
	push(getBytes(f));
}

void l2d(){
	long long l = popLong();
	double d = l+0.0;
	pushDbl(d);
}

void f2i(){
	float f = toFloat(pop());
	int i = (int) f;
	push(i);
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
	push((int)d);
}

void d2l(){
	double d = popDbl();
	pushLong((long long)d);
}

void d2f(){
	double d = popDbl();
	push(getBytes(d+0.0));
}

void i2b(){
	char c = pop();
	push((unsigned int)c);
}

void i2c(){
	i2b();
}

void i2s(){
	short s = pop();
	push((unsigned int)s);
}

void iadd(){
	int a = pop();
	int b = pop();
	push(a+b);
}

void isub(){
	int b = pop();
	int a = pop();
	push(a-b);
}

void iand(){
	int a = pop();
	int b = pop();
	push(a&b);
}

void ixor(){
	int a = pop();
	int b = pop();
	int x,y;
	x = a&b;
	y = ~a & ~b;
	push(~y & ~x);
}

void ior(){
	int a = pop();
	int b = pop();
	push(a|b);
}

void iconst_m1 (){
	push(-1);
}

void iconst_0 (){
	push(0);
}

void iconst_1 (){
	push(1);
}

void iconst_2 (){
	push(2);
}

void iconst_3 (){
	push(3);
}

void iconst_4 (){
	push(4);
}

void iconst_5 (){
	push(5);
}

void idiv(){
	int b = pop();
	int a = pop();
	push(a/b);
}

void imul(){
	int a = pop();
	int b = pop();
	push(a*b);
}

void ineg(){
	int a = pop();
	push(0-a);
}

void iinc(unsigned int index, int constante){
	int v = getLocalIndex(index);
	v+=constante;
	setLocalIndex(index,v);
}

void iload(unsigned int index){
	aload(index);
}

void iload_0(){
	aload_0();
}

void iload_1(){
	aload_1();
}

void iload_2(){
	aload_2();
}

void iload_3(){
	aload_3();
}

void irem(){
	int b = pop();
	int a = pop();
	push(a%b);
}

void ishl(){
	int b = pop();
	int a = pop();
	b&=0x1F;
	push(a<<b);
}

void ishr(){
	int b = pop();
	int a = pop();
	b&=0x1F;
	push(a>>b);
}

void iushr(){
	unsigned int b = pop();
	unsigned int a = pop();
	b&=0x1F;
	push(a>>b);
}

void istore(unsigned int index){
	astore(index);
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
		push(1);
	else if(a==b)
		push(0);
	else
		push(-1);
}

void lconst_0 (){
	pushLong(0L);
}

void lconst_1 (){
	pushLong(1L);
}

void ldiv(){
	long long a,b;
	b=popLong();
	a=popLong();
	pushLong(a/b);
}

void lload (unsigned int index){
	unsigned int ah, al;
	ah=getLocalIndex(index);
	al=getLocalIndex(index+1);
	long long l = toLong(ah,al);
	pushLong(l);
}

void lload_0 (){
	lload(0);
}

void lload_1 (){
	lload(1);
}

void lload_2 (){
	lload(2);
}

void lload_3 (){
	lload(3);
}

void lstore(unsigned int index){
	long long a = popLong();
	unsigned int ah, al;
	ah=getLHigh(a);
	al=getLlow(a);
	setLocalIndex(index,ah);
	setLocalIndex(index+1,al);
}

void lstore_0(){
	lstore(0);
}

void lstore_1(){
	lstore(1);
}

void lstore_2(){
	lstore(2);
}

void lstore_3(){
	lstore(3);
}


void lmul(){
	long long a, b;
	b=popLong();
	a=popLong();
	pushLong(a*b);
}

void lneg(){
	long long a, b;
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
	push(ah);
	push(bh);
	ixor();
	xh=pop();
	push(al);
	push(bl);
	ixor();
	xl=pop();
	push(xh);
	push(xl);
}

void nop(){

}

void pop_(){
	pop();
}

void pop2(){
	popDbl();
}

void sipush(unsigned int byte1, unsigned int byte2){
	unsigned int a= byte1;
	a<<=8;
	a|=byte2;
	push(a);
}

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



