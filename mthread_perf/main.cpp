#include <pthread.h>
#include <stdio.h>

static volatile int balance = 0;

#define MAX_NUM 10000000

void *deposit(void *param) {
  char *who = (char *)param;

  int i, j;
  printf("%s: begin\n", who);
  for (j = 0; j < MAX_NUM; j++) {
    for (i = 0; i < MAX_NUM; i++) {
      balance = balance + 1;
    }
  }
  printf("%s: done\n", who);
  return NULL;
}

int main() {
  pthread_t p1, p2, p3, p4;
  printf("main() starts depositing, balance = %d\n", balance);
  pthread_create(&p1, NULL, deposit, (void *)"A");
  pthread_create(&p2, NULL, deposit, (void *)"B");
  pthread_create(&p3, NULL, deposit, (void *)"A");
  pthread_create(&p4, NULL, deposit, (void *)"B");

  // join waits for the threads to finish
  pthread_join(p1, NULL);
  pthread_join(p2, NULL);
  pthread_join(p3, NULL);
  pthread_join(p4, NULL);
  printf("main() A and B finished, balance = %d\n", balance);
  return 0;
}
