/*
 * Collection of algorithms to calculate the multiplication within finite fields
 *
 * TEAM
 * Letzkus, Fabian, 1480247 (TU Darmstadt)
 * Springer, Markus, 1401899 (TU Darmstadt) 
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

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
 *
 */
void mult(uint32_t la, uint32_t *a, uint32_t lb, uint32_t *b, uint32_t *c) {

	// Initialize 				
	memcpy (ca, a, (sizeof(uint32_t) * la)); // Copy of a	
	memcpy (cb, b, (sizeof(uint32_t) * (lb))); // cb = b	
	
	int k;
	int j;

	// Prepare result vector
	for(j = 0; j < la; j++)
		c[j]=0x00000000; 
			
	// Multiply a & b
	for(j=0;j<la;j++) { 
		for(k=0;k<32;k++) {
			if((ca[j] >> k) & 0x1) {// is bit k of a[j] = 1? 
				add(6,c,cb,c);	// c = b ^ c 
			}

			shiftBl(lb,cb);	// b << 1

			if(cb[5] >> 3){
				cb[5] = cb[5] & 0xFFFFFFF7;
				cb[0] = cb[0] ^ 0x000000C9; // Rest of F				
			}		
		}
	}
}
