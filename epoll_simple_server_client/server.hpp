#ifndef __SERVER_HPP__
#define __SERVER_HPP__

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/sctp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/file.h>
#include <sys/inotify.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <unistd.h>

#include <atomic>
#include <cerrno>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <future>
#include <iostream>
#include <iterator>
#include <map>
#include <mutex>
#include <random>
#include <shared_mutex>
#include <thread>

#define LOG_I(...)
#define LOG_E(...)
#define LOG_D(...)

// #define LOG_I(...) {fprintf(stderr, "[INFO]"); fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n");}
// #define LOG_E(...) {fprintf(stderr, "[ERROR]"); fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n");}
// #define LOG_D(...) {fprintf(stderr, "[DEBUG]"); fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n");}

#define MAXEVENTS 128
#define PORT_NUM 3600
#define EPOLL_SIZE 20
#define MAXLINE 1024

#define MAX_ENODB_NAME_SIZE 64

typedef struct sctp_params {
  int epollTimeOut = -1;
  int epoll_fd = 0;
  int listenFD = 0;
  int tcpPort = PORT_NUM;
  std::string myIP{};
} sctp_params_t;

typedef struct ConnectedCU {
  int fileDescriptor = 0;
  char hostName[NI_MAXHOST]{};
  char portNumber[NI_MAXSERV]{};
  char enodbName[MAX_ENODB_NAME_SIZE]{};
  // char asnData[RECEIVE_SCTP_BUFFER_SIZE] {};
  // size_t asnLength = 0;
  // int mtype = 0;
  bool isConnected = false;
  bool gotSetup = false;
  sctp_params_t *sctpParams = nullptr;
  // Counter *counters[6][2][ProcedureCode_id_RICsubscriptionDelete + 1] {};
} ConnectedCU_t;

#endif

/*
#if 0
struct sockaddr_in addr, clientaddr;
struct epoll_event ev, *events;  // ev는 listen 소켓의 사건, *event는
struct udata *user_data;         // user들의 데이터가 포인터로 처리가 가능하다.

int listenfd;
int clientfd;
int i;
socklen_t addrlen, clilen;
int readn;
int eventn;
int epollfd;
char buf[MAXLINE];

events = (struct epoll_event *)calloc(sizeof(struct epoll_event) * EPOLL_SIZE);

addrlen = sizeof(addr);
if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) return 1;
addr.sin_family = AF_INET;
addr.sin_port = htons(PORT_NUM);
addr.sin_addr.s_addr = htonl(INADDR_ANY);
if (bind(listenfd, (struct sockaddr *)&addr, addrlen) == -1) return 1;

listen(listenfd, 5);

// EPOLL_CTL_ADD를 통해 listen 소켓을 이벤트 풀에 추가시켰다.
ev.events = EPOLLIN;                               // 이벤트가 들어오면 알림
ev.data.fd = listenfd;                             // 듣기 소켓을 추가한다.
epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &ev);  // listenfd의 상태변화를 epollfd를 통해 관찰
memset(user_fds, -1, sizeof(int) * 1024);
#endif

#if 0

    if (eventn == -1) {
      return 1;
    }
    for (i = 0; i < eventn; i++) {
      if (events[i].data.fd == listenfd)  // 듣기 소켓에서 이벤트가 발생함
      {
        memset(buf, 0x00, MAXLINE);
        clilen = sizeof(struct sockaddr);
        clientfd = accept(listenfd, (struct sockaddr *)&clientaddr, &clilen);
        user_fds[clientfd] = 1;  // 연결 처리

        user_data = (struct udata *)malloc(sizeof(user_data));
        user_data->fd = clientfd;

        char *tmp = "First insert your nickname :";
        write(user_data->fd, tmp, 29);

        sleep(1);
        read(user_data->fd, user_data->name, sizeof(user_data->name));
        user_data->name[strlen(user_data->name) - 1] = 0;

        printf("Welcome [%s]  \n", user_data->name);

        sleep(1);
        sprintf(buf, "Okay your nickname : %s\n", user_data->name);

        write(user_data->fd, buf, 40);

        ev.events = EPOLLIN;
        ev.data.ptr = user_data;

        epoll_ctl(epollfd, EPOLL_CTL_ADD, clientfd, &ev);
      } else  // 연결 소켓에서 이벤트가 발생함
      {
        user_data = (struct udata *)events[i].data.ptr;
        memset(buf, 0x00, MAXLINE);
        readn = read(user_data->fd, buf, MAXLINE);
        if (readn <= 0)  // 읽는 도중 에러 발생
        {
          epoll_ctl(epollfd, EPOLL_CTL_DEL, user_data->fd, events);
          close(user_data->fd);
          user_fds[user_data->fd] = -1;
          free(user_data);
        } else  // 데이터를 읽는다.
        {
          send_msg(events[i], buf);
        }
      }
    }
#endif
*/
