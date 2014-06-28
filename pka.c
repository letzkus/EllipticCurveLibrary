/* 
 * LECTURE 
 *   Implementierung moderner Public-Key-Algorithmen
 *   by Michael Braun, Wintersemester 2013/2014
 *
 * PRACTICAL
 *   Implementation of an Ansi C library for elliptic curves 
 *   over binary fields.
 *
 * TEAM
 * Letzkus, Fabian, 1480247 (TU Darmstadt)
 * Springer, Markus, 1401899 (TU Darmstadt)
 */
 
#include <stdio.h>
#include <stdint.h>
#include "add.c"
#include "shift.c"
#include "inv.c"
#include "mult.c"
#include "mod.c"
#include "quad.c"

// Temporary variables for later use
uint32_t temp[6];
uint32_t temp2[6];
uint32_t temp3[6];

/*
 * FUNCTION
 *   f2m_rand
 *
 * INPUT
 *   + length t of array A
 *   + bit length m of value represented by A
 *
 * OUTPUT
 *   + random m-bit value in array A
 * 
 * RETURN
 *   -
 * 
 * DESCRIPTION/REMARKS
 *   The random number generator "rand()" is used. The memory of A must
 *   already be allocated before the function is called.
 */
void f2m_rand(
  uint32_t t,
  uint32_t m,
  uint32_t *A
)
{
  uint32_t i;
  for (i = 0; i < t-1; i++) A[i] = rand();
  A[t-1] = rand() & (0xFFFFFFFF >> (32 - m % 32));
}

/*
 * FUNCTION
 *   f2m_print
 *
 * INPUT
 *   + length t of array A
 *   + array A 
 *
 * OUTPUT
 *   -
 * 
 * RETURN
 *   -
 * 
 * DESCRIPTION/REMARKS
 *   The function prints the array A in hexadecimal representation
 *   onto the sceen. The least significant bit is aligned to the
 *   right hand side.
 */
void f2m_print(
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
 *   f2m_is_equal
 *
 * INPUT
 *   + length t of all arrays
 *   + array A 
 *   + array B
 * 
 * OUTPUT
 *   -
 *
 * RETURN 
 *   + 1 (=true) if the content of A and B is equal
 *   + 0 (=false) otherwise
 *
 * DESCRIPTION/REMARKS
 *   -
 */
uint32_t f2m_is_equal(
  uint32_t t,
  uint32_t *A,
  uint32_t *B
)
{
  uint32_t i;
  for (i = 0; i < t; i++) if (A[i] != B[i]) return 0;
  return 1;
}

/*
 * FUNCTION
 *   pdbl
 *
 * INPUT
 *   + elliptic curve parameter b
 *   + projective x-coordinate XP of point P
 *   + projective Z-Value ZP of point P
 *
 * OUTPUT
 *   + projective x-coordinate XS of point S
 *   + projective Z-Value ZS of point S
 *
 * RETURN
 *   -
 *
 * DESCRIPTION/REMARKS
 *   Implements the doubling of point P for projective coordinates
 */
void pdbl(uint32_t *b, uint32_t *XP, uint32_t *ZP, uint32_t *XS, uint32_t *ZS){
	square(6,XP,temp); 	// temp = XP^2
	square(6,temp,temp2); 	// temp2 = XP^4
	square(6,ZP,temp3);	// temp3 = ZP^2

	// ZS
	mult(6,temp,6,temp3,ZS); 	// ZS = XP^2 + ZP^2

	// XS
	square(6,temp3,temp3);	// temp3 = ZP^4
	mult(6,temp3,6,b,temp3);// temp3 = ZP^4 * b
	add(6,temp2,temp3,XS); 	// XS = XP^4 + ZP^4
}

/*
 * FUNCTION
 *   padd
 *
 * INPUT
 *   + x-coordinate xP of point P
 *   + projective x-coordinate XP of point P
 *   + projective Z-Value ZP of point P
 *   + projective x-coordinate XQ of point Q
 *   + projective Z-Value ZQ of point Q
 *
 * OUTPUT
 *   + projective x-coordinate XR of point R
 *   + projective Z-Value ZR of point R
 *
 * RETURN
 *   -
 *
 * DESCRIPTION/REMARKS
 *   Implements the addition of points P and Q for projective coordinates
 */
void padd(uint32_t *xD, uint32_t *XP, uint32_t *ZP, uint32_t *XQ, uint32_t *ZQ, uint32_t *XR, uint32_t *ZR){
	mult(6,XQ,6,ZP,temp); 	// temp = XQ * ZP
	mult(6,XP,6,ZQ,temp2);	// temp2 = XP * ZQ
	mult(6,temp,6,temp2,temp3);	// temp3 = XP * ZQ * XQ * ZP
	
	// ZR
	add(6,temp,temp2,ZR);
	square(6,ZR,ZR);	// ZR = (XQ * ZP + XP * ZQ)^2

	// XR
	mult(6,ZR,6,xD,XR); 	// XR = xD * (XQ * ZP + XP * ZQ)^2
	add(6,XR,temp3,XR);	// XR = xD * (XQ * ZP + XP * ZQ)^2 + (XP * ZQ * XQ * ZP) 
}

/*
 * FUNCTION
 *   Convert
 *
 * INPUT
 *   + x-coordinate xP of point P
 *   + y-coordinate yP of point P
 *   + irreducible polynom F to generate the finite field
 *   + projective x-coordinate X_Q of point Q
 *   + projective Z-Value Z_Q of point Q
 *   + projective x-coordinate X_R of point R
 *   + projective Z-Value Z_R of point R
 *
 * OUTPUT
 *   + x-coordinate xQ of point Q
 *   + y-coordinate yQ of point Q
 *
 * RETURN
 *   -
 *
 * DESCRIPTION/REMARKS
 *   Converts the projective coordinates of point Q = kP
 */
void convert(uint32_t *xP, uint32_t *yP, uint32_t *F, uint32_t *X_Q, uint32_t *Z_Q, uint32_t *X_R, uint32_t *Z_R, uint32_t *xQ, uint32_t *yQ){  
	// Check if R is infinite element and convert accordingly...
	int i;	
	int zq = 1;
	int zr = 1;
	for(i = 0; i < 6; i++){
		if(Z_R[i] != 0x00000000){
			zr = 0;
		}
		if(Z_Q[i] != 0x00000000){
			zq = 0;
		}
	}	

	// Reminder: Q = kP, R = (k+1)P
	// xQ 
	if(zq){	
		//Q is infinite element
		for(i = 0; i < 6; i++){
			xQ[i] = 0xFFFFFFFF;
			yQ[i] = 0xFFFFFFFF;
		}	

		return;
	} else {
		// xQ is not the finite element
		multInv(6,Z_Q,6,F,xQ);  
		mult(6,xQ,6,X_Q,xQ); 
	}

	// yQ
	// Check if R is the infinite element
	// If that is the case then 	
	if(zr){
		// This means that Q is -P
		add(6,xP,yP,yQ);
	} else {
		add(6,xP,xQ,temp); // temp = (xkp + xp)

		multInv(6,Z_R,6,F,temp2);	// temp2 = Zk+1p^-1 
		mult(6,temp2,6,X_R,temp2); 	// temp2 = x_(k+1)p 
		
		add(6,temp2,xP,temp2); 		// temp2 = (x(k+1p) + xkp)	
		
		mult(6,temp,6,temp2,temp2); 	// temp2 = (xkp + xp)(xk+1p + xkp)
		square(6,xP,temp3); 		// temp3 = xp^2
		add(6,temp2,temp3,temp2); 	// temp2 = xp^2 + (xkp + xp)(xk+1p + xkp)
		add(6,temp2,yP,temp2); 		// temp2 = yp + xp^2 + (xkp + xp)(xk+1p + xkp)
	
		mult(6,temp,6,temp2,temp);	// temp = temp2 * (xkp + xp)
		multInv(6,xP,6,F,temp2);	// temp2 = xP^-1
		mult(6,temp,6,temp2,temp);

		add(6,temp,yP,yQ);
	}
}

/*
 * FUNCTION
 *   mult_scalar
 *
 * INPUT
 *   + extension degree m of the binary field 
 *   + irreducible polynom F to generate the finite field
 *   + elliptic curve parameter a
 *   + elliptic curve parameter b
 *   + scalar d with maximum bitlength m
 *   + x-coordinate xP of point P
 *   + y-coordinate yP of point P
 *
 * OUTPUT
 *   + x-coordinate xQ of point Q
 *   + y-coordinate yQ of point Q
 *
 * RETURN
 *   -
 *
 * DESCRIPTION/REMARKS
 *   The function calculates the point Q = dP
 */
uint32_t X_Q[6];
uint32_t Z_Q[6];
uint32_t X_R[6];
uint32_t Z_R[6];

void mult_scalar(
  uint32_t m,
  uint32_t *F,
  uint32_t *a,
  uint32_t *b,
  uint32_t *d,
  uint32_t *xP,
  uint32_t *yP,
  uint32_t *xQ,
  uint32_t *yQ  
)
{
	// Initialize
	int i;	
	int z = 1;
	
	for(i = 0; i < 6; i++){
		X_Q[i] = 0;
		Z_Q[i] = 0;
		X_R[i] = xP[i];
		Z_R[i] = 0;
	}

	X_Q[0] = 1;
	Z_R[0] = 1;
	

	// Check if d is 0, return infinite element then
	for(i = 0; i < 6; i++){
		if(d[i] != 0x00000000){
			z = 0;	
		}
		xQ[i] = 0xFFFFFFFF;
		yQ[i] = 0xFFFFFFFF;
	}

	// Return if d was 0
	if(z) return;
	
	// Implements montgomery point multiplication
	for(i = deg(6,d); i >= 0; i--){
		if((d[i/32]>>(i%32))&0x1){ // (d[i/32] &(0x1 << (i%32)))
			padd(xP, X_Q, Z_Q, X_R, Z_R, X_Q, Z_Q);			
			pdbl(b, X_R, Z_R, X_R, Z_R);			
		} else {
			padd(xP, X_R, Z_R, X_Q, Z_Q, X_R, Z_R);			
			pdbl(b, X_Q, Z_Q, X_Q, Z_Q);	
		}
	}

	// Calculate final result
	convert(xP, yP, F, X_Q, Z_Q, X_R, Z_R, xQ, yQ);
}

/* 
 * FUNCTION 
 *   test_ecc_b163
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
uint32_t test_ecc_b163()
{
  uint32_t 
		
  m = 163, /* extension degree of binary field */
  t = 6, /* number of 32-bit words needed to store finite field element */
		
  i, /* iteration variable */
		
  xQ[6], /* public key Q */
  yQ[6],
  d[6], /* private key d */
		
  xC[6], /* challenge C */
  yC[6],
  k[6], /* random scalar for challenge generation */
		
  xR[6], /* response R */
  yR[6],
		
  xV[6], /* verification point C */
  yV[6],
	
  f[6] = {0x000000C9, 0x00000000, 0x00000000, 
		  0x00000000, 0x00000000, 0x00000008}, /* irreducible polynomial */
	  
  a[6] = {0x00000001, 0x00000000, 0x00000000, 
		  0x00000000, 0x00000000, 0x00000000}, /* ec parameter a */
		
  b[6] = {0x4A3205FD, 0x512F7874, 0x1481EB10,
          0xB8C953CA, 0x0A601907, 0x00000002}, /* ec parameter b */ 
		
  xP[6] = {0xE8343E36, 0xD4994637, 0xA0991168,
           0x86A2D57E, 0xF0EBA162, 0x00000003}, /* x-coord. of base point */
						 
  yP[6] = {0x797324F1, 0xB11C5C0C, 0xA2CDD545,
           0x71A0094F, 0xD51FBC6C, 0x00000000}, /* y-coord. of base point */
						 
  n[6] = {0xA4234C33, 0x77E70C12, 0x000292FE, 
          0x00000000, 0x00000000, 0x00000004}; /* order of base point */ 
		
  printf("\n************************************************************\n");
  printf("test: scalar multiplication of EC over GF(2^163)\n");
  printf("\nirreducible polynomial to generate GF(2^163)\n");
  printf("f  = ");f2m_print(t,f); printf("\n");
  printf("\nparameter b to determine elliptic curve E of GF(2^163)\n");
  printf("E: y^2 + xy = x^3 + ax^2 + b\n");
  printf("a  = ");f2m_print(t,a); printf("\n");
  printf("b  = ");f2m_print(t,b); printf("\n");
  printf("\nbase point P\n");
  printf("xP = ");f2m_print(t,xP); printf("\n");
  printf("yP = ");f2m_print(t,yP); printf("\n");
  printf("\norder of base point P\n");
  printf("n  = ");f2m_print(t,n); printf("\n");

  for (i = 0; i < 10; i++) {
    printf("************************************************************\n");
    printf("test %d\n",i);	
    printf("************************************************************\n");
	printf("generate random private key d and corresponding\n");
	printf("public key Q with Q = d * P\n");
	f2m_rand(t,m,d);
	mult_scalar(m,f,a,b,d,xP,yP,xQ,yQ);
	printf("d  = ");f2m_print(t,d); printf("\n");
	printf("xQ = ");f2m_print(t,xQ); printf("\n");
	printf("yQ = ");f2m_print(t,yQ); printf("\n");
		
	printf("************************************************************\n");
	printf("generate random scalar d and challenge C\n");
	printf("with C = k * P\n");
	f2m_rand(6,m,k); 
	mult_scalar(m,f,a,b,k,xP,yP,xC,yC);
	printf("k  = ");f2m_print(t,k); printf("\n");
	printf("xC = ");f2m_print(t,xC); printf("\n");
	printf("yC = ");f2m_print(t,yC); printf("\n");

	printf("************************************************************\n");
	printf("generate response R with R = d * C = d * k * P \n");
	mult_scalar(m,f,a,b,d,xC,yC,xR,yR);
	printf("xR = ");f2m_print(t,xR); printf("\n");
	printf("yR = ");f2m_print(t,yR); printf("\n");

	printf("************************************************************\n");
	printf("generate verification point V with V = k * Q = k * d * P\n");
	mult_scalar(m,f,a,b,k,xQ,yQ,xV,yV);
	printf("xV = ");f2m_print(t,xV); printf("\n");
	printf("yV = ");f2m_print(t,yV); printf("\n");
	  if (!f2m_is_equal(t,xV,xR) || !f2m_is_equal(t,yV,yR)) return 1;
  }
  printf("************************************************************\n");
  printf("test scalar multiplications...\n");
  for (i = 0; i < 10000; i++){ 
  	mult_scalar(m,f,a,b,n,xP,yP,xQ,yQ);
  	//printf("Iteration %d finished \n",i);
  }

  return 0;
}


/*
Test data taken from following sources:

http://point-at-infinity.org/ecc/nisttv
http://csrc.nist.gov/groups/ST/toolkit/documents/dss/NISTReCur.pdf

The curve used for the tests is B163.
*/
void scalarMultTest(){
	//createTable();
	uint32_t 

	f[6] = {0x000000C9, 0x00000000, 0x00000000, 
			0x00000000, 0x00000000, 0x00000008}, // irreducible polynomial 
	  
	a[6] = {0x00000001, 0x00000000, 0x00000000, 
			0x00000000, 0x00000000, 0x00000000}, // ec parameter a 
		
  	b[6] = {0x4A3205FD, 0x512F7874, 0x1481EB10,
        		0xB8C953CA, 0x0A601907, 0x00000002}, // ec parameter b  
		
  	xP[6] = {0xE8343E36, 0xD4994637, 0xA0991168,
        		0x86A2D57E, 0xF0EBA162, 0x00000003}, // x-coord. of base point 
						 
  	yP[6] = {0x797324F1, 0xB11C5C0C, 0xA2CDD545,
        		0x71A0094F, 0xD51FBC6C, 0x00000000}, // y-coord. of base point
						 
	xQ[6], // x-coord. of result point
						 
  	yQ[6]; // y-coord. of result point

	// Test 0
	uint32_t dZ[6] = {0x0,0x0,0x0,0x0,0x0,0x0};	

	uint32_t xZ[6] = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
        		0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}; 
						 
  	uint32_t yZ[6] = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
        		0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
  	
    	printf("************************************************************\n");
    	printf("Test 0: \nk = 0\n");
	printf("xZ = "); f2m_print(6,xZ);printf("\n");	
	printf("yZ = "); f2m_print(6,yZ);printf("\n");    	
	printf("************************************************************\n");

	mult_scalar(163,f,a,b,dZ,xP,yP,xQ,yQ);
	
	printf("xZ = "); f2m_print(6,xQ);printf("\n");
	printf("yZ = "); f2m_print(6,yQ);printf("\n");
	
	if(f2m_is_equal(6,xQ,xZ) && f2m_is_equal(6,yQ,yZ))
		printf("Passed!\n\n");
	else
		printf("Error!\n\n");


	// Test 1
	uint32_t dO[6] = {0x1,0x0,0x0,0x0,0x0,0x0};	

	uint32_t xO[6] = {0xE8343E36, 0xD4994637, 0xA0991168,
        		0x86A2D57E, 0xF0EBA162, 0x00000003}; 
						 
  	uint32_t yO[6] = {0x797324F1, 0xB11C5C0C, 0xA2CDD545,
        		0x71A0094F, 0xD51FBC6C, 0x00000000};
  	
    	printf("************************************************************\n");
    	printf("Test 1: \nk = 1\n");
	printf("xO = "); f2m_print(6,xO);printf("\n");	
	printf("yO = "); f2m_print(6,yO);printf("\n");    	
	printf("************************************************************\n");

	mult_scalar(163,f,a,b,dO,xP,yP,xQ,yQ);
	
	printf("xO = "); f2m_print(6,xQ);printf("\n");
	printf("yO = "); f2m_print(6,yQ);printf("\n");
	
	if(f2m_is_equal(6,xQ,xO) && f2m_is_equal(6,yQ,yO))
		printf("Passed!\n\n");
	else
		printf("Error!\n\n");

	
	// Test 2
	uint32_t dF[6] = {0x5,0x0,0x0,0x0,0x0,0x0};	

	uint32_t xF[6] = {0xA449F864, 0x2764867D, 0x4C657BB5,
        		0x3630522F, 0x20589968, 0x00000007}; 
						 
  	uint32_t yF[6] = {0x3550CB9C, 0x007AF301, 0xDB01CA79,
        		0x5DADA096, 0x02537FF5, 0x00000003};
  	
    	printf("************************************************************\n");
    	printf("Test 2: \nk = 5\n");
	printf("xF = "); f2m_print(6,xF);printf("\n");	
	printf("yF = "); f2m_print(6,yF);printf("\n");    	
	printf("************************************************************\n");

	mult_scalar(163,f,a,b,dF,xP,yP,xQ,yQ);
	
	printf("xF = "); f2m_print(6,xQ);printf("\n");
	printf("yF = "); f2m_print(6,yQ);printf("\n");
	
	if(f2m_is_equal(6,xQ,xF) && f2m_is_equal(6,yQ,yF))
		printf("Passed!\n\n");
	else
		printf("Error!\n\n");

	
	// Test 3
	uint32_t dT[6] = {0xA,0x0,0x0,0x0,0x0,0x0};

	uint32_t xT[6] = {0xCAA10915, 0x3C9313E7, 0x6914C218,
        		0x0F581B0D, 0x07E54141, 0x00000005}; 
						 
  	uint32_t yT[6] = {0x22748341, 0x97E25F1B, 0xD20961BB,
        		0x69D3EFDB, 0x03C6D2DE, 0x00000003};
	
    	printf("************************************************************\n");
    	printf("Test 3: \nk = 10\n");	
	printf("xT = "); f2m_print(6,xT);printf("\n");
	printf("yT = "); f2m_print(6,yT);printf("\n");    	
	printf("************************************************************\n");
	
	mult_scalar(163,f,a,b,dT,xP,yP,xQ,yQ);
	
	printf("xT = "); f2m_print(6,xQ);printf("\n");
	printf("yT = "); f2m_print(6,yQ);printf("\n");
	
	if(f2m_is_equal(6,xQ,xT) && f2m_is_equal(6,yQ,yT))
		printf("Passed!\n\n");
	else
		printf("Error!\n\n");

	// Test 4
	uint32_t dB[6] = {0x5EED0E13,0x018EBBB9,0x0,0x0,0x0,0x0};
	//uint32_t dB[6] = {0x66778899,0x22334455,0x11,0x0,0x0,0x0};


	uint32_t xB[6] = {0x6082CBFE, 0x2E96679C, 0x7BA6AE8A,
        		0x2F1E4614, 0xB78128BD, 0x00000004}; 

  	uint32_t yB[6] = {0x44824065, 0x9B242ECA, 0x2F911313,
        		0x212EE4A8, 0x69576EAA, 0x00000003};
	
    	printf("************************************************************\n");
    	printf("Test 4: \nk = 112233445566778899\n");	
	printf("xB = "); f2m_print(6,xB);printf("\n");
	printf("yB = "); f2m_print(6,yB);printf("\n");    	
	printf("************************************************************\n");
	
	mult_scalar(163,f,a,b,dB,xP,yP,xQ,yQ);
	
	printf("xB = "); f2m_print(6,xQ);printf("\n");
	printf("yB = "); f2m_print(6,yQ);printf("\n");
	
	if(f2m_is_equal(6,xQ,xB) && f2m_is_equal(6,yQ,yB))
		printf("Passed!\n\n");
	else
		printf("Error!\n\n");

	// Test 5
	uint32_t dVB[6] = {0xa4234c32,0x77e70c12,0x000292fe,
				0x00000000,0x00000000,0x00000004};

	uint32_t xVB[6] = {0xE8343E36, 0xD4994637, 0xA0991168,
        		0x86A2D57E, 0xF0EBA162, 0x00000003}; 

  	uint32_t yVB[6] = {0x91471AC7, 0x65851A3B, 0x0254C42D,
        		0xF702DC31, 0x25F41D0E, 0x00000003};
	
    	printf("************************************************************\n");
    	printf("Test 5: \nk = 5846006549323611672814742442876390689256843201586\n");	
	printf("xVB = "); f2m_print(6,xVB);printf("\n");
	printf("yVB = "); f2m_print(6,yVB);printf("\n");    	
	printf("************************************************************\n");
	
	mult_scalar(163,f,a,b,dVB,xP,yP,xQ,yQ);
	
	printf("xVB = "); f2m_print(6,xQ);printf("\n");
	printf("yVB = "); f2m_print(6,yQ);printf("\n");
	
	if(f2m_is_equal(6,xQ,xVB) && f2m_is_equal(6,yQ,yVB))
		printf("Passed!\n\n");
	else
		printf("Error!\n\n");
	
	// Test 6
	uint32_t dR[6] = {0xa4234c33,0x77e70c12,0x000292fe,
				0x00000000,0x00000000,0x00000004};

	uint32_t xR[6] = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
        		0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}; 

  	uint32_t yR[6] = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
        		0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
	
    	printf("************************************************************\n");
    	printf("Test 6: \nk = 5846006549323611672814742442876390689256843201587\n");	
	printf("xR = "); f2m_print(6,xR);printf("\n");
	printf("yR = "); f2m_print(6,yR);printf("\n");    	
	printf("************************************************************\n");
	
	mult_scalar(163,f,a,b,dR,xP,yP,xQ,yQ);
	
	printf("xR = "); f2m_print(6,xQ);printf("\n");
	printf("yR = "); f2m_print(6,yQ);printf("\n");
	
	if(f2m_is_equal(6,xQ,xR) && f2m_is_equal(6,yQ,yR))
		printf("Passed!\n\n");
	else
		printf("Error!\n\n");

}

/* 
 * FUNCTION
 *   main 
 */
int main(void)
{
  	createTable();
	scalarMultTest();
	srand(1);
  	printf("\ntest_ecc_b163: %d\n",test_ecc_b163());
  	return 0;
}
