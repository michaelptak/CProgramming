#include <stdio.h>
#include <utmp.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define SHOWHOST
void show_info();

int main()
{
  struct utmp current_record;
  int utmpfd;
  int reclen = sizeof(current_record);

  if( (utmpfd = open(UTMP_FILE, O_RDONLY)) == -1){
    perror(UTMP_FILE);
    exit(1);
  }
  while(read(utmpfd, &current_record, reclen) == reclen) {
    show_info(&current_record);
  }
  close(utmpfd);

  return 0;
}

void show_info(struct utmp *utbufp) {
    printf("%-8.8s", utbufp->ut_user); // Username
    printf(" ");
    printf("%-8.8s", utbufp->ut_line); // Device name of tty
    printf(" ");
    printf("%-8d", utbufp->ut_tv.tv_sec); // ut_time
    printf(" ");
#ifdef SHOWHOST
    printf("%-8s", utbufp->ut_host); // host name for remote login
    printf(" ");
#endif
    printf("\n");
}
