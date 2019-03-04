# OperatingSystem

# The first milestone
## first step: What algorithms need to do this project. 
1. Stride scheduling: if process call sys call set_cpu_share then this process scheduled in stride. stride[NPROC]
2. MLFQ: MLFQ has 3 queues mlfq[0], mlfq[1], mlfq[2] and mlfq[0] is the highest priority queue, mlfq[2] is the lowest priority queue.
3. MLFQ must have 20% of CPU.

## second step: explain my code flow
1. New processes enter to MLFQ, first.
2. During looping, check IsEmpty of mlfq, this priority order. And if mlfq is not empty, dequeue process in mlfq and check this is runnable.  
3. If the process is runnable, then do context switch to the process. And the process is running for 5ticks in mlfq[0], 10ticks in mlfq[1], 20ticks in mlfq[2].(ticks are time between consecutive timer interrupt. In xv6 about 10ms) After the process is running 5ticks or 10ticks, the process down priority.(go to lower mlfq)
4. To resolve starvation, I use boosting. In trap.c, if  interrupt timer ticks's is 100, do boosting. (Boosing set all process to go mlfq[0])
5. To resolve gaming scheduler, I add 1 tick to process's ticks and process's ticks 5 or 10 down this priority(go to lower mlfq).
6. If set_cpu_share called, this process go to stride array. If stride gets 80% of CPU over, this process doesn't go to stride array.  
7. I set a large number 1000 and all tickets 100.
8. If set_cpu_share 10 is called, then in scheduler function mlfq loop 90th, and stride loop 10th.(stridenum->the number how many loop, mlfqnum->the number how many loop)

# The first milestone
## What systemcalls need to do this project. 
1. create: int thread_create(thread_t* thread, void* (* start routine)(void* ),void* arg)
2. exit: void thread_exit(void* retval)
3. join: int thread_join(thread_t thread, void ** retval)

## second step: explain my code
  thread struct in proc struct.  
  - pthread: if pthread value is 1 then the process is pthread, but then the process is not pthread, tid: pthread's id, pthreadret: save retval of exit to this, threadcnt is the number of threads.

### create systemcall
  In thread_create, I made a new proc(p) to use as pthread.
 - So, I used allocproc to allocate proc's kernel stack.  In allocproc, I initialize pthread and threadcnt.
 - And pointing of new proc's tf to current process' tf. 
 - And pointing of new proc's page directory to current process' page directory.  
 - And then, allocate a new stack for the new proc to use allocuvm. I will allocate the size, PGSIZE. And the new stack will be allocated next to last pthread address, By use this sz=proc->sz+proc->threadcnt*PGSIZE 
 - Then,new proc sz is the return value of allocuvm function. 
 - And set new process's esp p->sz. Because stack is allocated from high address. 
 - And push arg in esp-4 and fake return(0xFFFFFFFF) in esp-8 into new process' userstack.
 - And  change esp to esp-8.
 - And the new thread of tid is current proc threadcnt+1.
 - And load address of thread in tid of proc. 
 - Finally if all is run success, return 0, but return error number.

### exit systemcall
  I use exit function in proc.c. In exit, wakeup thread's parent and exit thread doing load retval in pthreadret of proc.

### join systemcall
  - I use wait function in proc.c. In wait, the process wait until the process' kids exit. So I use sleep.
  - Loop until find p's tid is thread.
  - And the process don't have kids or exit kids and process' state ZOMBIE, then dealloc the process userstack using deallocuvm. 
  - And the process state change UNUSED, free process' kernel stack.
  



# The second Milestone
### EXIT SYSCALL
  I separate the three cases. First case is the caller is process, not have threads. Second case is the caller is process, have threads. Last case is the caller is thread.
  In first case, it is same algorithm exit in proc.c
  In second case, all threads' parent process map to initproc. And exit process treat this in wait function.
  In last case, all thread clean up, and kill thread's parent, and wakeup thread's parent parent treat this in wait function.
  But,, I don't implement this.


# Filesystem
## What kind of things that I changed
  In fs.h code, I changed define NDIRECT to 11, changed addrs' index to NDIRECT + 2 in diode structure. And in param.h, I changed FSSIZE to 2000. And in file.h, I changed addrs' index to NDIRECT + 2 ini node structure. And in fs.c, I add some code in bmap function and in  itruc function.
## Explain my code
  The inode block number is 13. 11 inode blocks are DIRECT So I define NDIRECT 11. And one INDIRECT, another one Double INDIRECT.  So NINDIRECT size is BSIZE/sizeof(*uint) and Double INDIRECT size is NINDIRECT * NINDIRECT. So MAXFILE size is NDIRECT + NINDIRECT + NINDIRECT * NINDIRECT. 
As like built in bmap function DIRECT and INDIRECT code, Double Indirect is similar. 
- If bn is < NINDIRECT * NINDIRECT assignment addr is ip->addrs[NDIRECT+1]. (This is Double Direct index in inode)
- if add = 0 then allocate address using balloc
- And save buf struct bp using bread(ip->dev, addr) , a is bp->data
- And again assignment addr is a[bn/NINDIRECT] and if addr is 0 then balloc and write. And release bp.
- And again assignment addr is a[bn%NINDIRECT] and if addr is 0 then balloc and write.

And in iturnc as like Direct, Indirect code Double indirect is similar.
- First check ip->addrs[NDIRECT+1] is 0 or not.
- If it is not 0, read buf. And a is bp->data. 
- And loop NDIRECT times check a[j] is 0 or not.
- If a[j] is not 0, read buf. And a2 is bp2->data.
- And loop NDIRECT times bfree a2[k]
- And bfree a[j]
- Finally, bfree ip->addrs[NINDIRECT+1]