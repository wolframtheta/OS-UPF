#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(){
  int fd[2];
  pipe(fd);
  int pid = fork();
  if(pid==0){
    close(fd[0]);
    dup2(fd[1],1);
    char * cmd[] = {"./text2bin",NULL};
    execvp(cmd[0],&cmd[0]); //this kill child process
    _exit(0);
  }
  
  wait(NULL);
  close(fd[1]);
  dup2(fd[0],0);
  char* cmd[] = {"./text2bin",NULL};
  
  execvp(cmd[0],&cmd[0]);



  return 0;
}
