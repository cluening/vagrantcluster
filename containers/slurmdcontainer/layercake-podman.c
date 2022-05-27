#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <sched.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <slurm/spank.h>

SPANK_PLUGIN(layercake-podman, 1)

pid_t get_job_container_pid(){
  pid_t pid;
  FILE *fp;

  fp = fopen("/run/layercake/job_container_pid", "r");
  fscanf(fp, "%d", &pid);
  fclose(fp);

  return pid;
}


int slurm_spank_init(spank_t sp, int ac, char **av){
  slurm_info("Plugin initializing");

  slurm_info("Number of options: %d", ac);

  return 0;
}


int slurm_spank_task_init(spank_t sp, int ac, char **av){
  slurm_info("in slurm_spank_task_init");

  int filedescriptor, result;
  pid_t processid = get_job_container_pid();
  char mntnspath[255];

  printf("Here goes!\n");

  snprintf(mntnspath, 255, "/proc/%d/ns/mnt", processid);

  filedescriptor = open(mntnspath, O_RDONLY);

  result = setns(filedescriptor, 0);

  printf("Result: %d\n", result);

  return 0;
}
