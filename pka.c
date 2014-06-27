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

#define INFINITELEMENT 0xFFFFFFFF

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
 *   isNeutral
 *
 * INPUT
 *   + length of vector v 
 *   + x-coordinate xP of point P
 *   + y-coordinate yP of point P
 *
 * OUTPUT
 *   -
 *
 * RETURN
 *   1 if the given vector is the neutral Element
 *   0 if the given vector is not the neutral Element
 *
 * DESCRIPTION/REMARKS
 *   The function calculates point R = Q + P
 */
int isNeutral(uint32_t l, uint32_t *xV, uint32_t *yV){
	int i;
	//uint32_t mask = 0xFFFFFFFF;

	for(i = 0; i < l; i++){
		if(xV[i]!=INFINITELEMENT || yV[i]!=INFINITELEMENT)
			return 0;
	}

	return 1;
}


uint32_t cxP[6];
//uint32_t cyP[6];

/*
 * FUNCTION
 *   add
 *
 * INPUT
 *   + length of vectors 
 *   + irreducible polynom F to generate the finite field
 *   + elliptic curve parameter a
 *   + x-coordinate xP of point P
 *   + y-coordinate yP of point P
 *   + x-coordinate xP of point Q
 *   + y-coordinate yP of point Q
 *
 * OUTPUT
 *   + x-coordinate xR of point R
 *   + y-coordinate yR of point R
 *
 * RETURN
 *   -
 *
 * DESCRIPTION/REMARKS
 *   The function calculates point R = Q + P
 */
void addP(uint32_t l, uint32_t *f, uint32_t *a, uint32_t *xP, uint32_t *yP, uint32_t *xQ, uint32_t *yQ, uint32_t *xR, uint32_t *yR){

	// Handle Neutral Element case
	if(isNeutral(l,xP,yP)){
		memcpy (xR, xQ, (sizeof(uint32_t) * (l)));
		memcpy (yR, yQ, (sizeof(uint32_t) * (l)));
		return;
	} else if (isNeutral(l,xQ,yQ)) {
		memcpy (xR, xP, (sizeof(uint32_t) * (l)));
		memcpy (yR, yP, (sizeof(uint32_t) * (l)));
		return;
	}	

	// Create Copies of all variables that are used
	//uint32_t *cxP = (uint32_t *) malloc(sizeof(uint32_t)*(l+1));
	memcpy (cxP, xP, (sizeof(uint32_t) * (l))); // Copy of xP
	//uint32_t *cyP = (uint32_t *) malloc(sizeof(uint32_t)*(l+1));
	//memcpy (cyP, yP, (sizeof(uint32_t) * (l))); // Copy of yP
	
	//uint32_t *temp = (uint32_t *) malloc(sizeof(uint32_t)*(l+1));		
	//uint32_t *temp2 = (uint32_t *) malloc(sizeof(uint32_t)*(l+1));
	//uint32_t *temp3 = (uint32_t *) malloc(sizeof(uint32_t)*(l+1));
	
	// Check if P = +-Q => P+Q = infinit element // TODO testen
	add(l,xQ,yQ,temp);
	int i;
	if(f2m_is_equal(l,xP,xQ) && (f2m_is_equal(l,yP,yQ) || f2m_is_equal(l,yP,temp))){
		// Return neutral element
		for(i = 0; i < l; i++){
			xR[i] = INFINITELEMENT;
			yR[i] = INFINITELEMENT;
		}
		return;
	}

	// Calculate xR
	////////////////
	
	// Calculate (yQ+yP)*(xQ+xP)^-1
	add(l,yQ,yP,temp);  // not needed anymore after this => throw it away
	add(l,xQ,cxP,temp3); // Contains xP + xQ => keep for later use
	multInv(l,temp3,l,f,temp2); 	// Contains (xP + xQ)^-1
	mult(l,temp,l,temp2,temp); 	// Contains result of (yQ+yP)*(xQ+xP)^-1 

	// Calculate (yQ+yP)*(xQ+xP)^-1 + xP + xQ + a
	//add(l,temp,xP,xR); 		// At this point xP MAY BE(!) overwritten if addP was called with (xP,yP) = (xR,yR)
	//add(l,xR,cxQ,xR);
	add(l,temp,temp3,xR);	
	add(l,xR,a,xR);

	// Caclculate final xR
	square(l, temp, temp2); //mult(l,temp,l,temp,temp2); 	// temp2 = ((yQ+yP)*(xQ+xP)^-1)^2 TODO Binäre quadrierung von Polynomen für speedup
	add(l,xR,temp2,xR);	
	
	// Calculate yR
	////////////////
	
	// !temp still contains (yQ+yP)*(xQ+xP)^-1!
	
	// Calculate temp * (xP + xR)
	add(l,cxP,xR,temp2);
	mult(l,temp,l,temp2,temp);

	// Calculate yR = temp + xR + yP
	add(l,temp,xR,temp);
	add(l,temp,yP,yR);

	//free(temp);
	//free(temp2);
	//free(temp3);
	//free(cxP);
	//free(cyP);
}

/*
 * FUNCTION
 *   dbl
 *
 * INPUT
 *   + length of vectors 
 *   + irreducible polynom F to generate the finite field
 *   + elliptic curve parameter b
 *   + x-coordinate xP of point P
 *   + y-coordinate yP of point P
 *
 * OUTPUT
 *   + x-coordinate xR of point R
 *   + y-coordinate yR of point R
 *
 * RETURN
 *   -
 *
 * DESCRIPTION/REMARKS
 *   The function calculates the point R = 2P
 */
void dbl(uint32_t l, uint32_t *f, uint32_t *b, uint32_t *xP, uint32_t *yP, uint32_t *xR, uint32_t *yR){
	// Is Q neutral element?
	if(isNeutral(l,xP,yP))
		return;	// Assumes that dbl call has (xP,yP) = (xR,yR)!!!

	// Create copy of xP and yP to make sure (xP,yP) may be (xR,yR)
	uint32_t *cxP = (uint32_t *) malloc(sizeof(uint32_t)*(l+1));
	memcpy (cxP, xP, (sizeof(uint32_t) * (l))); // Copy of xP
	//uint32_t *cyP = (uint32_t *) malloc(sizeof(uint32_t)*(l+1));
	//memcpy (cyP, yP, (sizeof(uint32_t) * (l))); // Copy of yP


	//uint32_t *temp = (uint32_t *) malloc(sizeof(uint32_t)*(l+1));		
	//uint32_t *temp2 = (uint32_t *) malloc(sizeof(uint32_t)*(l+1));
	
	// Check if P == -P => R is infinite element TODO UNSER IF CORRECT
	add(l,xP,yP,temp);
	int i;
	if(f2m_is_equal(l,yP,temp)){
		// Return neutral element
		for(i = 0; i < l; i++){
			xR[i] = INFINITELEMENT;
			yR[i] = INFINITELEMENT;
		}
		return;
	}

	// Calclulate xR
	////////////////
	square(l, xP, temp); //mult(l,xP,l,xP,temp); // temp = xp^2 // TODO Polynomquadrierung 
	multInv(l,temp,l,f,xR); // xR = temp^-1 = x^2^-1
	mult(l,xR,l,b,xR); // xR = b * xP^2^-1
	add(l,xR,temp,xR); // xR = xR + xP^2

	// Calclulate yR
	////////////////
	add(l,temp,xR,temp); // temp = xp^2 + xR
	multInv(l,cxP,l,f,temp2); // temp2 = xP^-1
	mult(l,temp2,l,yP,yR); // yR = yP * xP^-1
	add(l,yR,cxP,yR); // => yR = (xP + yP * xP^-1)
	mult(l,yR,l,xR,yR); // yR = (xP + yP * xP^-1) * xR
	add(l,yR,temp,yR); // yR = (xP + yP * xP^-1) * xR + xp^2 + xR
	
	//free(temp);
	//free(temp2);
	free(cxP);
}

// TODO Point doubleing on projective coordinates for Montgomery
void pdbl(uint32_t *b, uint32_t *XP, uint32_t *ZP, uint32_t *XS, uint32_t *ZS){

}

// TODO Point adding on projective coordinates for Montgomery
void padd(uint32_t *xD, uint32_t *XP, uint32_t *ZP, uint32_t *XQ, uint32_t *ZQ, uint32_t *XR, uint32_t *ZR){

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
/*void mult_scalar(
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
	

	// Q = neutral element
	for(i = 0; i < 6; i++){
		xQ[i] = INFINITELEMENT;
		yQ[i] = INFINITELEMENT;
	}
	
	// Implements the double and add method
	for(i = deg(6,d); i >= 0; i--){
		// Double
		dbl(6, F, b, xQ, yQ, xQ, yQ);
		//dbls++; // TODO DEBUG REMOVE
		if((d[i/32]>>(i%32))&0x1){ // (d[i/32] &(0x1 << (i%32)))
			// Add
			addP(6, F, a, xQ, yQ, xP, yP, xQ, yQ);
			//adds++; // TODO DEBUG REMOVE
		} 
	}

	//printf("Dbl Operations: %d \t Add Operations: %d \n", dbls, adds); // TODO REMOVE DEBUG 
}*/
 

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
 
	// Q = neutral element
	for(i = 0; i < 6; i++){
		xQ[i] = INFINITELEMENT;
		yQ[i] = INFINITELEMENT;
	}

	// Implements the double and add method
	for(i = deg(6,d); i >= 0; i--){
		// Double
		dbl(6, F, b, xQ, yQ, xQ, yQ);
		if((d[i/32]>>(i%32))&0x1){ // (d[i/32] &(0x1 << (i%32)))
			// Add
			addP(6, F, a, xQ, yQ, xP, yP, xQ, yQ);
		} 
	}

	//printf("Dbl Operations: %d \t Add Operations: %d \n", dbls, adds); // TODO REMOVE DEBUG 
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
	printf("yF = "); f2m_print(6,yQ);printf("\n");    	
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
	//scalarMultTest();
	srand(1);
  	printf("\ntest_ecc_b163: %d\n",test_ecc_b163());
  	return 0;
}
