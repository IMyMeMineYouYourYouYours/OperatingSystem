#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"
#include "proc.h"
#include "spinlock.h"

int thread_create(thread_t* thread, void*(*start_routine)(void*), void arg){
    int i,pid;
    struct proc *p;

    if((p=allocproc())==0)
        return -1;
    
    p->sz=proc->sz;

    if(p->pthread==0){
        p->parent=proc;
    }
    else{
        p->parent=proc->parent;
    }
    *p->tf=*proc->tf;

    p->tf->eax=0;

    p->pgdir=proc->pgdir;
    p->tf->eip=(uint)start_routine;
    p->pthread=1;
    p->ustack=kalloc();
    p->tf->esp=p->ustack+4096;
    *((int*)(p->tf->esp))=(int)arg;
    p->tf->esp-=4;
    *((int*)(p->tf->esp))=0xFFFFFFFF;
    
    for(i=0;i<NOFILE;i++)
        if(proc->ofile[i])
            p->ofile[i]=filedup(proc->ofile[i]);
    p->cwd=idup(proc->cwd);
    
    safestrcpy(p->name,proc->name,sizeof(proc->name));

    pid=p->pid;
    
    acquire(&ptable.lock);
    p->state=RUNNABLE;
    release(&ptable.lock);

    return pid;
}

int
sys_thread_create(void)
{
    char *str;

    if(argstr(0,&str)<0)
        return -1;
    return thread_create(str);
}


