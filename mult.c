/*
 * Collection of algorithms to calculate the multiplication within finite fields
 *
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
//#include "add.c" // TODO Uncomment for DEBUG
//#include "shift.c" // TODO Uncomment for DEBUG
//#include "mod.c" // TODO Uncomment for DEBUG
//#include "binops.c" // TODO Uncomment for DEBUG

uint32_t ca[6];
uint32_t cb[6];

/* 
 * FUNCTION 
 *   r2lvec_mult
 *
 * INPUT
 *   la - length of vector A
 *   *a - vector a
 *   lb - length t of vector B
 *   *b - vector b
 *   la - length of vector A
 *   *c - result vector c
 *
 * OUTPUT
 *   *c - result of the multiplication of a and b mod f
 *
 * RETURN 
 *   -
 *
 * DESCRIPTION/REMARKS
 *   The function calculates a*b mod f within the binary field F_2^m
 * 	 This implementation makes use of the R2L Shift and Add Algorithm for vectors
 *    For Vector f it is assumed to use the well known irreducable polynom
 *	  x^163 + x^7 + x^6 + x^3 + 1
 *    Vector c is expected to have a length of at least 10
 *
 */
void mult(uint32_t la, uint32_t *a, uint32_t lb, uint32_t *b, uint32_t *c) {

	// Initialize
	//uint32_t *ca = (uint32_t *) calloc((la+1), sizeof(uint32_t)); // Needed to ba able to use *a = *c 				
	memcpy (ca, a, (sizeof(uint32_t) * la)); // Copy of a	
	//uint32_t *cb = (uint32_t *) calloc((lb+1), sizeof(uint32_t)); // Copy of b 		
	memcpy (cb, b, (sizeof(uint32_t) * (lb))); // cb = b	
	
	int k;
	int j;

	for(j = 0; j < la; j++)
		c[j]=0x00000000; // make c = 0; TODO MAYBE throw this out
		

	//uint32_t mask = 0xFFFFFFF7;
	//uint32_t r = 0x000000C9;	
	
	// Multiply a & b
	for(j=0;j<la;j++) { // maybe verdreht!!!
		for(k=0;k<32;k++) {
			if((ca[j] >> k) & 0x1) {// is bit k of a[j] = 1? "getBit(ca,j,k) == 1"
				add(6,c,cb,c);	// c = b ^ c TODO optimization to length 5?
			}

			shiftBl(lb,cb);	// b << 1

			if(cb[5] >> 3){
				cb[5] = cb[5] & 0xFFFFFFF7;
				cb[0] = cb[0] ^ 0x000000C9; // Rest of F				
				//addI(lb,cb,1,r,cb);				
				// mod f
				//mod_f163(cb); // Works but slower
			}		
		}
	}
}

/*
void r2lvec_mult(uint32_t t, uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *f) {
	
	// f = x^m + r -> r = everything exept msb
	uint32_t *r;
	int i;
	int j;
	for(i=0;i<t;i++) {
		r[i] = f[i];
	}
	r[t-1] = r [t-1] && 127;
	
	for(i=0;i<t;i++) {
		for(j=0;j<32;j++) {
			
			if(getBit(t,a,i,j) == 1) { // if a_i = 0 then// if a_i = 0 then
				add(t,a,c,c);			 // 	c = c ^ a
			}

// void shiftl(uint32_t la, uint32_t *a, uint32_t k, uint32_t lb, uint32_t *b) 
			shiftl(t,b,1,b);			// b << 1
	
			if(b[t-1] >> 7 == 1) {		// if b_m == 1 then
				add(t,b,r,b)			//		b = b ^ r		
			}			
			
		}
	}
}
*/

/* 
 * FUNCTION 
 *   test_mult
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
 *   - 
 */
void test_mult() {

	/*uint32_t a[6] = {0x00000004, 0x00000000, 0x00000000,
			0x00000000, 0x00000000, 0x00000000};*/
	
	uint32_t a[5] = {0x8,0x0,0x0,0x0,0x0}; // x^3 

	uint32_t b[5] = {0x10,0x0,0x0,0x0,0x0}; // x^4

	uint32_t d[5] = {0x80008081,0x0,0x0,0x0,0x8000000}; // (x^155 + x^31 + x^15 + x^7 + 1)

	uint32_t e[1] = {0x100}; // x^8

	uint32_t c[10];
	
	printf("Multiplication of polynomes tests:\n\n");

	// Test 1
	printf("Test 1: (x^3) * (x^4) mod f = x^7\n");
		
	mult(5,a,5,b,c);
	if(c[0] == 0x80)
		printf("Result: x^7 => Passed!");
	else
		printf("Error!");

	printf("\n\n");
	
	// Test 2
	printf("Test 2: (x^155 + x^31 + x^15 + x^7 + 1) * (x^8) mod f\n");
	printf("= x^163 + x^39 + x^23 + x^15 + x^8 mod f\n");
	printf("= x^39 + x^23 + x^15 + x^8 + x^7 + x^6 + x^3 + 1\n");
	
	mult(5,d,1,e,c);
	if(c[0] == 0x8081C9 && c[1] == 0x80)
		printf("Result: => Passed!");
	else
		printf("Error!");

	printf("\n\n");
}
