#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>

void mode_to_string(int mode, char str[]) {
  strcpy(str, "----------");

  if(S_ISDIR(mode)) str[0] = 'd';
  if(S_ISCHR(mode)) str[0] = 'c';
  if(S_ISBLK(mode)) str[0] = 'b';
  if(S_ISLNK(mode)) str[0] = '1';

  if(mode & S_IRUSR) str[1] = 'r';
  if(mode & S_IWUSR) str[2] = 'w';
  if(mode & S_IXUSR) str[3] = 'x';
  
  if(mode & S_IRGRP) str[4] = 'r';
  if(mode & S_IWGRP) str[5] = 'w';
  if(mode & S_IXGRP) str[6] = 'x';

  if(mode & S_IROTH) str[7] = 'r';
  if(mode & S_IWOTH) str[8] = 'w';
  if(mode & S_IXOTH) str[9] = 'x';

}

void show_dir_entry(char *dir_entry_name, struct stat *info){
  char modestring[11];
  mode_to_string(info->st_mode, modestring);
  //printf("Dir entry: %s, size: %ld\n", dir_entry_name, info->st_size);
  printf("%s ",modestring);
  printf("%4d ", (int)info->st_nlink);
  printf("%8d ", (int)info->st_size);
  printf("%.20s ", ctime(&info->st_mtime));
  printf("%s\n", dir_entry_name);

}

void do_stat(char *dir_entry_name){
  struct stat info;
  if(stat(dir_entry_name, &info)==-1) {
    perror(dir_entry_name);
  } 
  else {
    show_dir_entry(dir_entry_name, &info);
    //printf("Dir entry: %s, size: %ld\n", dir_entry_name, info->st_size);
  }
}


void do_ls(char *dirname){
  DIR *dir_ptr;
  struct dirent *dirent_ptr;

  dir_ptr=opendir(dirname);
  if(dir_ptr==0){
    perror(dirname);
  } 
  else {
    while ((dirent_ptr = readdir(dir_ptr)) != 0) {
      //printf("%s\n", dirent_ptr->d_name);
      do_stat(dirent_ptr->d_name);
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
