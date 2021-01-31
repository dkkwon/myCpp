
### Goal
- multi-thread 환경에서 signal 사용

### Ref
- https://noritor82.tistory.com/entry/multithread-%ED%99%98%EA%B2%BD%EC%97%90%EC%84%9C-signal-%EC%82%AC%EC%9A%A9
- https://www.joinc.co.kr/w/Site/Thread/Beginning/PthreadApiReference#AEN230
- https://man7.org/linux/man-pages/man7/signal.7.html

### Test
```
*Process PID = 5298
Create thread : tid = 140008690276096
* Start signal thread (tid = 140008690276096)
Create thread : tid = 140008681883392
Create thread : tid = 140008673490688
        [140008690276096] Signal(USR1)
```
```
➜  mthread_signal git:(master) ✗ ps -ef | grep test_dk
dk        5298  5176  0 17:33 pts/21   00:00:00 ./test_dk
➜  mthread_signal git:(master) ✗ kill -SIGUSR1 5298
```

### 정리
- 시그널 (Singal)
  - 비동기 이벤트 처리를 위한 메커니즘을 제공하는 소프트웨어 인터럽트
  - 시스템 외부(Ctrl+C 입력) 또는 시스템 내부(`divide by 0` 등) 발생
  - 기본적인 프로세스 간 통신방식
- 핸들러를 등록해서 사용
  - typedef void (*sighandler_t)(int)
  - struct sigaction {...}
  ```
  #define	SIG_ERR	 ((__sighandler_t) -1)	/* Error return.  */
  #define	SIG_DFL	 ((__sighandler_t)  0)	/* Default action.  */
  #define	SIG_IGN	 ((__sighandler_t)  1)	/* Ignore signal.  */
  ```
- 지원 시그널 리스트 `$ kill -l`
- 시그널 보내기
```
int kill (pid_t pid, int signo)
int killpg (int pgrp, int signo)
```
- fork() vs exec()
  - 부모 프로세스 시그널 동작을 상속받음
- 재진입성 가능 함수
  - signal-safe : 재진입 가능하고, signal handler 에서 사용가능
- 추가
  - 쓰레드에서 시그널은 서로 공유됨
  - 프로세스에 시그널이 전달되면 프로세스가 생성된 모든 쓰레드로 시그널이 전달
  - 특정 쓰레드만 시그널을 받도록 하고 싶을 때 --> `pthread_sigmask`
