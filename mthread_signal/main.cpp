#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define NUM_THREADS 3

typedef struct _thread_args {
  pthread_t tId;
  void* (*func)(void *);
} thread_args;

void* startSigThread(void*);  // signal processing thread func
void* startThread(void*);     // worker thread func

void cleanThread(thread_args* tArgs);  // thread 종료 담당
void sa_handler_usr(int nSigNum);      // signal handler func

thread_args tThreadArgs[] = {{0, startSigThread}, {0, startThread}, {0, startThread}};

int main() {
  // const char* test = getenv("DK"); // wrong value
  // if (test == NULL) {
  //   printf("Oh!!");
  //   return 0;
  // }

  int i;
  int nRtn;
  sigset_t tSigSetMask;

  // signal set setting 및 sigmask
  sigfillset(&tSigSetMask);         // 모든 시그널 셋 (<->sigemptyset)
  sigdelset(&tSigSetMask, SIGINT);  // set으로 지정한 시그널 모음에서 signo 제외 (<-> sigaddset)
  pthread_sigmask(SIG_SETMASK, &tSigSetMask, NULL);
  printf("*Process PID = %d\n", getpid());

  // thread 들 생성
  for (i = 0; i < NUM_THREADS; i++) {
    nRtn = pthread_create(&tThreadArgs[i].tId, NULL, tThreadArgs[i].func, (void*)&tThreadArgs[i]);
    if (nRtn == -1) {
      perror("pthread create error\n");
      return -1;
    }
    printf("Create thread : tid = %lu\n", tThreadArgs[i].tId);
  }

  // thread 종료
  cleanThread(tThreadArgs);

  return 0;
}

void* startSigThread(void* pArgs) {
  thread_args* tArgs = (thread_args*)pArgs;
  sigset_t tSigSetMask;
  int nSigNum;
  int nErrno;

  printf("* Start signal thread (tid = %lu)\n", (long)pthread_self());
  // 수신할 signal set setting
  sigemptyset(&tSigSetMask);
  sigaddset(&tSigSetMask, SIGUSR1);
  sigaddset(&tSigSetMask, SIGUSR2);
  sigaddset(&tSigSetMask, SIGTERM);

  while (1) {
    // signal 대기
    nErrno = sigwait(&tSigSetMask, &nSigNum);
    if (nErrno > 0) {
      perror("sigwait error\n");
      return NULL;
    }
    // signal no. 에 따라 signal handler 함수 호출 혹은 종료
    switch (nSigNum) {
      case SIGUSR1:
      case SIGUSR2:
        sa_handler_usr(nSigNum);
        break;
      case SIGTERM:
        printf("[signal SIGTERM]\n");
        exit(EXIT_SUCCESS);
      default:
        break;
    }
  }
return tArgs;
}

void* startThread(void* pArgs) {
  thread_args* tArgs = (thread_args*)pArgs;
  while (1) {
    sleep(1);
  }
  return tArgs;
}

void cleanThread(thread_args* pArgs) {
  thread_args* tArgs;
  int i;

  for (i = 0; i < NUM_THREADS; i++) {
    pthread_join(pArgs->tId, (void**)&tArgs);
    printf("Thread id(%lu) joined\n", pArgs->tId);
    pArgs++;
  }
}

void sa_handler_usr(int nSigNum) {
  printf("\t[%lu] Signal(%s)\n", pthread_self(), nSigNum == SIGUSR1 ? "USR1" : "USR2");
}
