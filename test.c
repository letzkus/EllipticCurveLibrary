#include <stdio.h>
#include <stdint.h>

#include "binops.c"
#include "add.c"
#include "mod.c"
#include "shift.c"
#include "inv.c"
#include "mult.c"
#include "quad.c"

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

int count(int l, uint32_t *a){
	int ctr = 0;
	int i = deg(6,a);
	// Implements the double and add method
	for(; i >= 0; i--){
		if((a[i/32]>>(i%32))&0x1){
			// Add
			ctr++;
		} 
	}
	return ctr;
}

void testNumberOfOnes(){
	
	uint32_t a[6] = {0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000};
	printf("a should be: 0\n"); 
	printf("Result: %d \n\n",count(6,a));	
	uint32_t b[6] = {0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000001};
	printf("b should be: 1\n"); 
	printf("Result: %d \n\n",count(6,b));
	uint32_t c[6] = {0x10000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000001};
	printf("c should be: 2\n"); 
	printf("Result: %d \n\n",count(6,c));	
	uint32_t d[6] = {0x80000003,0x80000010,0x80000001,0x80000001,0x80000001,0x80000001};
	printf("Should be: 13\n"); 
	printf("Result: %d \n\n",count(6,d));
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

// runs testcases of the different files
void main(){
	//invTest();
	//invTest2();
	testNumberOfOnes();
	testDeg();
	//testMultSpecific();
	//testAddP(); TODO!!!!
	//testDbl(); TODO!!!!!
	//testQuad();
	//shiftOptimizedTest();
}
