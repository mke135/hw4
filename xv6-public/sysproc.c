#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
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
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

//implementation of system call of traceback
int
sys_backtrace(void)
{
  //inplement backtrace
  uint ebp, retaddr;
  struct trapframe *tf;
  tf = myproc()->tf;
  //print registers in trapframe for backtrace
  cprintf("eax: 0x%x\nebx: 0x%x\necx: 0x%x\nedx: 0x%x\nesi: 0x%x\nedi: 0x%x\nebp: 0x%x\nesp: 0x%x\neip: 0x%x\n", 
    tf->eax, tf->ebx, tf->ecx, tf->edx, tf->esi, tf->edi, tf->ebp, tf->esp, tf->eip);
  ebp = tf->ebp;
  int i = 0;
  while(ebp) {
    retaddr = *(uint *)(ebp + 4);
    cprintf("#%d  0x%x\n", i, retaddr);
    i++;
    if(retaddr == 0xffffffff)
      break;
    ebp = *(uint *)ebp;
  }
  //unwind kernel stack of the program
  uint sp, bottom_k, content;
  bottom_k = (uint) myproc()->kstack;
  sp = bottom_k + KSTACKSIZE - 4;
  cprintf("\n-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.\n");
  cprintf("Extra Part: unwind content in kernel stack:\n");
  while(sp > bottom_k){
    content = *(uint *)(sp);
    if(content == 0x1010101) {
      break;//repeated content of 0x1010101 in the following, it may be nonsense to print out
    }
    cprintf("%x : 0x%x\n", sp, content);
    sp -= 4;
  }

  return 0;
}

int 
sys_getprocinfo(void)
{
  int pid;
	char* up;
  //get arguments
	if (argint(0, &pid) < 0)
		cprintf("%s\n", "argint(pid) error");
	if (argptr(1, &up, 4) < 0)
		cprintf("%s\n", "argptr(up) error");

  if (getprocinfo(pid, (struct uproc*)up) == 0) {
		return 0;
	}
	else return 1;
}
