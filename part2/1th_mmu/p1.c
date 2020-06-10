#include <stdio.h>

int a = 1;

void main()
{
	while(1)
	{
		printf("&a = %d,a = %d",&a,a);
		sleep(3);
	}
}