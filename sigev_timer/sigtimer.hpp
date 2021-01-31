#ifndef __SIGTIMER_H__
#define __SIGTIMER_H__ 1

typedef void (*fp_timerhandlefunc)(void);

typedef struct _interval {
  unsigned int sec;
  unsigned int msec;
} interval_t;


// Client API : # of intervl should be less than MAX_SIG_TIMER_HANDLER(3)
int create_sigtimerhandlerlist(interval_t intervallist[], int num_interval);
int register_timerhandlefunc(interval_t interval, fp_timerhandlefunc handlefunc);

#endif /* __SIGTIMER_H__ */
