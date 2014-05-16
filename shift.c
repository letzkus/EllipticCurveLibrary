/*
 * Implementation of shift operations
 */

/*
 * FUNCTION
 *  shiftr
 *
 * INPUT
 *   length t of vector
 *	 input vector a
 *   shift k
 *	 
 *
 * OUTPUT
 *   result vector b
 * 
 * RETURN
 *   -
 * 
 * DESCRIPTION/REMARKS
 *   Shifts vector a k digits to the right and stores result in vector c. 
 * 
 * EXAMPLE
 * 	 a = x^2 + x^1 + 1 => [00000111,0,0,0]
 *   k = 1
 *	 f = x^3 + 1
 *
 *	 result = a >> k mod f = ...?
 *
 */
void shiftr(uint32_t t, uint32_t *a, uint32_t k, uint32_t *b) {
	// b = a >> k for a = a[t]...a[0]
}


/*
 * FUNCTION
 *  shiftl
 *
 * INPUT
 *   length t of vector
 *	 input vector a
 *   shift k
 *	 
 *
 * OUTPUT
 *   result vector b
 * 
 * RETURN
 *   -
 * 
 * DESCRIPTION/REMARKS
 *   Shifts vector a k digits to the left and stores result in vector c. 
 * 
 * EXAMPLE
 * 	 a = x^2 + x^1 + 1 => [00000111,0,0,0]
 *   k = 1
 *	 f = x^3 + 1
 *
 *	 result = a << k mod f = ...?
 *
 */
void shiftl(uint32_t t, uint32_t *a, uint32_t k, uint32_t *b) {
	// TODO
	// important for R2L_mult
}