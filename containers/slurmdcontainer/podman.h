int _pull_job_container(char *imagename);
int _create_job_container(char *imagename, char *podname, char *containername, uint32_t jobid);
int _start_job_container(char *containername);
int _wait_job_container(char *containername, char *containerstate);
int _kill_job_container(char *containername);
int _delete_job_container(char *containername);
