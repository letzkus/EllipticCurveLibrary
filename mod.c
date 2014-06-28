/*
 * Implementation of modulo operations
 *
 * TEAM
 * Letzkus, Fabian, 1480247 (TU Darmstadt)
 * Springer, Markus, 1401899 (TU Darmstadt) 
 */

#include <stdint.h>

/*
 * FUNCTION
 *  mod_f163
 *
 * INPUT
 *	 input vector a > f(x) as mentioned in description
 *	 
 * OUTPUT
 *   output vector a reduced by f
 * 
 * RETURN
 *   -
 * 
 * DESCRIPTION/REMARKS
 *   Optimized for f(x) = x^163 + x^7 + x^6 + x^3 + 1
 * 
 */
void mod_f163(uint32_t *c) {
	uint32_t t;
	int j;
	for (j=10;j>5;j--) { 
		t = c[j];
		c[j-6] = c[j-6] ^ (t << 29);
		c[j-5] = c[j-5] ^ (t << 4) ^ (t << 3) ^ t ^ (t >> 3);
		c[j-4] = c[j-4] ^ (t >> 28) ^ (t >> 29);
	}
	t = c[5] >> 3; // = (c[5] >> 3) << 3;
	c[0] = c[0] ^ (t << 7) ^ (t << 6) ^ (t << 3) ^ t;
	c[1] = c[1] ^ (t >> 25) ^ (t >> 26);
	c[5] = c[5] & 0x7;
}
