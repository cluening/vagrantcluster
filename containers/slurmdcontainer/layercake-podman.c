#define _GNU_SOURCE
#include <limits.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <sched.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <slurm/spank.h>
#include "podman.h"


SPANK_PLUGIN(layercake-podman, 1)


pid_t _get_job_container_pid(uint32_t jobid){
  pid_t pid;
  FILE *fp;
  char pathtemplate[] = "/run/layercake/job_container_pid.%d";
  char path[255];

  snprintf(path, 255, pathtemplate, jobid);

  fp = fopen(path, "r");

  if(fp == NULL){
    slurm_info("opening job container pid file failed: %s", strerror(errno));
    return -1;
  }

  fscanf(fp, "%d", &pid);
  fclose(fp);

  return pid;
}


int _remove_job_container_pid_file(uint32_t jobid){
  char pathtemplate[] = "/run/layercake/job_container_pid.%d";
  char path[255];
  int result;

  snprintf(path, 255, pathtemplate, jobid);

  result = unlink(path);

  return result;
}


int slurm_spank_init(spank_t sp, int ac, char **av){
  slurm_info("Plugin initializing");

  slurm_info("Number of options: %d", ac);

  return 0;
}


int slurm_spank_init_post_opt(spank_t sp, int ac, char **av){
  int result;
  char hostname[HOST_NAME_MAX];
  spank_err_t rc;
  uint32_t jobid;

  char imagename[] = "head:5000/jobcontainer:latest";
  char containername[] = "libcurljobcontainer";

  slurm_info("In slurm_spank_init_post_opt: uid %d", getuid());

  if(spank_remote(sp)){
    gethostname(hostname, HOST_NAME_MAX);

    rc = spank_get_item(sp, S_JOB_ID, &jobid);

    slurm_info("Job id: %d", jobid);

    result = _pull_job_container(imagename);
    if(result != 0){
      slurm_info("Failed to pull job container image");
      return -1;
    }

    result = _create_job_container(imagename, "layercake", containername, jobid);
    if(result != 0){
      slurm_info("Failed to create job container");
      return -1;
    }

    result = _start_job_container(containername);
    if(result != 0){
      slurm_info("Failed to start job container");
      return -1;
    }

    result = _wait_job_container(containername, "running");
    if(result != 0){
      slurm_info("Failed to wait for job container");
      return -1;
    }

    // FIXME: there's a race condition here between the container entering "running" state and the pid file actually getting written out
    sleep(2);

  }
  return 0;
}


int slurm_spank_task_init_privileged(spank_t sp, int ac, char **av){
  int errornumber, filedescriptor, result;
  pid_t processid;
  char mntnspath[PATH_MAX];
  spank_err_t rc;
  uint32_t jobid;

  slurm_info("In slurm_spank_task_init_privileged: uid %d", getuid());

  if(spank_remote(sp)){
    rc = spank_get_item(sp, S_JOB_ID, &jobid);

    processid = _get_job_container_pid(jobid);
    if(processid < 0){
      slurm_info("Failed to get job container pid");
      return -1;
    }

    snprintf(mntnspath, PATH_MAX, "/proc/%d/ns/mnt", processid);
    filedescriptor = open(mntnspath, O_RDONLY);
    result = setns(filedescriptor, 0);

    if(result == 0){
      slurm_info("setns() succeeded");
    } else {
      errornumber = errno;
      slurm_info("setns() failed: %s", strerror(errornumber));
      return -1;
    }
  }

  return 0;
}


int slurm_spank_task_init(spank_t sp, int ac, char **av){
  slurm_info("In slurm_spank_task_init: uid %d", getuid());
}


int slurm_spank_exit(spank_t sp, int ac, char **av){
  int result;
  spank_err_t rc;
  uint32_t jobid;

  char imagename[] = "jobcontainer";
  char containername[] = "libcurljobcontainer";

  slurm_info("in slurm_spank_exit");

  if(spank_remote(sp)){
    result = _kill_job_container(containername);
    if(result != 0){
      slurm_info("Failed to kill job container");
      return -1;
    }

    result = _wait_job_container(containername, "exited");
    if(result != 0){
      slurm_info("Failed to wait for job container");
      return -1;
    }

    result = _delete_job_container(containername);
    if(result != 0){
      slurm_info("Failed to delete job container");
      return -1;
    }

    rc = spank_get_item(sp, S_JOB_ID, &jobid);
    _remove_job_container_pid_file(jobid);

    slurm_info("job container cleaned up");
  }

  return 0;
}
