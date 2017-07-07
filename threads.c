#include "types.h"
#include "stat.h"
#include "user.h"

int stack[4096];
int x = 0;
int main(int argc, char *argv[])
{

	printf("stack is at %p", stack);
	int tid = fork();

	if(tid<0)
	{
		printf("Error!\n");
	}
	else if(tid==0)
	{
		for(;;)
			x++;
			sleep(100);	
	}
	else
	{
		for(;;)
                        printf("x is %d\n",x);
                        sleep(100);
	}
}
