/*
 * Collection of algorithms to calculate the multiplication within finite fields
 *
 */

#include <stdint.h>
#include "add.c"
#include "shift.c"

/* 
 * FUNCTION 
 *   bp_lookup
 *
 * INPUT
 *   a - lookup key, max. 32bit
 * 	 s - length of key s
 *
 * OUTPUT
 *   b - vector representing the result of the multiplication
 *	 t - length of output
 *
 * RETURN 
 *   0 - everything ok
 * 	 1 - key not found
 *
 * DESCRIPTION/REMARKS
 *   Makes a lookup to enable fast calculation of binary polynomquadration.
 *	 Is currently fixed to 4 bit keys, so key t is useless right now...
 *
 */
int bp_lookup(uint32_t *a, uint32_t s, uint32_t *b, uint32_t *t) {
	uint32_t lookupTable[16] = {0, 1, 4, 5, 16, 17, 20, 21, 64, 65, 68, 69, 80, 81, 84, 85};
				
	// TODO implement me :)
								
}

/* 
 * FUNCTION 
 *   bp_mult
 *
 * INPUT
 *   m - extension degree of binary field
 *   t - number of 32-bit words needed to store finite field element
 *   *a - vector which will be multiplicated with itself
 *
 * OUTPUT
 *   *b - vector representing the result of the multiplication
 *
 * RETURN 
 *   -
 *
 * DESCRIPTION/REMARKS
 *   The function calculates a(x)*a(x) mod m within the binary field F_2^m
 * 	 This implementation makes use of the binary polynomquadration algorithm.
 *
 */
void bp_mult(uint32_t m, uint32_t t, uint32_t *a, uint32_t *b) {
		
	// TODO implement me :)
	
}

/* 
 * FUNCTION 
 *   r2lvec_mult
 *
 * INPUT
 *    t - length t of array A
 *   *a - vector a
 *	  b - copy of vector b
 * 	  f - extension degree of binary field

 * OUTPUT
 *   *c - result of the multiplication of a and b mod f
 *
 * RETURN 
 *   -
 *
 * DESCRIPTION/REMARKS
 *   The function calculates a*b mod f within the binary field F_2^m
 * 	 This implementation makes use of the R2L Shift and Add Algorithm for vectors
 *
 */
void r2lvec_mult(uint32_t t, uint32_t *a, uint32_t b, uint32_t *c, uint32_t *f) {
	
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
			
			shiftl(t,b,1,b);			// b << 1
	
			if(b[t-1] >> 7 == 1) {		// if b_m == 1 then
				add(t,b,r,b)			//		b = b ^ r		
			}			
			
		}
	}
}

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
int test_mult() {
	
	printf("\n************************************************************\n");
	printf("test: multInv\n");

	// TODO insert test here
		
	if(false) { // TODO insert condition here
		printf("result: true\n");
		printf("\n************************************************************\n");
		return 0;
	}
	
	printf("result: false\n");
	printf("\n************************************************************\n");
	return 1;
}