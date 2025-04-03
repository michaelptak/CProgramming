#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

void mode_to_string(int mode, char str[]) {
    strcpy(str, "----------");
    if (S_ISDIR(mode)) str[0] = 'd';
    if (S_ISCHR(mode)) str[0] = 'c';
    if (S_ISBLK(mode)) str[0] = 'b';
    if (S_ISLNK(mode)) str[0] = 'l';
    if (mode & S_IRUSR) str[1] = 'r';
    if (mode & S_IWUSR) str[2] = 'w';
    if (mode & S_IXUSR) str[3] = 'x';
    if (mode & S_IRGRP) str[4] = 'r';
    if (mode & S_IWGRP) str[5] = 'w';
    if (mode & S_IXGRP) str[6] = 'x';
    if (mode & S_IROTH) str[7] = 'r';
    if (mode & S_IWOTH) str[8] = 'w';
    if (mode & S_IXOTH) str[9] = 'x';
}

char *pattern;
void do_find(char *dir_name);

void process_entry(char *full_path, char *entry_name, const char *cur_dir, int *printed) {
  struct stat info;
  if (stat(full_path, &info) == -1)
    perror(full_path);
  else {
    if (strstr(entry_name, pattern) != NULL) {
      if (!(*printed)) {
          printf("%s\n", cur_dir);
          *printed = 1;
      }
      if (!S_ISDIR(info.st_mode)) {
          char mode_str[11];
          mode_to_string(info.st_mode, mode_str);
          printf("   %s (%04o/%s)\n", entry_name, info.st_mode & 0777, mode_str);
      }
    }
    if (S_ISDIR(info.st_mode)) {
      if (strcmp(entry_name, ".") == 0 || strcmp(entry_name, "..") == 0)
        return;
      do_find(full_path);
    }
  }
}

void do_find(char *dir_name) {
  DIR *dir_ptr;
  struct dirent *dirent_ptr;
  int printed = 0;  // flag: has the directory header been printed?
  dir_ptr = opendir(dir_name);
  if(dir_ptr==0){
    perror(dir_name);
  }
  else {
    while((dirent_ptr = readdir(dir_ptr)) != 0) {
      char path[1024];
      snprintf(path, sizeof(path), "%s/%s", dir_name, dirent_ptr->d_name);
      // Pass dir_name and address of printed flag to process_entry
      process_entry(path, dirent_ptr->d_name, dir_name, &printed);
    }
    closedir(dir_ptr);
  }
}

int main(int ac, char *av[]) {
  if (ac < 2) {
        printf("Usage: %s filename [startpath]\n", av[0]);
        return 1;
    }
  pattern = av[1];

  char *startpath;
  if (ac >= 3) {
    startpath = av[2];
  } else {
    startpath = ".";
  }

  do_find(startpath);
  
  return 0;
}
