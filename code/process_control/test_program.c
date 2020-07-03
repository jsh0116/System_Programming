#include <stdio.h>
#include <stdlib.h>
int a,b;
int glob_var = 3;

void test_func(void)
{
	int local_var, *buf;
	buf = (int *)malloc(10000*sizeof(int));
}

int main(int argc, char **argv)
{
	int i=1;
	int local_var;

	a = i+1;
	printf("value of a = %d\n",a);
	test_func();
}
