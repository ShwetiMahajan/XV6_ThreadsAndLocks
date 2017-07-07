#include "types.h"
#include "stat.h"
#include "user.h"
#include "thread_lib.h"
#include "fs.h"

uint shared=0;
uint thread_count;
uint pass_count;
struct seq_lock *sl;
struct {
int pass;
int token;
} spinlock;

void* pass(void* arg)
{
  int seq;
  for(;;)
  {
    seq = seq_read();
    if(seq%2 == 0 && spinlock.pass < pass_count && seq == seq_read())
    {
      seq_write_acquire(sl);
      spinlock.token++;
      spinlock.pass++;
      printf(1,"Pass No:%d Token %d pass to Token %d\n",spinlock.pass, spinlock.token-1, spinlock.token);
      seq_write_release(sl);
      if(spinlock.token == thread_count)
        spinlock.token = 0;
    } 
    if(spinlock.pass >= pass_count){
     break;
    }
  }
  return 0;
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

	spinlock.token = 0;
	spinlock.pass = 0;
	int* d = malloc(sizeof(int)*thread_count);
	int i;
	for(i=0; i<thread_count;i++){
	  d[i] = i;
	  thread_create(pass,(void *)&d[i]);
	}
  seq_init(sl);
	for(i=0; i<thread_count;i++)
        {
	  thread_wait();
	}
	exit();
}
