#include "types.h"
#include "stat.h"
#include "user.h"
#include "syscall.h"

enum procstate { UNUSED, EMBRYO, SLEEPING, RUNNABLE, RUNNING, ZOMBIE };
struct uproc{
  uint sz;                     // Size of process memory (bytes)
  enum procstate state;        // Process state
  int pid;                     // Process ID
  int parent_pid;              // Parent ID
  int chan;                  // If non-zero, sleeping on chan
  int killed;                  // If non-zero, have been killed
  char name[16];               // Process name (debugging)
}; //add struct for uproc

static char *states[] = {
  [UNUSED]    "unused",
  [EMBRYO]    "embryo",
  [SLEEPING]  "sleep ",
  [RUNNABLE]  "runble",
  [RUNNING]   "run   ",
  [ZOMBIE]    "zombie"
};

int
main(int argc, char *argv[])
{
  struct uproc p;
  struct uproc* up = &p;
  int pid = 0;
  char *chan_s[] = {
    "NO ",
    "YES"
  };
  char *killed_s[] = {
    "NO ",
    "YES"
  };

  while(getprocinfo(pid, up) == 0) {
    printf(1, "\nprocess(pid) %d\n", up -> pid);
    printf(1, "name: %s\n", up -> name); 
    printf(1, "size: %d\n", up -> sz);
    printf(1, "parent id: %d\n", up -> parent_pid);
    printf(1, "state: %s\n", states[up -> state]);
    printf(1, "chan: %s\n", chan_s[up -> chan]);
    printf(1, "state: %s\n", killed_s[up -> killed]);
    pid ++;
  }

  exit();
}
