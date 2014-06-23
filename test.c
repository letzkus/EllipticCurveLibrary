#include <stdio.h>
#include <stdint.h>
#include "binops.c"
#include "add.c"
#include "mod.c"
#include "shift.c"
#include "inv.c"
#include "mult.c"

void f2m_print(
  uint32_t t,
	uint32_t *A
)
{ 
  uint32_t i;
  printf("0x");
  for (i = 0; i < t; i++) printf("%.8X ",A[t-i-1]);
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

void testAddP(){
	// TODO
}

void testDbl(){
	// TODO
}

// runs testcases of the different files
void main(){
	//invTest();
	//invTest2();
	//testNumberOfOnes();
	testMultSpecific();
	testAddP();
	testDbl();
}
