#include <stdio.h>

struct s_sampleStruct {
	int	num1;
	int num2;
};

int main(void) {
	s_sampleStruct myStruct;
	myStruct.num1 = 1;
	myStruct.num2 = 2;

	s_sampleStruct myStruct2(myStruct);

	myStruct = myStruct2;

	printf("%d %d\n", myStruct.num1, myStruct.num2);

	return 0;
}

enum Method
{
	GET = 0,
	PUT = 1,
	POST = 2,
	HEAD = 3,
	UNSUPPORTED = 4
};

Method test(void) {
	return UNSUPPORTED;
}