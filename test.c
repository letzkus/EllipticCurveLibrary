#include "add.c"
#include <stdio.h>
#include <stdint.h>


void main(){
	uint32_t a[3] = {0x1,0x0,0x0};
	uint32_t b[2] = {0x2,0x0};
	
	addI(3,a,2,b,a);

	printf("%.8X ",a[0]);
}
