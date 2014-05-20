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
	printf("test");
	
	uint32_t ws;	// Word internal shift value
	uint32_t i;	// loopctr
	while(k > 0){	

		if ( k > 32 ){
		    ws = 32;
		    k -= 32;
		} else {
		    ws = k;
		    k = 0;
		}	  
		
		b[t-1] = a[t-1] >> ws; // Most significant uint32_t field does not have "underflow" 
		for(i = t - 2; i >= 0; i--){
		      b[i] = (a[i+1] << (32 - ws) || a[i] >> ws);			    
		}
	}
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
 *   result vector b
 * 
 * RETURN
 *   -
 * 
 * DESCRIPTION/REMARKS
 *  Tests both shift functions 
 * 
 * EXAMPLE
 * 	 a = x^2 + x^1 + 1 => [00000111,0,0,0]
 *   k = 1
 *	 f = x^3 + 1
 *
 *	 result = a << k mod f = ...?
 *
 */
void shiftTEST() {
	// TODO
	uint32_t a[3] =	{0xffffffff, 0x0000000f, 0x00000000};
	uint32_t b[3];
	
	printf("a = "); 
	testprint(3, a);
	printf("\n");
	shiftr(3, a, 2, b);
	printf("a >> 2 = "); 
	testprint(3, b);
	printf("\n");
	shiftr(3, a, 35, b);
	printf("a >> 35 = ");
	testprint(3, b);
	printf("\n");
}
