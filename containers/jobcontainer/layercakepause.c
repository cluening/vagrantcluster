#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>


int main(){
  FILE *fp;
  pid_t pid;

  pid = getpid();

  fp = fopen("/run/layercake/job_container_pid", "w");
  if(fp == NULL){
    printf("opening job container pid file failed: %s\n", strerror(errno));
    exit(1);
  }

  fprintf(fp, "%d", pid);  
  fclose(fp);

  for(;;)
    pause();
}
