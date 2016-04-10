// integer tests

#include <stdio.h>
#include "intmath.h"

int main (int argc, char * argv[]) {

	int a,b;
	unsigned c,d;
	long e,f;
	unsigned long g,h;
	
	// Unsigned int addition overflow test
	
	
	// Unsigned int subtraction underflow test
	
	// Signed int overflow test
	printf("\n-- add_i --\n");
	printf(" 1 +  2 = % i\n", add_i( 1,  2));
	printf("-1 + -2 = % i\n", add_i(-1, -2));
	
	printf(" 1 + INT_MAX = % i\n", add_i( 1, INT_MAX));
	printf("-1 + INT_MIN = % i\n", add_i(-1, INT_MIN));
	printf("INT_MAX +  1 = % i\n", add_i(INT_MAX,  1));
	printf("INT_MIN + -1 = % i\n", add_i(INT_MIN, -1));
	
	printf("\n-- sub_i --\n");
	printf("INT_MIN -  1 = % i\n", sub_i(INT_MIN, 1));
	
	printf("INT_MAX - -1 = % i\n", sub_i(INT_MAX, -1));
	
	
	//printf("-1 - INT_MIN = %i\n", sub_i(-1, INT_MIN));
	
	//printf("-1 - INT_MIN = %i\n", sub_i(-1, INT_MIN));
	
	// Signed int underflow test
	
	return 0;
}