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

void invTest(){
	printf("Tests for multiplicative inverse: \n");
	uint32_t f[6] = {0x000000C9, 0x00000000, 0x00000000, 
		  0x00000000, 0x00000000, 0x00000008}; 

	uint32_t a[3] = {0x00000A74, 0x00000000, 0x00000000};

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

// runs testcases of the different files
void main(){
	invTest();		
}
