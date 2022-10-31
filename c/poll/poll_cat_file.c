#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <error.h>
#include <poll.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

void errExit(char *msg) {
  perror(msg);
  exit(-1);
}

void printArgs(char **argv,char **env) {
  for(int i=0;argv[i];i++)
    printf("argv[%d]=%s\n",i,argv[i]);

  for(int i=0;env[i];i++)
    printf("env[%d]=%s\n",i,env[i]);
}

int main(int argc, char *argv[],char *env[]) {
  int pipe_fds_child_stdin[2] ;
  int pipe_fds_child_stdout[2] ;
  pid_t child_id ;

  if( pipe(pipe_fds_child_stdin) < 0 )
    errExit("pipe");

  if( pipe(pipe_fds_child_stdout) < 0 )
    errExit("pipe");

  child_id = fork();

  if( child_id > 0 ) {
    const int MAX_POLL_FDS = 2;
    const int BUF_SIZE = 1;
    struct pollfd poll_fds[MAX_POLL_FDS] ;
    size_t readSize;
    char buf[BUF_SIZE];
    int status;

    memset(poll_fds,'\0',sizeof(poll_fds));
    close(pipe_fds_child_stdin[0]);
    close(pipe_fds_child_stdout[1]);

    poll_fds[0].fd      = pipe_fds_child_stdout[0];
    poll_fds[0].events  = POLLIN;
    poll_fds[1].fd      = STDIN_FILENO;
    poll_fds[1].events  = POLLIN;

    while(1) {
      if( poll(poll_fds,MAX_POLL_FDS,-1) < -1)
        errExit("poll");

      if( poll_fds[0].revents & POLLIN ) {
        readSize = read(poll_fds[0].fd,buf,BUF_SIZE);
        if( readSize == BUF_SIZE ) {
          write(STDOUT_FILENO,buf,BUF_SIZE);
        } else if(readSize == 0) { // eof
          break;
        } else  {
          errExit("read");
        }
      } else if( poll_fds[1].revents & POLLIN) {
        readSize = read(STDIN_FILENO,buf,BUF_SIZE);
        if( readSize == BUF_SIZE ) {
          write(pipe_fds_child_stdin[1],buf,BUF_SIZE);
        } else if(readSize == 0) { // eof
          close(pipe_fds_child_stdin[1]);
        } else  {
          errExit("read");
        }
      } else if( poll_fds[0].revents & POLLHUP) { //child closed its end
        wait(&status);
        exit(0);
      } else if( poll_fds[1].revents & POLLHUP) { 
        errExit("got POLLHUP on stdin!");
      } else if( poll_fds[0].revents & POLLNVAL) {
        errExit("got POLLNVAL on pipe_fds_child_stdout!");
      } else if( poll_fds[1].revents & POLLNVAL) {
        errExit("got POLLNVAL on stdin!");
      }
    }

  } else if( child_id == 0 ) {
    close(0);
    close(1);
    close(pipe_fds_child_stdin[1]);
    close(pipe_fds_child_stdout[0]);

    dup2(pipe_fds_child_stdin[0],0);
    dup2(pipe_fds_child_stdout[1],1);

    //execvpe(argv[1],&(argv[1]),env);
    execvp(argv[1],&(argv[1]));
    errExit("execvp");
  } else {
    errExit("fork");
  }
}
