
struct lock_t
{uint flag;};

// int *anderson_flags;
int *next_place;

int seq_no;
struct seq_lock{
  uint lock;
};

struct flags{	
 int *flag;
 uint next;
};

int anderson_init(struct flags *fl, int count);
uint anderson_acquire(struct flags *fl, int N);
void anderson_release(uint current, struct flags *fl, int N);
int fetch_and_inc(int *next);

int seq_init();
int seq_read();
void seq_write_acquire(struct seq_lock *sl);
void seq_write_release(struct seq_lock *sl);

int thread_create(void* (*start_routine) (void *), void *arg);
void thread_wait();
int lock_init(struct lock_t *lk);
void lock_acquire(struct lock_t *lk);
void lock_release(struct lock_t *lk);
