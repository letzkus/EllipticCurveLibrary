#include <stdio.h>
#include <stdint.h>

#include "binops.c"
#include "add.c"
#include "mod.c"
#include "shift.c"
#include "inv.c"
#include "mult.c"
#include "quad.c"

//#include "pka.c"

/*
void f2m_print(
  uint32_t t,
	uint32_t *A
)
{ 
  uint32_t i;
  printf("0x");
  for (i = 0; i < t; i++) printf("%.8X ",A[t-i-1]);
}

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
*/

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
	f2m_print(3, a);
	printf("\n\n");	

	// shiftr tests
	printf("Tests for shiftr: \n");
	
	// a >> 0 
	shiftr(3, a, 0, b);
	printf("a >> 0 = 0xFFFFFFFF 00000000 FFFFFFFF \n");
	printf("Result: ");
	f2m_print(3,b);	
	printf("\n\n");

	// a >> 4 
	shiftr(3, a, 4, b);
	printf("a >> 4 = 0x0FFFFFFF F0000000 0FFFFFFF \n");
	printf("Result: ");
	f2m_print(3, b);
	printf("\n\n");
	
	// a >> 36
	shiftr(3, a, 36, b);
	printf("a >> 36 = 0x00000000 0FFFFFFF F00000000 \n");
	printf("Result: ");
	f2m_print(3, b);
	printf("\n\n");

	// a >> 33
	shiftr(3, a, 33, b);
	printf("a >> 33 = 0x00000000 7FFFFFFF 800000000 \n");
	printf("Result: ");
	f2m_print(3, b);
	printf("\n\n");

	// shiftl tests
	// TODO these tests may be useless...
	printf("Tests for shiftl: \n");
	
	// a >> 0 
	shiftl(3, a, 0, 3, b);
	printf("a << 0 = 0xFFFFFFFF 00000000 FFFFFFFF \n");
	printf("Result: ");
	f2m_print(3,b);	
	printf("\n\n");

	// a >> 4 
	shiftl(3, a, 4, 3, b);
	printf("a << 4 = 0xFFFFFFF0 0000000F FFFFFFF0 \n");
	printf("Result: ");
	f2m_print(3, b);
	printf("\n\n");
	
	// a >> 36
	shiftl(3, a, 36, 3, b);
	printf("a << 36 = 0x0000000F FFFFFFF0 000000000 \n");
	printf("Result: ");
	f2m_print(3, b);
	printf("\n\n");

	// a >> 33
	shiftl(3, a, 33, 3, b);
	printf("a << 33 = 0x00000001 FFFFFFFE 000000000 \n");
	printf("Result: ");
	f2m_print(3, b);
	printf("\n\n");
}

void invTest2(){
// Problematic value
//0x6040c0:	0xe8343e36	0xd4994637	0x797db89e	0x5b2a42dd
//0x6040d0:	0xf64d0d91	0x00000000	0x00000000	0x00000000
	printf("Tests for multiplicative inverse 2: \n");
	uint32_t f[6] = {0x000000C9, 0x00000000, 0x00000000, 
		  0x00000000, 0x00000000, 0x00000008}; 

	uint32_t a[10] = {0xe8343e36, 0xd4994637, 0x797db89e, 0x5b2a42dd, 0xf64d0d91,0x0,0x0,0x0,0x0,0x0};

	uint32_t c[10] = {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};

	uint32_t d[10] = {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};

	multInv(6,a,6,f,c);
	
	//mod_f163(c);	

	printf("Multiplicative inverse:\n");
	f2m_print(10,c);	

	printf("\n\na multiplied by multiplicative inverse:\n");
	
	mult(6,a,10,c,d);
	f2m_print(10,d);

	printf("\n\n");	
}

void invTest(){
	printf("Tests for multiplicative inverse: \n");
	uint32_t f[6] = {0x000000C9, 0x00000000, 0x00000000, 
		  0x00000000, 0x00000000, 0x00000008}; 

	uint32_t a[3] = {0xDEADBEEF, 0x00000000, 0x00000000};

	uint32_t c[10] = {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};

	uint32_t d[10] = {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};

	multInv(3,a,6,f,c);
	
	//mod_f163(c);	

	printf("Multiplicative inverse:\n");
	f2m_print(10,c);	

	printf("\n\na multiplied by multiplicative inverse:\n");
	
	mult(3,a,10,c,d);
	f2m_print(10,d);

	printf("\n\n");	
}

void testNumberOfOnes(){

	// Init
	int i;
	int ctr = 0;

	uint32_t d[6] = {0x80000003,0x80000010,0x80000001,0x80000001,0x80000001,0x80000001};

	// Implements the double and add method
	for(i = 0; i < (32*6); i++){
		if((d[i/32]>>(i%32))&0x1){
			// Add
			ctr++;
		} 
	}

	printf("Number of 1 = %d \n\n",ctr);
}

void testMultSpecific(){
	uint32_t a[6] = {0x8,0x0,0x0,0x0,0x0}; // x^3 
	
	uint32_t d[6] = {0x8,0x0,0x0,0x0,0x0};

	uint32_t b[6] = {0x10,0x0,0x0,0x0,0x0}; // x^4

	uint32_t c[10] = {0x0, 0x0, 0x0, 0x0, 0x0, 
				0x0, 0x0, 0x0, 0x0, 0x0};
	
	printf("Multiplication of polynomes tests:\n\n");

	// Test 1
	printf("Test 1: mult(l,a,l,a,c) = working?\n\n");
		
	mult(6,a,6,a,c);
	if(c[0] == 0x40 && c[1] == 0x0 && c[2] == 0x0 && c[3] == 0x0 && c[4] == 0x0 && c[5] == 0x0
		&& a[0] == 0x8 && a[1] == 0x0 && a[2] == 0x0 && a[3] == 0x0 && a[4] == 0x0 && a[5] == 0x0)
		printf("=> Works!");
	else
		printf("Error!");

	printf("\n\n");
	
	// Test 2
	printf("Test 2: mult(l,a,l,b,a) = working?\n\n");
		
	mult(6,a,6,b,a);
	if(a[0] == 0x80 && a[1] == 0x0 && a[2] == 0x0 && a[3] == 0x0 && a[4] == 0x0 && a[5] == 0x0
		&& b[0] == 0x10 && b[1] == 0x0 && b[2] == 0x0 && b[3] == 0x0 && b[4] == 0x0 && b[5] == 0x0)
		printf("=> Works!");
	else
		printf("Error!");

	printf("\n\n");

	// Test 3
	printf("Test 2: mult(l,a,l,b,b) = working?\n\n");
		
	mult(6,d,6,b,b);
	if(b[0] == 0x80 && b[1] == 0x0 && b[2] == 0x0 && b[3] == 0x0 && b[4] == 0x0 && b[5] == 0x0
		&& d[0] == 0x8 && d[1] == 0x0 && d[2] == 0x0 && d[3] == 0x0 && d[4] == 0x0 && d[5] == 0x0)
		printf("=> Works!");
	else
		printf("Error!");

	printf("\n\n");
}

void testDeg(){
	uint32_t a[5] = {0x8,0x0,0x0,0x0,0x0};
	uint32_t b[5] = {0x0,0x0,0x0,0x0,0x0};
	uint32_t c[4] = {0x8,0x0,0x0,0x80000000};
	uint32_t d[4] = {0x1,0x0,0x0,0x0};

	printf("Deg(a) should be 3. Result: %d \n", deg(5,a));
	
	printf("Deg(b) should be 0. Result: %d \n", deg(5,b));
	
	printf("Deg(c) should be 127. Result: %d \n", deg(4,c));

	printf("Deg(d) should be 1. Result: %d \n", deg(4,d));
}

void testAddP(){
	// TODO
}

void testDbl(){
	// TODO
}

void testQuad(){
	createTable();

	printf("Test squaring of polynomials:\n\n");

	printf("Test 1:\n");
	uint32_t a[6] = {0xDEADBEEF,0xDEADBEEF,0xDEADBEEF,0x0,0x0,0x0}; // x^3 
	uint32_t b[6];	
	uint32_t c[6];

	mult(6,a,6,a,c);	
	square(6, a, b);

	printf("c = "); f2m_print(6,c); printf("\n");
	printf("b = "); f2m_print(6,b); printf("\n");


	if(f2m_is_equal(6,c,b))
		printf("Worked!\n\n");
	else
		printf("Error!\n\n");

	//f[6] = {0x000000C9, 0x00000000, 0x00000000, 
	//	  0x00000000, 0x00000000, 0x00000008}

	printf("Test 2:\n");
	uint32_t f[6] = {0xDEADBEEF,0xDEADBEEF,0xDEADBEEF,0xDEADBEEF,0xDEADBEEF,0x7};

	mult(6,f,6,f,b);	
	square(6, f, c);

	printf("b = "); f2m_print(6,b); printf("\n");
	printf("c = "); f2m_print(6,c); printf("\n");


	if(f2m_is_equal(6,c,b))
		printf("Worked!\n\n");
	else
		printf("Error!\n\n");
}

void shiftOptimizedTest(){
	uint32_t a[3] =	{0x00000001, 0x00000000, 0x00000000};
	uint32_t b[3] = {0x80000000, 0x00000000, 0x00000000};
	uint32_t c[3] = {0x80000001, 0x80000001, 0x80000001};
	
	printf("Arithmetic shift tests:\n\n");	

		
	// shiftr tests
	printf("Tests for shiftl: \n");
	
	// Test a
	printf("a << 0 = ");f2m_print(3, a); printf("\n");
	shiftBl(3,a);
	printf("a << 1 = ");f2m_print(3, a); printf("\n");

	// Test b
	printf("b << 0 = ");f2m_print(3, b); printf("\n");
	shiftBl(3,b);
	printf("b << 1 = ");f2m_print(3, b); printf("\n");

	// Test c
	printf("c << 0 = ");f2m_print(3, c); printf("\n");
	shiftBl(3,c);
	printf("c << 1 = ");f2m_print(3, c); printf("\n");

	uint32_t d[3] =	{0x00000001, 0x00000000, 0x00000000};
	uint32_t e[3] = {0x00000000, 0x00000001, 0x00000000};
	uint32_t f[3] = {0x00000001, 0x00000001, 0x80000001};
	
	// shiftr tests
	printf("Tests for shiftr: \n");
	
	// Test a
	printf("d >> 0 = ");f2m_print(3, d); printf("\n");
	shiftBr(3,d);
	printf("d >> 1 = ");f2m_print(3, d); printf("\n");

	// Test b
	printf("e >> 0 = ");f2m_print(3, e); printf("\n");
	shiftBr(3,e);
	printf("e >> 1 = ");f2m_print(3, e); printf("\n");

	// Test c
	printf("f >> 0 = ");f2m_print(3, f); printf("\n");
	shiftBr(3,f);
	printf("f >> 1 = ");f2m_print(3, f); printf("\n");

}

/*
Test data taken from following sources:

http://point-at-infinity.org/ecc/nisttv
http://csrc.nist.gov/groups/ST/toolkit/documents/dss/NISTReCur.pdf
*/
/*
void scalarMultTest(){
	createTable();
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
						 
  	yQ[6], // y-coord. of result point

	xF[6] = {0xA449F864, 0x2764867D, 0x4C657BB5,
        		0x3630522F, 0x20589968, 0x00000007}, // x-coord. of result point d = 5
						 
  	yF[6] = {0x3550CB9C, 0x007AF301, 0xDB01CA79,
        		0x5DADA096, 0x02537FF5, 0x00000003}, // y-coord. of result point	
  	
	xT[6] = {0xCAA10915, 0x3C9313E7, 0x6914C218,
        		0x0F581B0D, 0x07E54141, 0x00000005}, // x-coord. of result point
						 
  	yT[6] = {0x22748341, 0x97E25F1B, 0xD20961BB,
        		0x69D3EFDB, 0x03C6D2DE, 0x00000003}, // y-coord. of result point
	
	dF[6] = {0x1,0x1,0x1,0x1,0x1,0x0},	
	
	dT[6] = {0xA,0x0,0x0,0x0,0x0,0x0};

	  	
    	printf("************************************************************\n");
    	printf("Test 1: k = 5\n");	
    	printf("************************************************************\n");
	
	mult_scalar(163,f,a,b,dF,xP,yP,xQ,yQ);
	printf("xF should be "); f2m_print(7,xF);printf("\n");
	printf("xF = "); f2m_print(7,xQ);printf("\n");
	printf("yF should be "); f2m_print(7,yF);printf("\n");
	printf("yF = "); f2m_print(7,yQ);printf("\n");

	if(f2m_is_equal(6,xQ,xF) && f2m_is_equal(6,yQ,yF))
		printf("Works!\n\n");
	else
		printf("Error!\n\n");


    	printf("************************************************************\n");
    	printf("Test 2: k = 10\n");	
    	printf("************************************************************\n");
	
	mult_scalar(163,f,a,b,dT,xP,yP,xQ,yQ);
	printf("xT should be "); f2m_print(7,xT);printf("\n");
	printf("xT = "); f2m_print(7,xQ);printf("\n");
	printf("yT should be "); f2m_print(7,yT);printf("\n");
	printf("yT = "); f2m_print(7,yQ);printf("\n");
	
	if(f2m_is_equal(6,xQ,xT) && f2m_is_equal(6,yQ,yT))
		printf("Works!\n\n");
	else
		printf("Error!\n\n");


//
k = 1
x = 03F0EBA16286A2D57EA0991168D4994637E8343E36
y = 00D51FBC6C71A0094FA2CDD545B11C5C0C797324F1

k = 2
x = 01AEB33FED9C49E0200A0C561EA66D5AB85BD4C2D4
y = 0530608192CD47D0C24C20076475FD625CC82895E8

k = 3
x = 0634000577F86AA315009D6F9B906691F6EDD691FE
y = 0401A3DE0D6C2EC014E6FBA5653587BD45DC2230BE

k = 4
x = 04053748C8CCD84AF888D3E7623F4FF3B75D153F39
y = 064B0908949B6A838153953B06CD169CC311F5FDA7

k = 5
x = 07205899683630522F4C657BB52764867DA449F864
y = 0302537FF55DADA096DB01CA79007AF3013550CB9C

k = 6
x = 065AD02C42180EA317348FFE342FB1CF2A3E896195
y = 0054D6F924A2880B5507C59B5B768ABDD6883CC94F

k = 7
x = 043EAAAF4BEA5A8C0A3EB105B31A0CF6ABAD87B13A
y = 05FAD8CE53A9D7FD436C988C7A932B0BD27289A17F

k = 8
x = 04547BD66270DF7A9601351A616FEF080D44528B03
y = 019303302D63359036B047497DC2F1BB94BB3D93C4

k = 9
x = 04802FB7306AE7CAA87F08815BABDFEEBBA9E7A7D3
y = 051887A199573D8C5E2E54FA7FB6859C9F5ABA0256

k = 10
x = 0507E541410F581B0D6914C2183C9313E7CAA10915
y = 0303C6D2DE69D3EFDBD20961BB97E25F1B22748341

///
}*/

// runs testcases of the different files
void main(){
	//invTest();
	//invTest2();
	//testNumberOfOnes();
	//testDeg();
	//testMultSpecific();
	//testAddP(); TODO!!!!
	//testDbl(); TODO!!!!!
	//testQuad();
	//shiftOptimizedTest();
	//scalarMultTest();
}
