#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#define BASE_DIR "/var"
#define DUPLICATE_DIR "/mnt1"
#define handle_error(msg) do { perror(msg); exit(EXIT_FAILURE); } while (0)

char *exclude[] = { 
".",
".."
};

int should_ignore(char *n) {
  int iter_total = sizeof(exclude)/sizeof(char*);

  for(int i=0;i<iter_total;i++)
    if(strcmp(n,exclude[i]) == 0)
      return 1;

  return 0;
}

int make_sock(struct dirent *dp,struct stat *s) {
  printf("%s is sock\n",dp->d_name);
  return 0;
}

int make_reg_file(struct dirent *dp,struct stat *s) {
  printf("%s is reg file\n",dp->d_name);
  return 0;
}

int make_link(struct dirent *dp,struct stat *s) {
  printf("%s is link\n",dp->d_name);
  return 0;
}

int make_fifo(struct dirent *dp,struct stat *s) {
  printf("%s is fifo\n",dp->d_name);
  return 0;
}

int make_dir(struct dirent *dp,struct stat *s) {
  printf("%s is dir\n",dp->d_name);
  return 0;
}

int make_char_device(struct dirent *dp,struct stat *s) {
  printf("%s is char device\n",dp->d_name);
  return 0;
}

int make_block_device(struct dirent *dp,struct stat *s) {
  //printf("%s is block device\n",dp->d_name);

  return 0;
}

int main() {
  DIR *dp = NULL;
  struct dirent *d_ent_ptr= NULL;
  int dir_fd = -1;
  struct stat stat_buf ;

  dp = opendir(BASE_DIR);

  if( dp == NULL )
    handle_error("opendir");

  dir_fd = dirfd(dp);

  errno = 0;
  while( d_ent_ptr= readdir(dp) ) {
    if( should_ignore(d_ent_ptr->d_name) ) 
      continue;

    if( fstatat(dir_fd,d_ent_ptr->d_name,&stat_buf,AT_SYMLINK_NOFOLLOW) )
      handle_error("fsstatat");

#ifdef DEBUG
    printf("name %s,mode =%o\n",d_ent_ptr->d_name,stat_buf.st_mode) ;
    //printf("permission=%lo,uid=%d,gid=%d\n",(unsigned long)stat_buf.st_mode,stat_buf.st_uid,stat_buf.st_gid);
    continue;
#endif

    if( S_ISBLK(stat_buf.st_mode)) {
      make_block_device(d_ent_ptr,&stat_buf);
    } else if( S_ISCHR(stat_buf.st_mode)) {
      make_char_device(d_ent_ptr,&stat_buf);
    } else if( S_ISFIFO(stat_buf.st_mode)) {
      make_fifo(d_ent_ptr,&stat_buf);
    } else if( S_ISLNK(stat_buf.st_mode)) {
      make_link(d_ent_ptr,&stat_buf);
    } else if( S_ISREG(stat_buf.st_mode)) {
      make_reg_file(d_ent_ptr,&stat_buf);
    } else if( S_ISSOCK(stat_buf.st_mode)) {
      make_sock(d_ent_ptr,&stat_buf);
    } else if( S_ISDIR(stat_buf.st_mode)) {
      make_dir(d_ent_ptr,&stat_buf);
    } else {
      handle_error("unknown type fstatat");
    }
    errno = 0;
  }
  if(errno) {
    handle_error("readdir");
  }
}
