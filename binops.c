/*
 * Implementation of modulo operations
 */

/*
 * FUNCTION
 *  getBit
 *
 * INPUT
 *   length t of vector
 *	 input vector a
 *   bit index j from [0,31]
 *   array index k
 *
 * OUTPUT
 *   - 
 * 
 * RETURN
 *   bit A[j]_i
 * 
 * DESCRIPTION/REMARKS
 *   
 * 
 * EXAMPLE
 *
 */
uint32_t getBit(uint32_t *a, uint32_t arrayIndex, uint32_t bitIndex) {
	return (a[arrayIndex] >> bitIndex) & 0x1;
}

void and(uint32_t t, uint32_t *a, uint32_t *b, uint32_t *c) {
	int i;
	for(i=0;i<t;i++) {
		c[i] = a[i] & b[i];
	}
}

void or(uint32_t t, uint32_t *a, uint32_t *b, uint32_t *c) {
	int i;
	for(i=0;i<t;i++) {
		c[i] = a[i] | b[i];
	}
}
