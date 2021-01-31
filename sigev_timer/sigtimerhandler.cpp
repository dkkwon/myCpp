#include <stdio.h>
#include <atomic>
#include <iostream>

#include "sigtimerhandler.hpp"

static std::atomic<unsigned int> cur_cnt;
static unsigned int prev_cnt;

unsigned int printDiff(unsigned int& new_cnt, unsigned int& old_cnt) {
  unsigned int diff = new_cnt - old_cnt;
  if (diff == 0) return 0;

  printf("new_cnt:%d, old_cnt:%d, diff:%d\n", new_cnt, old_cnt, diff);
  old_cnt = new_cnt;
  return diff;
}

// register to sigtimer
void loadDiff() {
  unsigned int new_cnt = cur_cnt.load(std::memory_order_acquire);
  printDiff(new_cnt, prev_cnt);
}

// Use to count metric
void countMsg() {
  cur_cnt.fetch_add(1);
  return;
}
