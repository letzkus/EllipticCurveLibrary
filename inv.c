/*
 * Implementation for the calculation of the multiplicative inverse 
 *
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
//#include "mult.c" // buggy?
//#include "add.c" // TODO Uncomment for DEBUG
//#include "shift.c" // TODO Uncomment for DEBUG

/* 
 * FUNCTION 
 *   deg
 *
 * INPUT
 *   s - size of vector a
 *   a - vector from which the multiplative inverse is calculated 
 *
 * OUTPUT
 *   degree of given polynomial
 *
 * RETURN 
 *   degree of given polynomial
 *
 * DESCRIPTION/REMARKS
 *   Caclulates the degree of the given polynomial
 *
 */
uint32_t deg(uint32_t s, uint32_t *a) {
	uint32_t mask = 0x80000000;
	uint32_t i = (s * 32) -1;
	uint32_t *c = (uint32_t *)malloc(sizeof(uint32_t) * (s+1));
	memcpy(c,a,(sizeof(uint32_t) * (s)));
	
	while(i > 0){
		if((c[s-1] & mask) > 0){
			free(c);		
			return i;
		}
		i -= 1;
		shiftl(s, c, 1, s, c);
	}

	free(c);
	return 0;
}


/* 
 * FUNCTION 
 *   isOne
 *
 * INPUT
 *   s - size of vector a
 *   a - vector containing the polynpomial 
 *
 * OUTPUT
 *   -
 *
 * RETURN 
 *   1 if given polynomial is 1
 *   0 if given polynomial is NOT 1
 *
 * DESCRIPTION/REMARKS
 *   The function calculates a^(-1) within the binary field F_2^m
 *
 */
int isOne(uint32_t s, uint32_t *a) {
	int i = s-1;	

	for(i; i > 0; i--)
		if(a[i] != 0)
			return 0;		

	if(a[0] == 1)
		return 1;

	return 0;
}

/* 
 * FUNCTION 
 *   multInv_bp
 *
 * INPUT
 *   m - extension degree of binary field
 *   s_a - size of vector a
 *   a - vector from which the multiplative inverse is calculated 
 *   s_f - size of vector f
 *   f - vector 
 *
 * OUTPUT
 *   c - vector representing the mult. inverse of input vector a
 *
 * RETURN 
 *   -
 *
 * DESCRIPTION/REMARKS
 *   The function calculates a^(-1) within the binary field F_2^m
 *
 */
void multInv(uint32_t sa, uint32_t *a, uint32_t sf, uint32_t *f, uint32_t *c) {
	// degree of a is at most m-1 => array size should be that of f
	// => sf >= sa	

	// Initialize 
	//uint32_t *u = (uint32_t *) malloc(sizeof(uint32_t) * (s_a+1));	
	uint32_t *u = (uint32_t *) calloc(sf, sizeof(uint32_t));		
	memcpy (u, a, (sizeof(uint32_t) * (sa))); // u = a	
	//uint32_t *v = (uint32_t *) malloc(sizeof(uint32_t) * (s_a+1));
	uint32_t *v = (uint32_t *) calloc(sf, sizeof(uint32_t));	
	memcpy (v, f, (sizeof(uint32_t) * (sf))); // v = f	
	uint32_t *g1 = (uint32_t *) calloc(sf, sizeof(uint32_t));// size ???
	uint32_t *g2 = (uint32_t *) calloc(sf, sizeof(uint32_t));// size ??
	
	g1[0] = 1;
	//g2[0] = 0;

	while(!isOne(sf, u) && !isOne(sf,v)){
		
		while ((u[0] & 0x1) != 1){	// u divisible by x ?
			shiftr(sf, u, 1, u);	// u = u / x
			
			if((g1[0] & 0x1) != 1) {	// g1 divisible by x?
				shiftr(sf, g1, 1, g1);	// g1 = g1 / x
			} else {
				add(sf, g1, f, g1);	// g1 = g1 + f
				shiftr(sf, g1, 1, g1);		// g1 = g1 / x
			}
		}

		while ((v[0] & 0x1) != 1){		// v divisible by x
			shiftr(sf, v, 1, v);	// v = v / x
			
			if((g2[0] & 0x1) != 1) {	// g2 divisible by x?
				shiftr(sf, g2, 1, g2);	// g2 = g2 / x
			} else {
				add(sf, g2, f, g2);	// g2 = g2 + f
				shiftr(sf, g2, 1, g2);		// g2 = g2 / x
			}
		}

		if (deg(sf,u) > deg(sf,v)){
			add(sf, u, v, u);		// u = u + v
			add(sf, g1, g2, g1);		// g1 = g1 + g2
		} else {
			add(sf, u, v, v);		// v = u + v
			add(sf, g1, g2, g2);		// g2 = g1 + g2
		}
	}

	// Set result
	if(isOne(sf,u)){
		memcpy (c, g1, (sizeof(uint32_t) * (sf))); // a^-1 = g1
	} else {
		memcpy (c, g2, (sizeof(uint32_t) * (sf))); // a^-1 = g2
	}

	// Finalize
	free(u);
	free(v);
	free(g1);
	free(g2);
}

/* 
 * FUNCTION 
 *   multInv_bp
 *
 * INPUT
 *   m - extension degree of binary field
 *   s_a - size of vector a
 *   a - vector from which the multiplative inverse is calculated 
 *   s_f - size of vector f
 *   f - vector 
 *
 * OUTPUT
 *   c - vector representing the mult. inverse of input vector a
 *
 * RETURN 
 *   -
 *
 * DESCRIPTION/REMARKS
 *   The function calculates a^(-1) within the binary field F_2^m
 *
 */
void multInv_test(uint32_t s_a, uint32_t *a, uint32_t s_f, uint32_t *f, uint32_t *c) {
	// Iterative extended binary gcd algorithm (by Stein)
	
	// Initialize 
	//uint32_t *u = (uint32_t *) malloc(sizeof(uint32_t) * (s_a+1));	
	uint32_t *u = (uint32_t *) calloc(s_a, sizeof(uint32_t));		
	memcpy (u, a, (sizeof(uint32_t) * (s_a))); // u = a	
	//uint32_t *v = (uint32_t *) malloc(sizeof(uint32_t) * (s_a+1));
	uint32_t *v = (uint32_t *) calloc(s_a, sizeof(uint32_t));	
	memcpy (v, f, (sizeof(uint32_t) * (s_a))); // v = f	
	uint32_t *g1 = (uint32_t *) calloc(s_a, sizeof(uint32_t));// size ???
	uint32_t *g2 = (uint32_t *) calloc(s_a, sizeof(uint32_t));// size ??
	
	g1[0] = 1;
	g2[0] = 0;

	while(!isOne(s_a, u) && !isOne(s_a,v)){
		
		while ((u[0] & 0x1) != 1){	// u divisible by x ?
			shiftr(s_a, u, 1, u);	// u = u / x
			
			if((g1[0] & 0x1) != 1) {	// g1 divisible by x?
				shiftr(s_a, g1, 1, g1);	// g1 = g1 / x
			} else {
				addI(s_a, g1, s_f, f, g1);	// g1 = g1 + f
				shiftr(s_a, g1, 1, g1);		// g1 = g1 / x
			}
		}

		while ((v[0] & 0x1) != 1){		// v divisible by x
			shiftr(s_a, v, 1, v);	// v = v / x
			
			if((g2[0] & 0x1) != 1) {	// g2 divisible by x?
				shiftr(s_a, g2, 1, g2);	// g2 = g2 / x
			} else {
				addI(s_a, g2, s_f, f, g2);	// g2 = g2 + f
				shiftr(s_a, g2, 1, g2);		// g2 = g2 / x
			}
		}

		if (deg(s_a,u) > deg(s_a,v)){
			add(s_a, u, v, u);		// u = u + v
			add(s_a, g1, g2, g1);		// g1 = g1 + g2
		} else {
			add(s_a, u, v, v);		// v = u + v
			add(s_a, g1, g2, g2);		// g2 = g1 + g2
		}
	}

	// Set result
	if(isOne(s_a,u)){
		memcpy (c, g1, (sizeof(uint32_t) * (s_a))); // a⁻1 = g1
	} else {
		memcpy (c, g2, (sizeof(uint32_t) * (s_a))); // a⁻1 = g2
	}

	// Finalize
	free(u);
	free(v);
	free(g1);
	free(g2);
}

/* 
 * FUNCTION 
 *   test_multInv
 *
 * INPUT
 *   -
 *
 * OUTPUT
 *   -
 *
 * RETURN 
 *   + 0 if test passed
 *   + 1 if test failed
 *
 * DESCRIPTION/REMARKS
 *   The function generates random values a(z) 
 *   from the binary field generated by the polynomial
 *   f(z) = z^163 + z^7 + z^6 + z^3 + 1.
 *   We have: 11001001 = 0xC9.
 */
void test_multInv() {

	uint32_t m[6] = {0x000000C9, 0x00000000, 0x00000000, 
		  	0x00000000, 0x00000000, 0x00000008};

	uint32_t a[6] = {0x00000008, 0x00000000, 0x00000000,
			0x00000000, 0x00000000, 0x00000000};

	uint32_t b[6] = {0x00000033, 0x00000000, 0x00000000,
			0x00000000, 0x00000000, 0x00000000};

	uint32_t d[6] = {0x00000011, 0x00000000, 0x00000000,
			0x00000000, 0x00000000, 0x00000000};

	uint32_t c[6] = {0x0,0x0,0x0,0x0,0x0,0x0};

	// Test 1
	printf("Multiplicative inverse tests:\n\nTest 1:");
	printf("multInv(x^2,x^5+1) = x^2\n");

	multInv(6, a, 6, b, c);
	
	if(c[0] == 4 && c[1] == 0 && c[2] == 0){
		printf("Test passed!");
	} else {
		printf("Error!");
	}
	printf("\n\n");


	// Test 2
	printf("Test 2: multInv(4,17) = 13\n");
	multInv(6,d,6,a,c);

	if(c[2] == 0 && c[1] == 0 && c[0] == 0xD){
		printf("Test passed!");
	} else {
		printf("Error!");
	}
	printf("\n\n");
}
