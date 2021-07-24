#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <limits.h>

#define MINIMUM 3990000000UL	// UL == unsigned long
#define MAXIMUM 4010000000UL
#define ROOF    4759123141UL

//======================================================+ MODULO  +======================================================
static unsigned long modulo(unsigned long p, unsigned long k, unsigned long m)
{
	if (k == 0)
		return 1;
	else if (k % 2 == 1)
		return((p * modulo(p, k-1, m)) % m);
	else
		return(modulo((p*p) % m, k/2, m));
}

//===================================================+ DETERMINISTIC +===================================================
static unsigned long deterministic_calc(unsigned long current)
{
	for (unsigned long i = 5; i * i <= current; i += 4) {
		if (current % i == 0)
			return 0;
		i += 2;
		if (i * i > current)
			return 1;
		else if (current % i == 0)
			return 0;
	}
	return 1;
}

static void deterministic(unsigned long current, unsigned long range)
{
	clock_t start = clock();
	unsigned long counter = 0;

	for (; current < range; current++) {
		if (current == 1)
			continue;

		if (current == 2)
			counter++;
		if (current % 2 == 0)
			continue;

		if (current == 3)
			counter++;
		if (current % 3 == 0)
			continue;
		
		counter += deterministic_calc(current);
	}
	printf("Deterministic algorithm: Found %d primes", counter);

	clock_t end = clock();
	printf(" in %lf\n", ((double)(end - start))/CLOCKS_PER_SEC);
}
//==================================================+ MILLER - RABIN  +==================================================
static unsigned long miller_rabin_calc(unsigned long a, unsigned long r, unsigned long d, unsigned long current)
{
	unsigned long x = modulo(a,d,current);
	if (x == 1 || (x == current - 1))
		return 1;
	for (unsigned long j = 0;j < r-1 && r > 1;j++) {
		x = modulo(x, 2, current);
		if (x == current - 1)
			return 1;
	}
	return 0;
}

static void miller_rabin(unsigned long current, unsigned long range)
{
	clock_t start = clock();
	unsigned long counter = 0;

	for (; current < range; current++) {
		if (current == 1)
			continue;

		if (current == 2)
			counter++;
		if (current % 2 == 0)
			continue;

		if (current == 3)
			counter++;
		if (current % 3 == 0)
			continue;
		
		if (current == 7 || current == 61) {
			counter ++;
			continue;
		}
		unsigned long r = 0;
		for (r = 0;((current - 1) >> r) % 2 == 0;r++) {}

		unsigned long d = ((current - 1) >> r);
		counter += (
			miller_rabin_calc(2, r, d, current)
			&& miller_rabin_calc(7, r, d, current)
			&& miller_rabin_calc(61, r, d, current)
		);
	}
	printf("Miller-Rabin  algorithm: Found %d primes", counter);

	clock_t end = clock();
	printf(" in %lf\n", ((double)(end - start))/CLOCKS_PER_SEC);
}

//=======================================================+ MAIN  +=======================================================
int main()
{
	printf("\n");
	if (MAXIMUM > ROOF) {
		printf("MAXIMUM should be less than %ld\n", ROOF);
		return 0;
	}
	if (MINIMUM > MAXIMUM) {
		printf("MINIMUM should be less than MAXIMUM\n");
		return 0;
	}

	printf("Checking range [%ld,%ld] for primes:\n", MINIMUM, MAXIMUM);

	deterministic(MINIMUM, MAXIMUM);
	miller_rabin(MINIMUM, MAXIMUM);

	printf("\n");
	return 0;
}
