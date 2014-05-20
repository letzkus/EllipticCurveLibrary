/*
 * Implementation of modulo operations
 */

/*
 * FUNCTION
 *  mod
 *
 * INPUT
 *   length t of vector
 *	 input vector a
 *   modul vector f
 *	 
 *
 * OUTPUT
 *   result vector b
 * 
 * RETURN
 *   -
 * 
 * DESCRIPTION/REMARKS
 *   b = a mod f
 * 
 * EXAMPLE
 * 	 
 *
 */
void mod(uint32_t t, uint32_t *a, uint32_t *f, uint32_t *b) {
	// b = a >> k for a = a[t]...a[0]
}

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
	for (int j=10;j>5;j--) {
		t = c[j];
		c[j-6] = c[j-6] ^ (t << 29);
		c[j-5] = c[j-5] ^ (t << 4) ^ (t << 3) ^ (t >> 3) ^ t;
		c[j-4] = c[j-4] ^ (t >> 28) ^ (t >> 29)
	}
	t = (c[5] >> 3) << 3; // == c[5] & 0xFFFFFFF8
	c[0] = c[0] ^ (t << 4) ^ (t << 3) ^ (t >> 3);
	c[1] = c[1] ^ (t >> 28) ^ (t >> 29);
	c[5] = c[5] & 0x7;
}
