#include "types.h"
#include "stat.h"
#include "user.h"
#include "thread_lib.h"
#include "fs.h"

uint shared=0;
uint thread_count;
uint pass_count;
int *flags, *next;
struct slock_t {
  int pass;
  struct lock_t l;
}spinlock;
struct flags *a_flag;

void* pass(void* arg)
{
  for(;;)
  {
    uint current = anderson_acquire(a_flag, thread_count);
    if(spinlock.pass < pass_count)
    {
      spinlock.pass++;
      printf(1,"Pass No:%d Token %d pass to Token %d\n",spinlock.pass,current,current+1);
    } else if(spinlock.pass >= pass_count)
    {
      anderson_release(current, a_flag, thread_count);
      return 0;
    }
    anderson_release(current, a_flag, thread_count);
  }
}
int
main(int argc, char* argv[])
{
	if(argc!=3)
	{
	  printf(1,"wrong input\n");
	  exit();
	}
	thread_count = atoi(argv[1]);
	pass_count = atoi(argv[2]);

	spinlock.pass = 0;
        anderson_init(a_flag, thread_count);
	int* d = malloc(sizeof(int)*thread_count);
	int i;
	for(i=0; i<thread_count;i++){
		d[i] = i;
		thread_create(pass,(void *)&d[i]);  	
         }
         for(i=0; i<thread_count;i++)
        {
	 thread_wait();				      
	}					 
 	exit();
}
	
