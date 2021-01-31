#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <mutex>

#include "sigtimer.hpp"


#define MAX_TIMER_HANDLE_FUNC 10
#define MAX_SIG_TIMER_HANDLER 3


typedef void (*fp_sighandler)(int, siginfo_t *, void *);

typedef struct _timerhandlefunclist {
  int n_handlefunc;
  fp_timerhandlefunc handlefunc[MAX_TIMER_HANDLE_FUNC];
} timerhandlefunclist_t;

typedef struct sigtimerlist {
  interval_t interval;
  timer_t timerid;
  fp_sighandler sighandler;
  timerhandlefunclist_t handlefuncs;
} sigtimerlist_t;


// register timerhandlefunc mutex
static std::mutex timerhandlefunc_mutex;

static int num_registed_sigtimer = 0;
sigtimerlist_t sigtimers[MAX_SIG_TIMER_HANDLER];

void run_timerhandlefunc(timerhandlefunclist_t *timerhandlefunclist);

/*************************************************************************************************/
/*
  To create new timer, please check all part related to MAX_SIG_TIMER_HANDLER
  Each timer can handle functions to MAX_TIMER_HANDLE_FUNC

  To inject handlefunc into sighandler,
  it is needed to be coupled sighandler_* and sigtimers[*].handlefuncs

  handlefunc can be registed multiple timer
*/

void sighandler_0(int sig, siginfo_t *siginfo, void *uc) {
  printf("on_timer[%s] : %p\n", __func__, siginfo->si_value.sival_ptr);
  run_timerhandlefunc(&sigtimers[0].handlefuncs);
  return;
}
void sighandler_1(int sig, siginfo_t *siginfo, void *uc) {
  printf("on_timer[%s] : %p\n", __func__, siginfo->si_value.sival_ptr);
  run_timerhandlefunc(&sigtimers[1].handlefuncs);
  return;
}
void sighandler_2(int sig, siginfo_t *siginfo, void *uc) {
  printf("on_timer[%s] : %p\n", __func__, siginfo->si_value.sival_ptr);
  run_timerhandlefunc(&sigtimers[2].handlefuncs);
  return;
}

// NOTE: # of sighandlers must be same as MAX_SIG_TIMER_HANDLER
fp_sighandler sighandlers[] = {sighandler_0, sighandler_1, sighandler_2};

/*************************************************************************************************/

void run_timerhandlefunc(timerhandlefunclist_t *timerhandlefunclist) {
  for (int i = 0; i < timerhandlefunclist->n_handlefunc; i++) {
    // printf("timerhandlefunclist->handlefunc[%d]:%p\n", i, timerhandlefunclist->handlefunc[i]);
    timerhandlefunclist->handlefunc[i]();
  }
  return;
}

int add_handlefunc_to_sighandler(timerhandlefunclist_t *timerhandlefunclist, fp_timerhandlefunc handlefunc) {
  if (timerhandlefunclist->n_handlefunc < MAX_TIMER_HANDLE_FUNC) {
    timerhandlefunclist->handlefunc[timerhandlefunclist->n_handlefunc] = handlefunc;
    printf("timerhandlefunclist[%d]:%p - register hanlde:%p\n", timerhandlefunclist->n_handlefunc, timerhandlefunclist, handlefunc);
    timerhandlefunclist->n_handlefunc++;
    return 0;
  }
  return -1;
}

int register_sighandler(int signo, void (*sighandle)(int, siginfo_t *, void *)) {
  struct sigaction sa;
  sa.sa_flags = SA_SIGINFO;
  sa.sa_sigaction = sighandle;
  sigemptyset(&sa.sa_mask);

  if (sigaction(signo, &sa, NULL) == -1) {
    printf("%s fail - sigaction signo:%d\n", __func__, signo);
    return -1;
  }
  printf("%s success - sigaction signo:%d\n", __func__, signo);
  return 0;
}

int register_sigtimer(int signo, timer_t *timer_id, unsigned int sec, unsigned int msec) {
  struct sigevent sev;
  sev.sigev_notify = SIGEV_SIGNAL;
  sev.sigev_signo = signo;
  sev.sigev_value.sival_ptr = timer_id;
  if (timer_create(CLOCK_REALTIME, &sev, timer_id) != 0) {
    printf("%s fail - timer_create timer_id:%p\n", __func__, timer_id);
    return -1;
  }

  struct itimerspec timerspec;
  /* Initial expiration */
  timerspec.it_value.tv_sec = sec;
  timerspec.it_value.tv_nsec = msec * 1000000;
  /* Timer interval.sec */
  timerspec.it_interval.tv_sec = sec;
  timerspec.it_interval.tv_nsec = msec * 1000000;

  if (timer_settime(*timer_id, 0, &timerspec, NULL) != 0) {
    printf("%s fail - timer_settime timer_id:%p\n", __func__, timer_id);
    return -1;
  }
  printf("%s success - timer_id:%p\n", __func__, timer_id);

  return 0;
}

int register_sigtimerhandler(int signo, timer_t *timer_id, unsigned int sec, unsigned int msec, void (*sighandle)(int, siginfo_t *, void *)) {
  if (register_sighandler(signo, sighandle) != 0) {
    return -1;
  }

  if (register_sigtimer(signo, timer_id, sec, msec) != 0) {
    return -1;
  }
  printf("%s success - %d, %p, %d, %d, %p\n", __func__, signo, timer_id, sec, msec, sighandle);
  return 0;
}

int init_sigtimers(interval_t intervallist[], int num_interval) {
  if ((sizeof(sighandlers) / sizeof(fp_sighandler)) != MAX_SIG_TIMER_HANDLER) {
    printf("%s fail - # of sighandlers should be same as MAX_SIG_TIMER_HANDLER(%d) \n", __func__, MAX_SIG_TIMER_HANDLER);
    return -1;
  }

  for (int i = 0; i < num_interval; i++) {
    sigtimers[i].interval.sec = intervallist[i].sec;
    sigtimers[i].interval.msec = intervallist[i].msec;
    sigtimers[i].sighandler = sighandlers[i];
    sigtimers[i].handlefuncs.n_handlefunc = 0;
    for (int k = 0; k < MAX_TIMER_HANDLE_FUNC; k++) {
      sigtimers[i].handlefuncs.handlefunc[k] = nullptr;
    }
  }
  return 0;
}

int create_sigtimerhandlerlist(interval_t intervallist[], int num_interval) {
  if (num_registed_sigtimer != 0) {
    printf("%s fail - already created \n", __func__);
    return -1;
  }

  if ((num_interval > MAX_SIG_TIMER_HANDLER) || (num_interval <= 0)) {
    printf("%s fail - num_interval \n", __func__);
    return -1;
  }

  // TODO: No way to check # of intervallist
  if (init_sigtimers(intervallist, num_interval) != 0) {
    return -1;
  }
  num_registed_sigtimer = num_interval;
  for (int i = 0; i < num_interval; i++) {
    register_sigtimerhandler((SIGRTMIN + i), &sigtimers[i].timerid, sigtimers[i].interval.sec, sigtimers[i].interval.msec, sigtimers[i].sighandler);
    printf("%s - register_sigtimerhandler sec:%d, msec:%d \n", __func__, sigtimers[i].interval.sec, sigtimers[i].interval.msec);
  }

  return 0;
}

int check_timerinlist(interval_t interval) {
  int matchedsigtimer = -1;
  for (int i = 0; i < num_registed_sigtimer; i++) {
    if ((sigtimers[i].interval.sec == interval.sec) && (sigtimers[i].interval.msec == interval.msec)) {
      // printf("%s - %d %d\n", __func__, sigtimers[i].interval.sec, sigtimers[i].interval.msec);
      matchedsigtimer = i;
      return matchedsigtimer;
    }
  }
  return matchedsigtimer;
}

int register_timerhandlefunc(interval_t interval, fp_timerhandlefunc handlefunc) {
  int i = check_timerinlist(interval);
  if (i < 0) {
    printf("%s fail - interval sec:%d, msec:%d\n", __func__, interval.sec, interval.msec);
    return -1;
  };
  timerhandlefunc_mutex.lock();
  add_handlefunc_to_sighandler(&sigtimers[i].handlefuncs, handlefunc);
  timerhandlefunc_mutex.unlock();
  printf("%s success - interval sec:%d, msec:%d\n", __func__, interval.sec, interval.msec);

  return 0;
}
