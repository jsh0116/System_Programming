#include <stdio.h>

int kk(void)
{
	int k = 5;
	printf("%d", k);
}

void main()
{
	int i;
	double j;
	
	kk();
	for(i=0; i<5; i++)
	{
		j = i/2 + i;
		printf("j is %f \n" ,j);
	}
}
