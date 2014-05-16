/*
 * Collection of algorithms to calculate the multiplication within finite fields
 *
 */

/* 
 * FUNCTION 
 *   bp_lookup
 *
 * INPUT
 *   a - lookup key, max. 32bit
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
void bp_lookup(uint32_t *a, uint32_t *b, uint32_t *t) {
	uint32_t lookupTable[16] = {0, 1, 4, 5,
								16, 17, 20, 21
								64, 65, 68, 69
								80, 81, 84 85};
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
	
	uint32_t s[2*t];
	
	
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
void test_mult() {
	
	uint32_t t = 6;
	uint32_t m = 7;
	
	uint32_t a[t] = {0x00000002, 0x00000000, 0x00000000, 
			0x00000000, 0x00000000, 0x00000000}; // 2 -> multInv(7, 2) = 3
	uint32_t b[t] = {0x00000000, 0x00000000, 0x00000000, 
			0x00000000, 0x00000000, 0x00000000}; // should be 3
	uint32_t c[t] = {0x00000000, 0x00000000, 0x00000000, 
			0x00000000, 0x00000000, 0x00000000}; // temporary...
	
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