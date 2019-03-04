#include "types.h"
#include "defs.h"
#include "param.h"
#include "mmu.h"
#include "proc.h"
#include "traps.h"

extern int mlfqnum;
extern int stridenum;
extern int schedset;

int
set_cpu_share(int num)
{
    if(stridenum+num>80){
        return 0;
    }
    else{
        stridenum+=num;
        mlfqnum-=num;
        proc->stride=1000/num;
        schedset=1;
        proc->pass=0;
        return 1;
    }
}

int
sys_set_cpu_share(void)
{
    int num;
    
    if(argint(0,&num)<0)
        return -1;
    return set_cpu_share(num);
}
