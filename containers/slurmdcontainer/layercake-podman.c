#define _GNU_SOURCE
#include <limits.h>
#include <stdio.h>
#include <unistd.h>
#include <sched.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <slurm/spank.h>
#include "podman.h"


SPANK_PLUGIN(layercake-podman, 1)


pid_t _get_job_container_pid(){
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


int slurm_spank_init_post_opt(spank_t sp, int ac, char **av){
  char hostname[HOST_NAME_MAX];

  char imagename[] = "head:5000/jobcontainer:latest";
  char containername[] = "libcurljobcontainer";

  slurm_info("In slurm_spank_init_post_opt: uid %d", getuid());

  gethostname(hostname, HOST_NAME_MAX);

  _pull_job_container(imagename);
  _create_job_container(imagename, hostname, containername);
  _start_job_container(containername);
  _wait_job_container(containername, "running");
 
  // FIXME: there's a race condition here between the container entering "running" state and the pid file actually getting written out
  sleep(2);

  return 0;
}


int slurm_spank_task_init_privileged(spank_t sp, int ac, char **av){
  int filedescriptor, result;
  pid_t processid;
  char mntnspath[255];

  slurm_info("In slurm_spank_task_init_privileged: uid %d", getuid());

  processid = _get_job_container_pid();

  snprintf(mntnspath, 255, "/proc/%d/ns/mnt", processid);
  filedescriptor = open(mntnspath, O_RDONLY);
  result = setns(filedescriptor, 0);

  slurm_info("setns result: %d\n", result);

  return 0;
}


int slurm_spank_task_init(spank_t sp, int ac, char **av){


  slurm_info("In slurm_spank_task_init: uid %d", getuid());


}


int slurm_spank_exit(spank_t sp, int ac, char **av){
  char imagename[] = "jobcontainer";
  char containername[] = "libcurljobcontainer";

  slurm_info("in slurm_spank_exit");

  _kill_job_container(containername);
  _wait_job_container(containername, "exited");
  _delete_job_container(containername);

  slurm_info("job container cleaned up");

  return 0;
}
