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
 * Name, First Name, Matr.-Nr
 * Name, First Name, Matr.-Nr
 * Letzkus, Fabian, 1480247 (TU Darmstadt)
 *
 */
 
#include <stdio.h>
#include <stdint.h>
#include "binops.c"
#include "add.c"
#include "shift.c"
#include "inv.c"
#include "mult.c"
#include "mod.c"

#define INFINITELEMENT 0xFFFFFFFF

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
	uint32_t *cxP = (uint32_t *) malloc(sizeof(uint32_t)*(l+1));
	memcpy (cxP, xP, (sizeof(uint32_t) * (l))); // Copy of xP
	uint32_t *cyP = (uint32_t *) malloc(sizeof(uint32_t)*(l+1));
	memcpy (cyP, yP, (sizeof(uint32_t) * (l))); // Copy of yP
	uint32_t *cxQ = (uint32_t *) malloc(sizeof(uint32_t)*(l+1));
	memcpy (cxQ, xQ, (sizeof(uint32_t) * (l))); // Copy of xQ
	uint32_t *cyQ = (uint32_t *) malloc(sizeof(uint32_t)*(l+1));
	memcpy (cyQ, yQ, (sizeof(uint32_t) * (l))); // Copy of yQ

	uint32_t *temp = (uint32_t *) malloc(sizeof(uint32_t)*(l+1));		
	uint32_t *temp2 = (uint32_t *) malloc(sizeof(uint32_t)*(l+1));
	uint32_t *temp3 = (uint32_t *) malloc(sizeof(uint32_t)*(l+1));
	memcpy (temp3, xP, (sizeof(uint32_t) * (l))); // temp2 = xP (makes sure that (xP,yP) may be (xR,yR) when calling dbl	
	
	// Check if P = +-Q => P+Q = infinit element
	add(l,xQ,yQ,temp);
	int i;
	if(f2m_is_equal(l,xP,xQ) && (f2m_is_equal(l,yP,yQ) || f2m_is_equal(l,yP,temp))){
		// Return neutral element
		for(i = 0; i < l; i++){
			xR[i] = INFINITELEMENT;
			yR[i] = INFINITELEMENT;
		}
	}

	// Calculate xR
	////////////////
	
	// Calculate (yQ+yP)*(xQ+xP)^-1
	add(l,cyQ,cyP,temp);  // not needed anymore after this => throw it away
	add(l,cxQ,cxP,temp2); // Contains xP + xQ => keep for later use
	multInv(l,temp2,l,f,temp2); 	// Contains (xP + xQ)^-1
	mult(l,temp,l,temp2,temp); 	// Contains result of (yQ+yP)*(xQ+xP)^-1 TODO Binäre quadrierung von Polynomen für speedup

	// Calculate (yQ+yP)*(xQ+xP)^-1 + xP + xQ + a
	add(l,temp,xP,xR); 		// At this point xP MAY BE(!) overwritten if addP was called with (xP,yP) = (xR,yR)
	add(l,xR,cxQ,xR);
	add(l,xR,a,xR);

	// Caclculate final xR
	mult(l,temp,l,temp,temp2); 	// temp2 = ((yQ+yP)*(xQ+xP)^-1)^2 
	add(l,xR,temp2,xR);	
	
	// Calculate yR
	////////////////
	
	// !temp still contains (yQ+yP)*(xQ+xP)^-1!
	
	// Calculate temp * (xP + xR)
	add(l,temp3,xR,temp2);
	mult(l,temp,l,temp2,temp);

	// Calculate yR = temp + xR + yP
	add(l,temp,xR,temp);
	add(l,temp,cyP,yR);

	free(temp);
	free(temp2);
	free(temp3);
	free(cxP);
	free(cyP);
	free(cxQ);
	free(cyQ);
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


	uint32_t *temp = (uint32_t *) malloc(sizeof(uint32_t)*(l+1));		
	uint32_t *temp2 = (uint32_t *) malloc(sizeof(uint32_t)*(l+1));

	// Check if P == -P => R is infinite element TODO UNSER IF CORRECT
	add(l,xP,yP,temp);
	int i;
	if(f2m_is_equal(l,yP,temp)){
		// Return neutral element
		for(i = 0; i < l; i++){
			xR[i] = INFINITELEMENT;
			yR[i] = INFINITELEMENT;
		}
	}

	// Calclulate xR
	////////////////
	mult(l,xP,l,xP,temp); // temp = xp^2 
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
	
	free(temp);
	free(temp2);
	free(cxP);
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
	
	// Make sure Point P is inside eliptic curve
	uint32_t *cxP = (uint32_t *) calloc(11, sizeof(uint32_t));		
	memcpy (cxP, xP, (sizeof(uint32_t) * (6))); // cxP = xP
	uint32_t *cyP = (uint32_t *) calloc(11, sizeof(uint32_t));		
	memcpy (cyP, yP, (sizeof(uint32_t) * (6))); // cyP = yP	
	
	//mod_f163(cxP);
	//mod_f163(cyP);

	// Implements the double and add method
	for(i = 0; i < (32*6); i++){
		// Double
		dbl(6, F, b, xQ, yQ, xQ, yQ);

		if((d[i/32]>>(i%32))&0x1){
			// Add
			addP(6, F, a, xQ, yQ, cxP, cyP, xQ, yQ); // TODO unklar wie rum P und Q!
			//addP(6, F, a, cxP, cyP, xQ, yQ, xQ, yQ); // TODO unklar wie rum P und Q!
		} 
	}
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
	mult_scalar(m,f,a,b,d,xP,yP,xQ,yQ); // t is overwritten in here oO
	printf("d  = ");f2m_print(6,d); printf("\n");// TODO t
	printf("xQ = ");f2m_print(6,xQ); printf("\n"); // TODO t
	printf("yQ = ");f2m_print(6,yQ); printf("\n"); // TODO t
		
	printf("************************************************************\n");
	printf("generate random scalar d and challenge C\n");
	printf("with C = k * P\n");
	f2m_rand(6,m,k); // TODO t
	mult_scalar(m,f,a,b,k,xP,yP,xC,yC);
	printf("k  = ");f2m_print(6,k); printf("\n"); // TODO t
	printf("xC = ");f2m_print(6,xC); printf("\n");// TODO t
	printf("yC = ");f2m_print(6,yC); printf("\n");// TODO t

	printf("************************************************************\n");
	printf("generate response R with R = d * C = d * k * P \n");
	mult_scalar(m,f,a,b,d,xC,yC,xR,yR);
	printf("xR = ");f2m_print(6,xR); printf("\n"); // TODO t
	printf("yR = ");f2m_print(6,yR); printf("\n");// TODO t

	printf("************************************************************\n");
	printf("generate verification point V with V = k * Q = k * d * P\n");
	mult_scalar(m,f,a,b,k,xQ,yQ,xV,yV);
	printf("xV = ");f2m_print(6,xV); printf("\n");// TODO t
	printf("yV = ");f2m_print(6,yV); printf("\n");// TODO t
	  if (!f2m_is_equal(6,xV,xR) || !f2m_is_equal(6,yV,yR)) return 1;// TODO t
  }
  printf("************************************************************\n");
  printf("test scalar multiplications...\n");
  for (i = 0; i < 10000; i++) mult_scalar(m,f,a,b,n,xP,yP,xQ,yQ);
  return 0;
}

/* 
 * FUNCTION
 *   main 
 */
int main(void)
{
  srand(1);
  printf("\ntest_ecc_b163: %d\n",test_ecc_b163());
  return 0;
}
