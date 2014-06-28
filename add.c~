/*
 * Implementation of add operations
 *
 * TEAM
 * Letzkus, Fabian, 1480247 (TU Darmstadt)
 * Springer, Markus, 1401899 (TU Darmstadt) 
 */

#include <stdint.h>

/*
 * FUNCTION
 *  add
 *
 * INPUT
 *   length t of vector
 *	 input vector a
 *   input vector b
 *
 * OUTPUT
 *   result vector c
 * 
 * RETURN
 *   -
 * 
 * DESCRIPTION/REMARKS
 *   Adds the two vectors a and b and stores result in vector c. 
 *   All vectors are of length t.
 *   It is assumed that all vectors are already mod f
 * 
 * EXAMPLE
 * 	 a = x^2 + x^1 + 1 => [00000111,0,0,0]
 * 	 b = x^2 + x^1 => [00000110,0,0,0]
 *   f = x^2 + 1 => [0000101,0,0,0]
 * 
 *	 result = a+b mod f = a+b = 1 => [00000001,0,0,0]
 *
 * 	 As one can see, thats simple bit-wise XOR.
 */
void add(uint32_t t, uint32_t *a, uint32_t *b, uint32_t *c) {
	int i;
	for(i=0;i<t;i++) {
		c[i] = a[i] ^ b[i];
	}
}
