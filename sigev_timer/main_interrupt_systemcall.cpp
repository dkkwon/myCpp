#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void handler(int s) {
  printf("read is interrupt by signal handler\n");
  return;
}

int main() {
  char buf[10];
  int nread = 0;
  struct sigaction act;

  sigemptyset(&act.sa_mask);
  act.sa_handler = handler;
  act.sa_flags = 0;
//   act.sa_flags |= SA_RESTART;  // Set SA_RESTART flag for SIGALRM signal

  sigaction(SIGALRM, &act, NULL);
  alarm(2);

  printf("read start\n");
  nread = read(STDIN_FILENO, buf, sizeof(buf));
  printf("read return\n");

  if (nread < 0) {
    if (errno == EINTR) {
      printf("errno: EINTR");
      // If read is interrupted, it should not be counted as a failure. It can be handled according to actual needs, such as rewriting the call to
      // read, or ignoring it.
    } else {
      // read Real Reading Errors
      printf("Real Reading Errors");
    }
  }

  return 0;
}
