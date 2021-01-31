// https://m.blog.naver.com/k5248/120197354010

#include <signal.h>
#include <stdio.h>
#include <string.h>

#include <time.h>
#include <sys/time.h>


// Timer
timer_t firstTimerID;
timer_t secondTimerID;

static void timer_handler( int sig, siginfo_t *si, void *uc );
static int makeTimer( char *name, timer_t *timerID, int sec, int msec );

int main ()
{
        makeTimer("First Timer", &firstTimerID, 0, 50);
        makeTimer("Second Timer", &secondTimerID,1, 0);

        /* Do busy work.  */
        while (1);
}

static int makeTimer( char *name, timer_t *timerID, int sec, int msec )
{
    struct sigevent         te;
    struct itimerspec       its;
    struct sigaction        sa;
    int                     sigNo = SIGRTMIN;

    /* Set up signal handler. */
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = timer_handler;
    sigemptyset(&sa.sa_mask);

    if (sigaction(sigNo, &sa, NULL) == -1)
    {
        printf("sigaction error\n");
        return -1;
    }

    /* Set and enable alarm */
    te.sigev_notify = SIGEV_SIGNAL;
    te.sigev_signo = sigNo;
    te.sigev_value.sival_ptr = timerID;
    timer_create(CLOCK_REALTIME, &te, timerID);

    its.it_interval.tv_sec = sec;
    its.it_interval.tv_nsec = msec * 1000000;
    its.it_value.tv_sec = sec;

    its.it_value.tv_nsec = msec * 1000000;
    timer_settime(*timerID, 0, &its, NULL);

    return 0;
}

static void timer_handler( int sig, siginfo_t *si, void *uc )
{
    timer_t *tidp;
    tidp = (timer_t *)si->si_value.sival_ptr;

    if ( *tidp == firstTimerID )
    {
        printf("firstTimerID\n");
    }
    else if ( *tidp == secondTimerID )
    {
        printf("secondTimerID\n");
    }
}
