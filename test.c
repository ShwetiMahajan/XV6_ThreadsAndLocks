#include "types.h"
#include "stat.h"
#include "user.h"
#include "thread_lib.h"
#include "fs.h"

uint shared=0;
struct lock_t *lk;

void* test1(void* arg ){
	//int i;
	lock_acquire(lk);
	int local_val=0;
	//printf(1,"Value local to thread %d is : %d",j, local_val);
	local_val = shared;;
	//for(i=0;i<400;i++){
	local_val = local_val+100;
	shared=local_val;
	//}
	//printf(1,"Value local to thread is : %d\n",local_val);
	//printf(1,"Global value is : %d\n",shared);
	lock_release(lk);
	exit();
}

uint thread_count;
uint pass_count;
struct slock_t {
  int token;
  int pass;
  struct lock_t l;
}spinlock;

void* pass(void* arg)
{
  int thid = *(int *)arg;
  for(;;)
  {
    lock_acquire(&spinlock.l);
    if(spinlock.token == thid && spinlock.pass < pass_count)
    {
      spinlock.token++;
      spinlock.pass++;
      if(spinlock.token == thread_count)
        spinlock.token = 0;
      printf(1,"Pass No:%d Token %d pass to Token %d\n",spinlock.pass,thid,spinlock.token);
    } else if(spinlock.pass >= pass_count)
    {
      lock_release(&spinlock.l);
      return 0;
    }
    lock_release(&spinlock.l);
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
	
	//lk = (struct lock_t*)malloc(sizeof(struct lock_t));
	spinlock.token = 0;
	spinlock.pass = 0;
	lock_init(&spinlock.l);
	int* d = malloc(sizeof(int)*thread_count); 
	int i;
	for(i=0; i<thread_count;i++)
        {
	  d[i] = i;
	  thread_create(pass,(void *)&d[i]);
	}
	for(i=0; i<thread_count;i++)
        {
	  thread_wait();
	}
	exit();
}
