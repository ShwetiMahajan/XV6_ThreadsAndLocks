#include "types.h"
#include "stat.h"
#include "user.h"
#include "x86.h"
#include "thread_lib.h"
#include "fs.h"

#define PGSIZE 4096 // size of kernel stack per process

//Initialise the value of lock as 0
int lock_init(struct lock_t *lk)
{
        lk->flag = 0;
        return 0;
}

//Check if the lock is not acquired, if not aqcuire and change flag's value to 1
void lock_acquire(struct lock_t *lk){
        while(xchg(&lk->flag, 1) != 0)
            ;
}

//Release the lock by setting flag's value to be 0
void lock_release(struct lock_t *lk){
        xchg(&lk->flag, 0);
}
int anderson_init(struct flags *fl, int count){
  int i;
  fl->flag = malloc(count*sizeof(int));
  for(i=0; i<count; i++)
    fl->flag[i] = 0;
  fl->flag[0] = 1;
  fl->next = 0;
  return 0;
}

uint anderson_acquire(struct flags *fl, int N){
  uint my_thread;
  xchg(&my_thread, fl->next); //next_place+1;
  xchg(&fl->next, fl->next+1);
  while(fl->flag[my_thread % N] == 0)
  {
   printf(1, "");
  }
  return (my_thread%N);
}

void anderson_release(uint my_thread, struct flags *fl, int N){
  fl->flag[my_thread % N] = 0;
  fl->flag[(my_thread+1) % N] = 1;
}

int seq_init(struct seq_lock *sl){
  seq_no = 0;
  sl->lock = 0;
  return 0;
}

int seq_read(){
  return seq_no;
}

void seq_write_acquire(struct seq_lock *sl){
  while(xchg(&sl->lock, 1) != 0);
  seq_no = seq_no + 1;
}

void seq_write_release(struct seq_lock *sl){
  xchg(&sl->lock, 0);
  seq_no = seq_no +1;
}

//routine thread_create
int
thread_create(void* (*start_routine)(void*), void *arg)
{
        void *stack= malloc(PGSIZE*2);

        int result = clone(stack, PGSIZE*2);

        if(result==0)
        {
                (*start_routine)(arg);
                //free(stack);
                exit();
        }
 return result;
}

void
thread_wait()
{
	wait();
}
