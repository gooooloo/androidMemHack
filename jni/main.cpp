#include <sys/ptrace.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>

// inspired by http://stackoverflow.com/questions/22852672/scanning-and-editing-android-app-memory-values-programmatically
int main (int argc, char *argv[]) {

  int ret = 0;
  int pid = 0;
  long address = 0;
  char *endCharIgnore;
  const bool readonly = (argc == 3);
  int waitPidStatus = 0;
  int dataOpResult = 0; 

  if (argc < 3) {
    printf("insufficient arguments. # >= 2 required.");
    ret = 1;
    goto RET;
  }
  errno = 0;
  pid = atoi(argv[1]);
  if (pid == 0) {
    printf("error argument #1. Must be non-0 integer");
    ret = 2;
    goto RET;
  }
  // atoi can't handle 0x**** input, so use strtol
  address = std::strtol(argv[2], &endCharIgnore, 16);
  if (address == 0) {
    printf("error argument #2. Must be non-0 base-16 long");
    ret = 3;
    goto RET;
  }

  if (ptrace(PTRACE_ATTACH, pid, NULL, NULL) == -1) {
    perror("attach");
    ret = 4;
    goto RET;
  }

  if (waitpid(pid, &waitPidStatus, 0) == -1 || !WIFSTOPPED(waitPidStatus)) {
    perror("waitPid");
    ret = 5;
    goto DETACH_AND_RET;
  }

  errno = 0;
  if (readonly) {
    dataOpResult = ptrace(PTRACE_PEEKDATA, pid, (void*)address, 0);
  } else {
    dataOpResult = ptrace(PTRACE_POKEDATA, pid, (void*)address, 8888);
  }
  if (errno != 0) {
    perror("peek | poke");
    ret = 6;
  } else {
    printf("dataOpResult:%d\n", dataOpResult);
  }

DETACH_AND_RET:
  ptrace(PTRACE_DETACH, pid, NULL, NULL);
RET:
  return ret;
}
