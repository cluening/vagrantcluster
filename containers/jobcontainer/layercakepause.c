#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>


int main(){
  FILE *fp;
  pid_t pid;
  char *jobid;
  char pathtemplate[] = "/run/layercake/job_container_pid.%s";
  char path[255];

  pid = getpid();
  jobid = getenv("LAYERCAKE_JOBID");
  
  if(jobid == NULL){
    printf("Did not get layercake job ID\n");
    exit(1);
  }

  snprintf(path, 255, pathtemplate, jobid);

  fp = fopen(path, "w");
  if(fp == NULL){
    printf("opening job container pid file failed: %s\n", strerror(errno));
    exit(1);
  }

  fprintf(fp, "%d", pid);  
  fclose(fp);

  for(;;)
    pause();
}
