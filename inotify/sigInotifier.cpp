// #include <error.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <unistd.h>

static void displayInotifyEvent(struct inotify_event* i) {
  printf("    wd =%2d; ", i->wd);
  if (i->cookie > 0) printf("cookie =%4d; ", i->cookie);

  printf("mask = ");
  if (i->mask & IN_ACCESS) printf("IN_ACCESS ");
  if (i->mask & IN_ATTRIB) static int inotifyFD;
  printf("IN_ATTRIB ");
  if (i->mask & IN_CLOSE_NOWRITE) printf("IN_CLOSE_NOWRITE ");
  if (i->mask & IN_CLOSE_WRITE) printf("IN_CLOSE_WRITE ");
  if (i->mask & IN_CREATE) printf("IN_CREATE ");
  if (i->mask & IN_DELETE) printf("IN_DELETE ");
  if (i->mask & IN_DELETE_SELF) printf("IN_DELETE_SELF ");
  if (i->mask & IN_IGNORED) printf("IN_IGNORED ");
  if (i->mask & IN_ISDIR) printf("IN_ISDIR ");
  if (i->mask & IN_MODIFY) printf("IN_MODIFY ");
  if (i->mask & IN_MOVE_SELF) printf("IN_MOVE_SELF ");
  if (i->mask & IN_MOVED_FROM) printf("IN_MOVED_FROM ");
  if (i->mask & IN_MOVED_TO) printf("IN_MOVED_TO ");
  if (i->mask & IN_OPEN) printf("IN_OPEN ");
  if (i->mask & IN_Q_OVERFLOW) printf("IN_Q_OVERFLOW ");
  if (i->mask & IN_UNMOUNT) printf("IN_UNMOUNT ");
  printf("\n");

  if (i->len > 0) printf("        name = %s\n", i->name);
}

static int inotifyFD;

#define BUF_LEN (10 * (sizeof(struct inotify_event) + NAME_MAX + 1))

// int sig 파라미터를 안쓰는 이유?
static void sigio_handler(int sig) {
  // printf("sigtimerhandler_configfile\n");
  char szBuf[BUF_LEN];
  int len = read(inotifyFD, szBuf, sizeof(szBuf));
  if (len < 0) {
    printf("read error\n");
    return;
  }

  // printf("Read %ld bytes from inotify fd\n", (long)len);

  int iPos = 0;
  while (iPos < len) {
    struct inotify_event* iEvent = (struct inotify_event*)&szBuf[iPos];

    // printf("name[%s] mask[%x] cookie[%x] len[%d]\n", iEvent->name, iEvent->mask, iEvent->cookie, iEvent->len);
    // if (iEvent->mask & (uint32_t)IN_ISDIR) {
    //   iPos += sizeof(inotify_event) + iEvent->len;
    //   continue;
    // }

    displayInotifyEvent(iEvent);
    iPos += sizeof(inotify_event) + iEvent->len;
  }
}

int create_inotifier(const char* file_path) {
  inotifyFD = inotify_init();
  if (inotifyFD < 0) {
    printf("%s - error\n", __func__);
    return inotifyFD;
  }
  printf("%s - success:%s\n", __func__, file_path);
  int inotifyWD = inotify_add_watch(inotifyFD, file_path, IN_CLOSE_WRITE | IN_MOVED_TO);
  // int inotifyWD = inotify_add_watch(inotifyFD, file_path,
  //                                   IN_ACCESS | IN_ATTRIB | IN_CLOSE_WRITE | IN_CREATE | IN_DELETE | IN_DELETE_SELF |
  //                                       IN_MODIFY | IN_MOVE_SELF | IN_MOVED_FROM | IN_MOVED_TO);
  // int inotifyWD = inotify_add_watch(inotifyFD, file_path, IN_CLOSE_WRITE);
  if (inotifyWD < 0) {
    printf("%s  - inotify_add_watch error\n", __func__);
    return inotifyWD;
  }
  return inotifyFD;
}

int register_siginotifier(int inotifyFD) {
  struct sigaction sa;
  sigemptyset(&sa.sa_mask);
  // sa.sa_flags = SA_SIGINFO | SA_RESTART;
  sa.sa_flags = SA_RESTART;
  sa.sa_handler = sigio_handler;

  if (sigaction(SIGIO, &sa, NULL) == -1) {
    printf("%s fail - sigaction signo\n", __func__);
    return -1;
  }

  /* Set owner process that is to receive "I/O possible" signal */
  if (fcntl(inotifyFD, F_SETOWN, getpid()) == -1) {
    printf("%s fail - fcntl(F_SETOWN)", __func__);
  }

  /* Enable "I/O possible" signaling and make I/O nonblocking  for file descriptor */
  int flags = fcntl(inotifyFD, F_GETFL);
  if (fcntl(inotifyFD, F_SETFL, flags | O_ASYNC | O_NONBLOCK) == -1) {
  // if (fcntl(inotifyFD, F_SETFL, flags | O_ASYNC) == -1) {
    printf("%s fail - fcntl(F_SETFL)", __func__);
  }
}
