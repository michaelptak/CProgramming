#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

struct login {
  char user_name[100];
  //time_t time_stamp;

};

#define LOGIN_FILE "login_file.txt"

int main(){
  struct login currentLogin;
  int intLoginFile;
  intLoginFile=creat(LOGIN_FILE,S_IRUSR | S_IWUSR);
  for(int i=0; i<3; i++) {
    strcpy(currentLogin.user_name, "John");
    write(intLoginFile, &currentLogin, sizeof(struct login));
  }
  close(intLoginFile);

  return 0;
}
