#include <stdio.h>
#include <atomic>

#include "sigtimer.hpp"

// NOTE: 등록한 fp 에서 instance 를 불러야 하는 문제가 있음...
// SingleTone이 아니므로.. 그닥 해결책이 보이지 않음.
// You can however (usually) pass a pointer to a static member function through C.
//  https://stackoverflow.com/questions/19808054/convert-c-function-pointer-to-c-function-pointer

// template <typename T>
class MetricCounter {
 private:
  std::atomic<int> cur_cnt;
  int prev_cnt;
  fp_timerhandlefunc fp;
  interval_t intv;

  MetricCounter(int init_cnt, interval_t interval, fp_timerhandlefunc timerhandlefunc) {
    cur_cnt.stroe(init_cnt, std::memory_order_acquire);
    prev_cnt = init_cnt;
    fp = timerhandlefunc;
    intv = interval;
    register_timerhandlefunc(interval, fp);
  }

 public:
  void updateOne() { cur_cnt.fetch_add(1, memory_order_relaxed); }
  int loadDiff() {
      cur_cnt.fetch_add(1, memory_order_acquire);
      int diff = cur_cnt - prev_cnt;
      if (diff == 0) return 0;

      printf("cur_cnt - prev_cnt:%d\n", diff);
      prev_cnt = cur_cnt;
      return (prev_cnt - cur_cnt);
  }
}


