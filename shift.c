/*
 * Implementation of shift operations
 */

#include <stdio.h>
#include <stdint.h>

/*
 * FUNCTION
 *  shiftr
 *
 * INPUT
 *   length la of vector
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
void shiftr(uint32_t la, uint32_t *a, uint32_t k, uint32_t *b) {
	// b = a >> k for a = a[t-1]...a[0]	 
	uint32_t ws;	// Word internal shift value
	uint32_t i;	// loopctr
	uint32_t uflow;	// Contains underflow of upper byte
	uint32_t cpy;	// Contains copy of current byte 

	// Copy a to result array
	for(i = 0; i < la; i++)
		b[i] = a[i];

	while(k > 0){
		if(k > 31){	// 32 bits is undefined in c ("nice" feature for a lovely sunday afternoon...) 
		    ws = 31;
		    k -= 31;
		} else {
		    ws = k;
		    k = 0;
		}

		uflow = (b[la-1] << (32-ws)); // Prepares underflow for successor byte
		b[la-1] >>= ws;		
		for(i = la-2; i >=1; i--){
			cpy = b[i];
			b[i] = (uflow | (b[i] >> ws));
			uflow = (cpy << (32-ws));
		}
		b[0] = (uflow | (b[0] >> ws)); // Needed to circumvent uint32_t underflow	
	}
}

/*
 * FUNCTION
 *  shiftl
 *
 * INPUT
 *   length la of vector
 *	 input vector a
 *   shift k
 *   length lb of 
 *	 input vector b
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
 *	 result = a << k 
 *
 */
void shiftl(uint32_t la, uint32_t *a, uint32_t k, uint32_t lb, uint32_t *b) {
	// TODO is overflow handling needed?
	// important for R2L_mult
	// b = a << k mod f for a = a[t-1]...a[0]	 
	uint32_t ws;	// Word internal shift value
	uint32_t i;	// loopctr
	uint32_t oflow;	// Contains overflow of lower byte
	uint32_t cpy;	// Contains copy of current byte

	// Copy a to result array
	for(i = 0; i < la; i++)
		b[i] = a[i];

	while(k > 0){
		if(k > 31){	// 32 bits is undefined in c ("nice" feature for a lovely sunday afternoon...) 
		    ws = 31;
		    k -= 31;
		} else {
		    ws = k;
		    k = 0;
		}

		oflow = (b[0] >> (32-ws)); // Prepares underflow for successor byte
		b[0] <<= ws;		
		for(i = 1; i < lb; i++){
			cpy = b[i];
			b[i] = (oflow | (b[i] << ws));
			oflow = (cpy >> (32-ws));
		}	
	}

	//free wcpy;	// Free allocation of working copy
}

/*
 *
 * TODO REMOVE IF POSSIBLE
 *
 */
void testprint(
  uint32_t t,
	uint32_t *A
)
{ 
  uint32_t i;
  printf("0x");
  for (i = 0; i < t; i++) printf("%.8X ",A[t-i-1]);
}

/*
 * FUNCTION
 *  shiftTEST
 *
 * OUTPUT
 *   -
 * 
 * RETURN
 *   -
 * 
 * DESCRIPTION/REMARKS
 *  Tests both shift functions 
 * 
 * EXAMPLE
 *   -	
 */
void shiftTEST() {
	// TODO
	uint32_t a[3] =	{0xffffffff, 0x00000000, 0xffffffff};
	uint32_t b[3];
	
	printf("Arithmetic shift tests:\n\n");	

	// Print a
	printf("a = "); 
	testprint(3, a);
	printf("\n\n");	

	// shiftr tests
	printf("Tests for shiftr: \n");
	
	// a >> 0 
	shiftr(3, a, 0, b);
	printf("a >> 0 = 0xFFFFFFFF 00000000 FFFFFFFF \n");
	printf("Result: ");
	testprint(3,b);	
	printf("\n\n");

	// a >> 4 
	shiftr(3, a, 4, b);
	printf("a >> 4 = 0x0FFFFFFF F0000000 0FFFFFFF \n");
	printf("Result: ");
	testprint(3, b);
	printf("\n\n");
	
	// a >> 36
	shiftr(3, a, 36, b);
	printf("a >> 36 = 0x00000000 0FFFFFFF F00000000 \n");
	printf("Result: ");
	testprint(3, b);
	printf("\n\n");

	// a >> 33
	shiftr(3, a, 33, b);
	printf("a >> 33 = 0x00000000 7FFFFFFF 800000000 \n");
	printf("Result: ");
	testprint(3, b);
	printf("\n\n");

	// shiftl tests
	// TODO these tests may be useless...
	printf("Tests for shiftl: \n");
	
	// a >> 0 
	shiftl(3, a, 0, 3, b);
	printf("a << 0 = 0xFFFFFFFF 00000000 FFFFFFFF \n");
	printf("Result: ");
	testprint(3,b);	
	printf("\n\n");

	// a >> 4 
	shiftl(3, a, 4, 3, b);
	printf("a << 4 = 0xFFFFFFF0 0000000F FFFFFFF0 \n");
	printf("Result: ");
	testprint(3, b);
	printf("\n\n");
	
	// a >> 36
	shiftl(3, a, 36, 3, b);
	printf("a << 36 = 0x0000000F FFFFFFF0 000000000 \n");
	printf("Result: ");
	testprint(3, b);
	printf("\n\n");

	// a >> 33
	shiftl(3, a, 33, 3, b);
	printf("a << 33 = 0x00000001 FFFFFFFE 000000000 \n");
	printf("Result: ");
	testprint(3, b);
	printf("\n\n");
}
