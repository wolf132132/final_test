#include "types.h"
#include "arm.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int sys_fork(void)
{   if(proc->trace == 1){
        cprintf("[%d]sys_fork()\n", proc->pid);
    }
    return fork();
}

int sys_exit(void)
{
    if(proc->trace == 1){
        cprintf("[%d]sys_exit()\n", proc->pid);
    }
    exit();
    return 0;  // not reached
}

int sys_wait(void)
{
    if(proc->trace == 1){
        cprintf("[%d]sys_wait()\n", proc->pid);
    }
    return wait();
}

int sys_kill(void)
{
    int pid;

    if(argint(0, &pid) < 0) {
        return -1;
    }

    int rv = argint(0, &pid);
    if(proc->trace == 1){
        cprintf("[%d]sys_kill(%d)\n", proc->pid, pid);
    }

    if(rv < 0) {
        return -1;
    }

    return kill(pid);
}

int sys_getpid(void)
{
    if(proc->trace == 1){
        cprintf("[%d]sys_getpid()\n", proc->pid);
    }
    return proc->pid;
}

int sys_sbrk(void)
{
    int addr;
    int n;

    int arg_v = argint(0, &n);

    if(proc->trace == 1){
        cprintf("[%d]sys_sbrk(%d)\n", proc->pid, n);
    }

    if(arg_v < 0) {
        return -1;
    }

    addr = proc->sz;

    if(growproc(n) < 0) {
        return -1;
    }

    return addr;
}

int sys_sleep(void)
{
    int n;
    uint ticks0;

    acquire(&tickslock);

    ticks0 = ticks;
    int arg_v = argint(0, &n);

    if(proc->trace == 1){
        cprintf("[%d]sys_sleep(%d)\n", proc->pid, ticks0);
    }

    if(arg_v < 0) {
        return -1;
    }

    while(ticks - ticks0 < n){
        if(proc->killed){
            release(&tickslock);
            return -1;
        }

        sleep(&ticks, &tickslock);
    }

    release(&tickslock);
    return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int sys_uptime(void)
{
    uint xticks;

    acquire(&tickslock);
    xticks = ticks;
    release(&tickslock);

    return xticks;
}

int sys_memfree(void)
{
  return kmemfree();
}

int sys_trace(void)
{
    if(proc->trace == 1){
        cprintf("[%d]sys_trace()\n", proc->pid);
    }
    return trace();
}

int sys_pstatus(void)
{
    struct ptable *pt_p;
    argptr(0, (void *) &pt_p, sizeof(struct ptable));
    return ps(pt_p);

}
