#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

struct login {
  char user_name[100];
  time_t time_stamp;
  char terminal[100];
};

#define LOGIN_FILE "login_file.txt"

int main(){
  struct login currentLogin;
  int intLoginFile;
  intLoginFile=creat(LOGIN_FILE,S_IRUSR | S_IWUSR);

  if(intLoginFile==-1){
    perror(LOGIN_FILE);
    return 1;
  }

  for(int i=0; i<3; i++) {
    sprintf(currentLogin.user_name, "user_%d", i);
    sprintf(currentLogin.terminal, "terminal_%d", (10+i));
    time(&currentLogin.time_stamp);
    write(intLoginFile, &currentLogin, sizeof(struct login));
    sleep(1);
  }
  close(intLoginFile);

  intLoginFile=open(LOGIN_FILE, O_RDONLY);
  if(intLoginFile==-1){
    perror(LOGIN_FILE);
    return 1;
  }

  for(int i=0; i<3; i++) {
    read(intLoginFile, &currentLogin, sizeof(struct login));
    printf("%s last logged in on %s on %.24s\n", currentLogin.user_name, currentLogin.terminal, 
      asctime(localtime(&currentLogin.time_stamp)));
  }
  close(intLoginFile);

  return 0;
}
