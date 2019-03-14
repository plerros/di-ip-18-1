#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <limits.h>

//====================================================+ DEFINITIONS +====================================================
#define MINIMUM	3990000000
#define MAXIMUM 4010000000
//		4294967295
#define	ROOF	4759123141
//======================================================+ MODULO  +======================================================
unsigned int modulo(unsigned long p, unsigned int k, unsigned int m){
if(k == 0)	{return 1;				}
else if(k & 1)	{return((p * modulo(p,k-1,m)) % m);	}
else		{return(modulo((p*p) % m, k >> 1, m));	}
}
//===================================================+ DETERMINISTIC +===================================================
unsigned int deterministic_calc(unsigned int current){
unsigned int i;
	for(i = 5; i * i <= current; i += 4){
		if(current % i == 0){return 0;}
		i += 2;		if(i * i > current){return 1;}
		else if(current % i == 0){return 0;}
	}
	return 1;
}
//-----------------------------------------------------------------------------------------------------------------------
void deterministic(unsigned int current, unsigned int range){
	clock_t time = clock();
	unsigned int counter = 0;
	unsigned int i;

	printf("Deterministic algorithm: Found ");

	if(current == 1){current++;}
	if(current == 2){current++;	counter++;}
	if(current == 3){current += 2;	counter++;}

	if(current % 2 == 0 && current + 1 < range){current++;}

	for(;(current + 1) % 6 != 0 && current + 2 <= range;current += 2){	//"current" normalizer to the expected
		if(current % 3 == 0){continue;}					//values {5,11,17,23,29,..... (6*n)-1}
		counter += deterministic_calc(current);
	}
	for(; current + 2 <= range; current += 4){				//{2,4}step prime scanner

		counter += deterministic_calc(current);
		current += 2;
		counter += deterministic_calc(current);
	}
	if(current < range){
		counter += deterministic_calc(current);
	}

	printf("%d", counter);
	printf(" primes in ");
	time = clock() - time;
	if(((double)time)/CLOCKS_PER_SEC < 10.0){printf(" ");}
	if(((double)time)/CLOCKS_PER_SEC < 100.0){printf(" ");}
	printf("%lf\n", ((double)time)/CLOCKS_PER_SEC);
}
//==================================================+ MILLER - RABIN  +==================================================
unsigned int miller_rabin_calc(unsigned long a, unsigned long r,unsigned long d,unsigned long current){
	unsigned int j, x; 
	x = modulo(a,d,current);
	if(x == 1||x == current - 1){		return 1;}
	for(j = 0;j < r-1 && r > 1;j++){
		x = modulo(x, 2, current);
		if (x == current - 1){	return 1;}
	}
	return 0;
}
//-----------------------------------------------------------------------------------------------------------------------
void miller_rabin(unsigned int current, unsigned int range){
	clock_t time = clock();
	unsigned int counter = 0;
	unsigned int i;
	unsigned int r, d;

	printf("Miller-Rabin  algorithm: Found ");

	if(current == 1){current++;}
	if(current == 2){current++;	counter++;}
	if(current == 3){current += 2;	counter++;}

	if(current % 2 == 0 && current + 1 <= range){current++;}

	for(;(current + 1) % 6 != 0 && current + 2 <= range;current += 2){	//"current" normalizer to the expected
		if(current % 3 == 0){continue;}					//values {5,11,17,23,29,..... (6*n)-1}

		if(current == 7 || current == 61){counter ++; continue;}
		for(r = 0;((current - 1) >> r) % 2 == 0;r++){}
		d = ((current - 1) >> r);
		counter += (miller_rabin_calc(2, r, d, current)
			 && miller_rabin_calc(7, r, d, current)
			&& miller_rabin_calc(61, r, d, current));
	}
	for(; current + 2  <= range; current += 4){				//{2,4}step prime scanner

		if(current == 7 || current == 61){counter ++; continue;}
		for(r = 0;((current - 1) >> r) % 2 == 0;r++){}
		d = ((current - 1) >> r);
		counter += (miller_rabin_calc(2, r, d, current)
			 && miller_rabin_calc(7, r, d, current)
			&& miller_rabin_calc(61, r, d, current));

		current += 2;

		if(current == 7 || current == 61){counter ++; continue;}
		for(r = 0;((current - 1) >> r) % 2 == 0;r++){}
		d = ((current - 1) >> r);
		counter += (miller_rabin_calc(2, r, d, current)
			 && miller_rabin_calc(7, r, d, current)
			&& miller_rabin_calc(61, r, d, current));
	}
	if(current < range){
		if(current == 7 || current == 61){counter ++; current += 2;}
		for(r = 0;((current - 1) >> r) % 2 == 0;r++){}
		d = ((current - 1) >> r);
		counter += (miller_rabin_calc(2, r, d, current)
			 && miller_rabin_calc(7, r, d, current)
			&& miller_rabin_calc(61, r, d, current));
	}
	printf("%d", counter);
	printf(" primes in ");
	time = clock() - time;
	if(((double)time)/CLOCKS_PER_SEC < 10.0){printf(" ");}
	if(((double)time)/CLOCKS_PER_SEC < 100.0){printf(" ");}
	printf("%lf\n", ((double)time)/CLOCKS_PER_SEC);
}

//=======================================================+ MAIN  +=======================================================
int main(){
	printf("\n");
	if(MAXIMUM > UINT_MAX){printf("Houston, we've had a problem here\n");return 0;}

	printf("Checking range [%ld,%ld] for primes\n", (unsigned long)MINIMUM, (unsigned long)MAXIMUM);

	deterministic(MINIMUM, MAXIMUM);
	miller_rabin(MINIMUM, MAXIMUM);

	printf("\n");
	return 0;
}

