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

int mainTeste1(){
	double a, b;
	unsigned int r;
	a=45.9;
	b=12.760;
	//pushDbl(a);
	//pushDbl(b);

	push(146);
	push(1789);
	push(1);
	push(456);
	dup2_x2();
	//b=popDbl();
	//printf("%d\n",isNaN(a));
	printf("%d %d %d %d %d %d\n",pop(),pop(),pop(),pop(),pop(),pop());

	return 0;
}


