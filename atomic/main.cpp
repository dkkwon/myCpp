/*
  memory_order_relaxed : CPU 에서 메모리 연산 순서에 관련해서 자유를 주는 것
  memory_order_release : 해당 명령 이전의 모든 메모리 명령들이 해당 명령 이후로 재배치 되는 것을 금지
  memory_order_acquire : 해당 명령 뒤에 오는 모든 메모리 명령들이 해당 명령 위로 재배치 되는 것을 금지
  memory_order_acq_rel : acquire 와 release 를 모두 수행
  memory_order_seq_cst : 메모리 명령의 순차적 일관성(sequential consistency) 을 보장


*/

#include <atomic>
#include <iostream>
#include <thread>
#include <vector>
using std::memory_order_relaxed;

std::atomic<bool> is_ready;
std::atomic<int> data[3];

void producer() {
  data[0].store(1, memory_order_relaxed);
  data[1].store(2, memory_order_relaxed);
  data[2].store(3, memory_order_relaxed);
  is_ready.store(true, std::memory_order_release);
}

void consumer() {
  // data 가 준비될 때 까지 기다린다.
  while (!is_ready.load(std::memory_order_acquire)) {
  }

  std::cout << "data[0] : " << data[0].load(memory_order_relaxed) << std::endl;
  std::cout << "data[1] : " << data[1].load(memory_order_relaxed) << std::endl;
  std::cout << "data[2] : " << data[2].load(memory_order_relaxed) << std::endl;
}

void worker(std::atomic<int>* counter) {
  for (int i = 0; i < 10000; i++) {
    // 다른 연산들 수행

    counter->fetch_add(1, memory_order_relaxed);
  }
}

int main() {

  std::atomic<int> intAtomic(2);
  std::cout << "std::atomic<int> intAtomic(2);; - " << intAtomic.load() << std::endl;

  intAtomic.store(3);
  int value = intAtomic.load();
  std::cout << "intAtomic.store(3);intAtomic.load(); - " << intAtomic << std::endl;

  int oldValue = intAtomic.exchange(5);
  std::cout << "int oldValue = intAtomic.exchange(5); - oldValue:" << oldValue << ", intAtomic.load():" << intAtomic.load() << std::endl;

  int comparand = 3;
  int newValue = 10;
  bool exchanged = intAtomic.compare_exchange_weak(comparand, newValue);
  std::cout << "intAtomic.compare_exchange_weak(comparand, newValue); - comparand:" << comparand << ", newValue:" << newValue
            << ", exchanged:" << exchanged << std::endl;
  std::cout << "intAtomic.load(); - " << intAtomic << std::endl;

  comparand = 5;
  exchanged = intAtomic.compare_exchange_weak(comparand, newValue);
  std::cout << "intAtomic.compare_exchange_weak(comparand, newValue); - comparand:" << comparand << ", newValue:" << newValue
            << ", exchanged:" << exchanged << std::endl;
  std::cout << "intAtomic.load(); - " << intAtomic << std::endl;
  std::cout << std::endl;

  /********************************************************/

  std::vector<std::thread> threads;
  std::atomic<int> counter(0);

  threads.push_back(std::thread(producer));
  threads.push_back(std::thread(consumer));

  for (int i = 0; i < 4; i++) {
    threads.push_back(std::thread(worker, &counter));
  }

  for (int i = 0; i < 6; i++) {
    threads[i].join();
  }

  std::cout << std::endl;
  std::cout << "std::atomic<int> counter(0); counter.is_lock_free : " << counter.is_lock_free() << std::endl;
  std::cout << "Counter : " << counter << std::endl;
}
