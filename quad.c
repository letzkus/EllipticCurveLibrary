/*
	Implements the efficient squaring of polynomials
*/
#include <math.h>

//#define PARTITIONS 2
#define TABLESIZE (1 << 16)

uint32_t table[(1 << 16)]; // Table that contains the precomputed partitions
uint32_t qTemp[12];

/*
	Square algorithm
*/
void square(uint32_t l, uint32_t *a, uint32_t *r){
	int i; // array ctr
	int j = 0; // ctr for result
	uint32_t index; 
	uint32_t *upper;	
	for(i = 0; i < l; i++ ){
		qTemp[j++] = table[(a[i] & 0x0000FFFF)]; // Lower half
		qTemp[j++] = table[((a[i] & 0xFFFF0000)>>16)];// Upper half	
	} 

	//upper = temp+2;
	//mod_f163(upper);
	mod_f163(qTemp);

	memcpy(r, qTemp, (sizeof(uint32_t) * l));
}

/*
	Creates lookup table
*/
void createTable(){
	int i,j;	
	int resultptr = 0;
	int help;
	table[resultptr++] = 0;	
	for(i = 0; i < 16; i++){
		help = resultptr;		
		for(j = 0; j < help; j++)
			table[resultptr++] = pow(2,i*2) + table[j];
		
	}

	// TODO DEBUG
	/*for(i = 0; i < (1 << 16); i++)
		printf("%d \n",table[i]);*/
}
