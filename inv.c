/*
 * Implementation for the calculation of the multiplicative inverse 
 *
 */

#include <stdint.h>

/* 
 * FUNCTION 
 *   multInv_bp
 *
 * INPUT
 *   m - extension degree of binary field
 *   t - number of 32-bit words needed to store finite field element
 *   *a - vector from which the multiplative inverse is calculated 
 *
 * OUTPUT
 *   *b - vector representing the mult. inverse of input vector a
 *
 * RETURN 
 *   -
 *
 * DESCRIPTION/REMARKS
 *   The function calculates a^(-1) within the binary field F_2^m
 *
 */
void multInv(uint32_t m, uint32_t t, uint32_t *a, uint32_t *b) {
	
	
	
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
uint32_t test_multInv() {
	
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
}
