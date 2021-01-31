#include <stdio.h>
#include <thread>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>

// #include "sigtimer.hpp"
#include "sigtimerhandler.hpp"


void testhandle_1(void) { printf("run [%s]\n", __func__); }
void testhandle_2(void) { printf("run [%s]\n", __func__); }
void testhandle_3(void) { printf("run [%s]\n", __func__); }
void testhandle_4(void) { printf("run [%s]\n", __func__); }
void testhandle_5(void) { printf("run [%s]\n", __func__); }
void testhandle_6(void) { printf("run [%s]\n", __func__); }
void testhandle_7(void) { printf("run [%s]\n", __func__); }
void testhandle_8(void) { printf("run [%s]\n", __func__); }

void runCount() {
  while (1) {
    countMsg();
    usleep(1);
  }
}

int main() {
  // interval_t intervals[] = {{0, 500}, {2, 0}, {4, 0}};
  // create_sigtimerhandlerlist(intervals,  sizeof(intervals)/sizeof(interval_t));

  // interval_t intervals[] = {{0, 500}, {4, 0}};
  // create_sigtimerhandlerlist(intervals, 3);  // <-- This make wrong value timer registration

#if 1
  interval_t intervals[] = {{2, 0}};
  create_sigtimerhandlerlist(intervals, sizeof(intervals) / sizeof(interval_t));

  register_timerhandlefunc({2, 0}, loadDiff);

  std::thread thread_1(runCount);

#else
  interval_t intervals[] = {{0, 500}, {2, 0}};

  // register to ccountMsg();ot created timer
  register_timerhandlefunc({1, 500}, testhandle_3);
  register_timerhandlefunc({1, 0}, testhandle_4);

  // register to multiple created timer
  register_timerhandlefunc({2, 0}, testhandle_5);
  register_timerhandlefunc({2, 0}, testhandle_6);
  register_timerhandlefunc({4, 0}, testhandle_7);
  register_timerhandlefunc({4, 0}, testhandle_8);
  register_timerhandlefunc({4, 0}, testhandle_6);
#endif

  while (1) {
    struct timespec req, rem;
    int time_msleep = 5000;
    req.tv_sec = time_msleep / 1000;
    req.tv_nsec = (time_msleep % 1000) * 1000000;
    while (nanosleep(&req, &rem) != 0) {
      printf("usleep error - tv_sec(%ld), tv_nsec(%ld): %s\n", rem.tv_sec, rem.tv_nsec, strerror(errno));
      if (errno == EINTR) {
        req.tv_sec = rem.tv_sec;
        req.tv_nsec = rem.tv_nsec;
      }
    }
  }

  return 0;
}
