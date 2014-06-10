/*
 * Implementation for the calculation of the multiplicative inverse 
 *
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
//#include "mult.c" // buggy?
#include "add.c"
#include "shift.c"

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
	uint32_t i = s * 32;
	
	while(i > 0){
		if((a[s-1] & mask) > 0)
			return 0;

		i -= 1;
		shiftl(s, a, 1, s, a);
	}

	return 0;
}


/* 
 * FUNCTION 
 *   lsc
 *
 * INPUT
 *   s - size of vector a
 *   a - vector from which the multiplative inverse is calculated 
 *
 * OUTPUT
 *   index of lowest coefficient != 0
 *
 * RETURN 
 *   -1 if no coefficient is 1
 *
 * DESCRIPTION/REMARKS
 *   Caclulates index of lowest coefficient != 0
 *
 */
uint32_t lsc(uint32_t s, uint32_t *a) {
	uint32_t mask = 0x00000001;
	uint32_t i = 0;

	while(i < s){
		if((a[i] & mask) == 1)
			return i;
		
		i += 1;
		shiftr(s, a, 1, a);
	}
	
	return -1;
}

/* 
 * FUNCTION 
 *   isZero
 *
 * INPUT
 *   s - size of vector a
 *   a - vector containing the polynpomial 
 *
 * OUTPUT
 *   -
 *
 * RETURN 
 *   1 if given polynomial is 0
 *   0 if given polynomial is NOT 0
 *
 * DESCRIPTION/REMARKS
 *   The function calculates a^(-1) within the binary field F_2^m
 *
 */
int isZero(uint32_t s, uint32_t *a) {
	int i = 0;	

	for(i; i < s; i++)
		if(a[i] != 0)
			return 0;		

	return 1;
}

/* 
 * FUNCTION 
 *   multInv_bp
 *
 * INPUT
 *   m - extension degree of binary field
 *   s_a - size of vector a
 *   a - vector from which the multiplative inverse is calculated 
 *   s_b - size of vector b
 *   b - vector 
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
void multInv(uint32_t s_a, uint32_t *a, uint32_t s_b, uint32_t *b, uint32_t *c) {
	// First implementation using iterative extended binary gcd algorithm (by Stein)
	
	// Initialize 
	uint32_t *ca = (uint32_t *) malloc(sizeof(uint32_t) * (s_a+1));
	memcpy (ca, a, (sizeof(uint32_t) * (s_a))); // copy a
	uint32_t *v = (uint32_t *) malloc(sizeof(uint32_t) * (s_a+1));	
	uint32_t *cb = (uint32_t *) malloc(sizeof(uint32_t) * (s_b+1));
	memcpy (cb, b, (sizeof(uint32_t) * (s_b))); // copy b
	uint32_t *u = (uint32_t *) malloc(sizeof(uint32_t) * (s_a+1));	
	uint32_t *g1 = (uint32_t *) malloc(sizeof(uint32_t) * (s_a+1));// size ???
	uint32_t *g2 = (uint32_t *) malloc(sizeof(uint32_t) * (s_a+1));// size ???
	uint32_t *h1 = (uint32_t *) malloc(sizeof(uint32_t) * (s_a+1));// size ???
	uint32_t *h2 = (uint32_t *) malloc(sizeof(uint32_t) * (s_a+1));// size ???
	uint32_t *e = (uint32_t *) malloc(sizeof(uint32_t) * (s_a+1));// size ???
	uint32_t *lambda;	// Type correct???
	uint32_t hint;
	uint32_t *helper = (uint32_t *) malloc(sizeof(uint32_t) * (s_a+1)); // helper variable

	e[0] = 1;
	
	// Preparationstep
	while((a[0]%2 == 0) && (b[0]%2 == 0)){
		//shiftl(uint32_t la, uint32_t *a, uint32_t k, uint32_t lb, uint32_t *b)
		//shiftr(uint32_t la, uint32_t *a, uint32_t k, uint32_t *b)	
		shiftr(s_a, ca, 1, ca);
		shiftr(s_b, cb, 1, cb);
		shiftl(s_a, e, 1, s_a, e);		
	}

	g1[0] = 1;
	g2[0] = 0;
	h1[0] = 0;
	h2[0] = 1;

	memcpy (u, ca, (sizeof(uint32_t) * (s_a))); // copy ca
	memcpy (v, cb, (sizeof(uint32_t) * (s_b))); // copy cb

	// while (u != 0)
	while(isZero(s_a,u)){
		
		while (v[0] % 2 == 0){
			 // TODO
			shiftr(s_b, v, 1, v);
			
			if((g1[0] % 2 == 0) && (h1[0] % 2 == 0)){ // funcktioniert das? überprüfung falsch
				shiftr(s_a, g1, 1, g1);
				shiftr(s_a, h1, 1, h1);
			} else if ((g1[0] % 2 == 0) && (h1[0] % 2 != 0)) {
				// TODO this is definitly wrong...
				hint = lsc(s_a,a);				
				lambda = (uint32_t *) calloc(sizeof(uint32_t) * ((hint/32)+1+1));
				lambda[0] = 0x1;
				shiftl((hint/32)+1, lambda, hint, (hint/32)+1, lambda);
				multInv((hint/32)+1, lamdba, s_b, b, lambda)
				

				// Calculate new g1
				// helper = b*lambda
				add(s_a, g1, helper, g1);
				shiftr(s_a, g1, 1, g1);
				
				// Calculate new h1
				// helper = a*lambda
				add(s_a, h1, helper, h1);
				shiftr(s_a, h1, 1, h1);

				free(lambda);
			} else {
				// TODO this is definitly wrong...
			}	

		}

		while (u[0] % 2 == 0){
			// TODO
			shiftr(s_a, u, 1, u);
			
			if((g2[0] % 2 == 0) && (h2[0] % 2 == 0)){
				shiftr(s_a, g2, 1, g2);
				shiftr(s_a, h2, 1, h2);
			} else if ((g2[0] % 2 == 0) && (h2[0] % 2 != 0)) {
				// TODO

			} else {
				// TODO
			}	
		
		}

		if (deg(s_b, v) >= deg(s_a,u)){
			// TODO

		} else {
			// TODO

		}

	}

	// Set result

	// Finalize
	free(ca);
	free(cb);
	free(u);
	free(v);
	free(g1);
	free(g2);
	free(h1);
	free(h2);
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

	uint32_t a[6] = {0x00000002, 0x00000000, 0x00000000, 
			0x00000000, 0x00000000, 0x00000000};

	uint32_t b[6] = {0x00000007, 0x00000000, 0x00000000, 
			0x00000000, 0x00000000, 0x00000000};

	uint32_t c[6];

	// Test 1
	printf("Multiplicative inverse tests:\n\nTest 1:");
	printf("multInv(7,2) = 4\n");

	multInv(6, b, 6, a, c);
	
	if(c[0] == 4){
		printf("Test passed!");
	} else {
		printf("Error!");
	}
	printf("\n\n");


	// Test 2
	printf("Test 2: TODO Formular...");
	

	/*
	uint32_t t = 6;
	uint32_t m = 7;
	
	uint32_t a[6] = {0x00000002, 0x00000000, 0x00000000, 
			0x00000000, 0x00000000, 0x00000000}; // 2 -> multInv(7, 2) = 3
	uint32_t b[6] = {0x00000000, 0x00000000, 0x00000000, 
			0x00000000, 0x00000000, 0x00000000}; // should be 3
	uint32_t c[6] = {0x00000000, 0x00000000, 0x00000000, 
			0x00000000, 0x00000000, 0x00000000}; // temporary...
	
	printf("\n************************************************************\n");
	printf("test: multInv\n");
	
	multInv(m, t, a, b);
	multInv(m, t, b, c);
		
	if(f2m_is_equal(t, a, c)) {
		printf("result: true\n");
		printf("\n************************************************************\n");
		return 0;
	}
	
	printf("result: false\n");
	printf("\n************************************************************\n");
	return 1;
	*/
}


// TODO REMOVE
void main(){
	test_multInv();
}
