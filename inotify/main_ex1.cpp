#include <stdio.h>
#include <unistd.h>
#include <sys/inotify.h>

// struct inotify_event {
//     int      wd;       /* Watch descriptor */
//     uint32_t mask;     /* Mask describing event */
//     uint32_t cookie;   /* Unique cookie associating related events (for rename(2)) */
//     uint32_t len;      /* Size of name field */
//     char     name[];   /* Optional null-terminated name */
// }

// #define INOTIFY_PATH "/tmp"
// #define INOTIFY_PATH "/tmp/test.txt"
#define INOTIFY_PATH "../watchedfolder"
// #define INOTIFY_PATH "../watchedfolder/a.txt"


void handleInotify(struct inotify_event *psttEvent) {
  // .a.txt.swp 와 같이 . 으로 시작하는 파일 이름은 무시한다.
  if (psttEvent->name[0] == '.') return;

  printf("name[%s] mask[%x] cookie[%x] len[%d]", psttEvent->name, psttEvent->mask, psttEvent->cookie, psttEvent->len);

  if (psttEvent->mask & IN_ACCESS) printf(" IN_ACCESS");
  if (psttEvent->mask & IN_ATTRIB) printf(" IN_ATTRIB");
  if (psttEvent->mask & IN_CLOSE_WRITE) printf(" IN_CLOSE_WRITE");
  if (psttEvent->mask & IN_CLOSE_NOWRITE) printf(" IN_CLOSE_NOWRITE");
  if (psttEvent->mask & IN_CREATE) printf(" IN_CREATE");
  if (psttEvent->mask & IN_DELETE) printf(" IN_DELETE");
  if (psttEvent->mask & IN_DELETE_SELF) printf(" IN_DELETE_SELF");
  if (psttEvent->mask & IN_MODIFY) printf(" IN_MODIFY");
  if (psttEvent->mask & IN_MOVE_SELF) printf(" IN_MOVE_SELF");
  if (psttEvent->mask & IN_MOVED_FROM) printf(" IN_MOVED_FROM");
  if (psttEvent->mask & IN_MOVED_TO) printf(" IN_MOVED_TO");
  if (psttEvent->mask & IN_OPEN) printf(" IN_OPEN");

  printf("\n");
}

int main(int argc, char *argv[])
{
  int iFd = inotify_init();
  if (iFd == -1) {
    printf("inotify_init error\n");
    return 0;
  }

  // 파일 변경을 감시할 디렉토리를 설정한다.
  int n = inotify_add_watch(iFd, INOTIFY_PATH, IN_ACCESS | IN_ATTRIB | IN_CLOSE_WRITE | IN_CLOSE_NOWRITE | IN_CREATE | IN_DELETE | IN_DELETE_SELF | IN_MODIFY | IN_MOVE_SELF | IN_MOVED_FROM | IN_MOVED_TO | IN_OPEN);
  if (n == -1) {
    printf("inotify_add_watch error\n");
  }
  else {
    char szBuf[8192];

    while (1)
    {
      n = read(iFd, szBuf, sizeof(szBuf));
      if (n < 0) {
        printf("read error\n");
        break;
      }

      int iPos = 0;
      while (iPos < n) {
        struct inotify_event *psttEvent = (struct inotify_event *)&szBuf[iPos];
        handleInotify(psttEvent);
        iPos += sizeof(inotify_event) + psttEvent->len;
      }
    }
  }

  close(iFd);

  return 0;
}
