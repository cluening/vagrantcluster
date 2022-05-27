#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>


int main(){
  FILE *fp;
  pid_t pid;

  pid = getpid();

  fp = fopen("/run/layercake/job_container_pid", "w");
  fprintf(fp, "%d", pid);  
  fclose(fp);

  for(;;)
    pause();
}
