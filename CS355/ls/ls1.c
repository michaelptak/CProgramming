#include <stdio.h>
#include <dirent.h>

void do_ls(char *dirname){
  DIR *dir_ptr;
  struct dirent *dirent_ptr;

  dir_ptr=opendir(dirname);
  if(dir_ptr==0){
    perror(dirname);
  } 
  else {
    while ((dirent_ptr = readdir(dir_ptr)) != 0) {
      printf("%s\n", dirent_ptr->d_name);
    }
    closedir(dir_ptr);
  }

  
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf("Enter one directory\n");
    return -1;
  }
  do_ls(argv[1]);

  return 0;
}
