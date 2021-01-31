#include "server.hpp"

// user data struct
struct udata {
  int fd;
  char name[80];
};

int user_fds[1024];

// client가 보낸 메시지를 다른 client들에게 전송한다.
void send_msg(struct epoll_event ev, char *msg) {
  int i;
  char buf[MAXLINE + 24];
  struct udata *user_data;
  user_data = (struct udata *)ev.data.ptr;
  for (i = 0; i < 1024; i++) {
    memset(buf, 0x00, MAXLINE + 24);
    sprintf(buf, "%s : %s", user_data->name, msg);
    if ((user_fds[i] == 1)) {
      write(i, buf, MAXLINE + 24);
    }
  }
}

/***********************************************************/

int setSocketNoBlocking(int socket) {
  auto flags = fcntl(socket, F_GETFL, 0);

  if (flags == -1) {
    LOG_E("%s, %s", __FUNCTION__, strerror(errno));
    return -1;
  }

  flags = (unsigned)flags | (unsigned)O_NONBLOCK;
  if (fcntl(socket, F_SETFL, flags) == -1) {
    LOG_E("%s, %s", __FUNCTION__, strerror(errno));
    return -1;
  }

  return 0;
}

void listener(sctp_params_t *params) {
  LOG_D("create listner thread");
  struct epoll_event *events = (struct epoll_event *)calloc(MAXEVENTS,sizeof(struct epoll_event) * EPOLL_SIZE);

  while (true) {
    int numOfEvents = epoll_wait(params->epoll_fd, events, MAXEVENTS, params->epollTimeOut);

    if (numOfEvents == 0) {
      LOG_D("got epoll timeout");
      continue;
    } else if (numOfEvents < 0) {
      if (errno == EINTR) {
        LOG_D("got EINTR : %s", strerror(errno));
        continue;
      }
      LOG_E("Epoll wait failed, errno = %s", strerror(errno));
      return;
    }

    for (auto i = 0; i < numOfEvents; i++) {
      LOG_D("handling epoll event %d out of %d", i + 1, numOfEvents);

      if ((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP)) {
        LOG_E("epoll error, events(%0x)", events[i].events);
      } else if (events[i].events & EPOLLOUT) {
        LOG_I("file descriptor got EPOLLOUT", );
      } else if (params->listenFD == events[i].data.fd) {
        LOG_I("New connection request from network\n");

        while (true) {
          struct sockaddr in_addr {};
          socklen_t in_len;
          char hostBuff[NI_MAXHOST];
          char portBuff[NI_MAXSERV];

          in_len = sizeof(in_addr);
          ConnectedCU_t* peerInfo = (ConnectedCU_t *)calloc(1, sizeof(ConnectedCU_t));
          if (peerInfo == nullptr) {
            LOG_E("calloc failed");
            break;
          }
          peerInfo->sctpParams = params;
          peerInfo->fileDescriptor = accept(params->listenFD, &in_addr, &in_len);
          if (peerInfo->fileDescriptor == -1) {
            if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
              LOG_E("Accept error, errno = %s", strerror(errno));
              /* We have processed all incoming connections. */
              break;
            } else {
              LOG_E("Accept error, errno = %s", strerror(errno));
              break;
            }
          }
          if (setSocketNoBlocking(peerInfo->fileDescriptor) == -1) {
            LOG_E("setSocketNoBlocking failed to set new connection %s on port %s\n", hostBuff, portBuff);
            close(peerInfo->fileDescriptor);
            break;
          }
          auto ans = getnameinfo(&in_addr, in_len, peerInfo->hostName, NI_MAXHOST, peerInfo->portNumber, NI_MAXSERV,
                                 (unsigned)((unsigned int)NI_NUMERICHOST | (unsigned int)NI_NUMERICSERV));
          if (ans < 0) {
            LOG_E("Failed to get info on connection request. %s\n", strerror(errno));
            close(peerInfo->fileDescriptor);
            break;
          }
          LOG_D("Accepted connection on descriptor %d (host=%s, port=%s)\n", peerInfo->fileDescriptor,
                peerInfo->hostName, peerInfo->portNumber);
          peerInfo->isConnected = false;
          peerInfo->gotSetup = false;
        }
      }
    }
  }
}

int main(int argc, char **argv) {
  sctp_params_t sctpParams;

  sctpParams.epoll_fd = epoll_create1(0);
  if (sctpParams.epoll_fd == -1) {
    LOG_E("failed to open epoll descriptor");
    exit(-1);
  }

  // set socket to listen tcp
  sctpParams.listenFD = socket(AF_INET, SOCK_STREAM, 0);
  if (sctpParams.listenFD <= 0) {
    LOG_E("Error Opening socket, %s", strerror(errno));
    return -1;
  }

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  addr.sin_port = htons(sctpParams.tcpPort);

  if (bind(sctpParams.listenFD, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    LOG_E("Error binding port %d. %s", PORT_NUM, strerror(errno));
    return -1;
  }

  // TODO: Why is it needed ?
  if (setSocketNoBlocking(sctpParams.listenFD) == -1) {
    LOG_E("Error binding. %s", strerror(errno));
    return -1;
  }

  if (listen(sctpParams.listenFD, SOMAXCONN) < 0) {
    LOG_E("Error listening. %s\n", strerror(errno));
    return -1;
  }

  // add listening port to epoll
  struct epoll_event event {};
  event.events = EPOLLIN | EPOLLET;
  event.data.fd = sctpParams.listenFD;

  if (epoll_ctl(sctpParams.epoll_fd, EPOLL_CTL_ADD, sctpParams.listenFD, &event)) {
    printf("Failed to add descriptor to epoll\n");
    LOG_E("Failed to add descriptor to epoll. %s\n", strerror(errno));
    return -1;
  }

  // run multi-thread
  unsigned num_cpus = std::thread::hardware_concurrency();
  std::vector<std::thread> threads(num_cpus);

  num_cpus = 3;
  for (unsigned int i = 0; i < num_cpus; i++) {
    threads[i] = std::thread(listener, &sctpParams);

    // cpu_set_t cpuset;
    // CPU_ZERO(&cpuset);
    // CPU_SET(i, &cpuset);
    // int rc = pthread_setaffinity_np(threads[i].native_handle(), sizeof(cpu_set_t), &cpuset);
    // if (rc != 0) {
    //   LOG_E("Error calling pthread_setaffinity_np: %d", rc);
    // }
  }

  for (auto &t : threads) {
    t.join();
  }
}
