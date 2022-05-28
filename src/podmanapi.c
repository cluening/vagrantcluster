/********* Sample code generated by the curl command line tool **********
 * All curl_easy_setopt() options are documented at:
 * https://curl.se/libcurl/c/curl_easy_setopt.html
 ************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>


/*
[root@node01 src]# curl -i -s -X POST -H "Content-Type: application/json" --unix
-socket /run/layercake/podman.sock 'http://h/v1.24/libpod/containers/create' -d 
'{ "image": "jobcontainer", "pod": "lc01", "name": "curljobcontainer", "mounts":
 [ {"Source": "/run/layercake", "Destination": "/run/layercake", "Type": "bind" 
} ] }'  
*/
int create_job_container(char *imagename, char *podname, char *containername){
  CURLcode ret;
  CURL *hnd;
  struct curl_slist *slist1;
  long responsecode;
  char *createjson;
  size_t createjsonlength;

  // char imagename[] = "jobcontainer";
  // char podname[] = "lc01";
  // char containername[] = "libcurljobcontainer";  

  char createjsontemplate[] = "{ \"image\": \"%s\", \"pod\": \"%s\", \"name\": \"%s\", \"mounts\": [ {\"Source\": \"/run/layercake\", \"Destination\": \"/run/layercake\", \"Type\": \"bind\" } ] }";

  createjsonlength = snprintf(NULL, 0, createjsontemplate, imagename, podname, containername);
  createjson = malloc((createjsonlength + 1) * sizeof(char));
  sprintf(createjson, createjsontemplate, imagename, podname, containername);

  slist1 = NULL;
  slist1 = curl_slist_append(slist1, "Content-Type: application/json");

  hnd = curl_easy_init();
  curl_easy_setopt(hnd, CURLOPT_URL, "http://localhost/v4.0.0/libpod/containers/create");
  curl_easy_setopt(hnd, CURLOPT_UNIX_SOCKET_PATH, "/run/layercake/podman.sock");
  curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, createjson);
  curl_easy_setopt(hnd, CURLOPT_POSTFIELDSIZE_LARGE, (curl_off_t)strlen(createjson));
  curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, slist1);
  curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "POST");
  curl_easy_setopt(hnd, CURLOPT_BUFFERSIZE, 102400L);
  curl_easy_setopt(hnd, CURLOPT_NOPROGRESS, 1L);
  curl_easy_setopt(hnd, CURLOPT_USERAGENT, "curl/7.82.0");
  curl_easy_setopt(hnd, CURLOPT_MAXREDIRS, 50L);
  curl_easy_setopt(hnd, CURLOPT_HTTP_VERSION, (long)CURL_HTTP_VERSION_2TLS);
  curl_easy_setopt(hnd, CURLOPT_FTP_SKIP_PASV_IP, 1L);
  curl_easy_setopt(hnd, CURLOPT_TCP_KEEPALIVE, 1L);

  ret = curl_easy_perform(hnd);

  curl_easy_getinfo(hnd, CURLINFO_RESPONSE_CODE, &responsecode);
  if(responsecode == 201){
    printf("Created successfully\n");
  } else {
    printf("Failed to create.  Response code: %d\n", responsecode);
  }

  curl_easy_cleanup(hnd);
  hnd = NULL;
  curl_slist_free_all(slist1);
  slist1 = NULL;

  free(createjson);

  return (int)ret;
}


int start_job_container(char *containername) {
  CURLcode ret;
  CURL *hnd;
  long responsecode;
  char *url;
  size_t urllength;

  char urltemplate[] = "http://localhost/v4.0.0/libpod/containers/%s/start";

  urllength = snprintf(NULL, 0, urltemplate, containername);
  url = malloc((urllength + 1) * sizeof(char));
  sprintf(url, urltemplate, containername);

  hnd = curl_easy_init();
  curl_easy_setopt(hnd, CURLOPT_BUFFERSIZE, 102400L);
  curl_easy_setopt(hnd, CURLOPT_URL, url);
  curl_easy_setopt(hnd, CURLOPT_NOPROGRESS, 1L);
  curl_easy_setopt(hnd, CURLOPT_USERAGENT, "curl/7.82.0");
  curl_easy_setopt(hnd, CURLOPT_MAXREDIRS, 50L);
  curl_easy_setopt(hnd, CURLOPT_HTTP_VERSION, (long)CURL_HTTP_VERSION_2TLS);
  curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "POST");
  curl_easy_setopt(hnd, CURLOPT_FTP_SKIP_PASV_IP, 1L);
  curl_easy_setopt(hnd, CURLOPT_TCP_KEEPALIVE, 1L);
  curl_easy_setopt(hnd, CURLOPT_UNIX_SOCKET_PATH, "/run/layercake/podman.sock");

  ret = curl_easy_perform(hnd);

  curl_easy_getinfo(hnd, CURLINFO_RESPONSE_CODE, &responsecode);
  if(responsecode == 204){
    printf("Started successfully\n");
  } else {
    printf("Failed to start.  Response code: %d\n", responsecode);
  }

  curl_easy_cleanup(hnd);
  hnd = NULL;

  free(url);

  return (int)ret;
}


int wait_job_container(char *containername, char *containerstate){
  CURLcode ret;
  CURL *hnd;
  long responsecode;
  char *url;
  size_t urllength;

  char urltemplate[] = "http://localhost/v4.0.0/libpod/containers/%s/wait?condition=%s";

  urllength = snprintf(NULL, 0, urltemplate, containername, containerstate);
  url = malloc((urllength + 1) * sizeof(char));
  sprintf(url, urltemplate, containername, containerstate);

  hnd = curl_easy_init();
  curl_easy_setopt(hnd, CURLOPT_BUFFERSIZE, 102400L);
  curl_easy_setopt(hnd, CURLOPT_URL, url);
  curl_easy_setopt(hnd, CURLOPT_NOPROGRESS, 1L);
  curl_easy_setopt(hnd, CURLOPT_USERAGENT, "curl/7.82.0");
  curl_easy_setopt(hnd, CURLOPT_MAXREDIRS, 50L);
  curl_easy_setopt(hnd, CURLOPT_HTTP_VERSION, (long)CURL_HTTP_VERSION_2TLS);
  curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "POST");
  curl_easy_setopt(hnd, CURLOPT_FTP_SKIP_PASV_IP, 1L);
  curl_easy_setopt(hnd, CURLOPT_TCP_KEEPALIVE, 1L);
  curl_easy_setopt(hnd, CURLOPT_UNIX_SOCKET_PATH, "/run/layercake/podman.sock");

  ret = curl_easy_perform(hnd);

  curl_easy_getinfo(hnd, CURLINFO_RESPONSE_CODE, &responsecode);
  if(responsecode == 200){
    printf("Waited successfully\n");
  } else {
    printf("Failed to wait.  Response code: %d\n", responsecode);
  }

  curl_easy_cleanup(hnd);
  hnd = NULL;

  free(url);

  return (int)ret;
}


int kill_job_container(char *containername){
  CURLcode ret;
  CURL *hnd;
  long responsecode;
  char *url;
  size_t urllength;

  char urltemplate[] = "http://localhost/v4.0.0/libpod/containers/%s/kill";

  urllength = snprintf(NULL, 0, urltemplate, containername);
  url = malloc((urllength + 1) * sizeof(char));
  sprintf(url, urltemplate, containername);

  hnd = curl_easy_init();
  curl_easy_setopt(hnd, CURLOPT_BUFFERSIZE, 102400L);
  curl_easy_setopt(hnd, CURLOPT_URL, url);
  curl_easy_setopt(hnd, CURLOPT_NOPROGRESS, 1L);
  curl_easy_setopt(hnd, CURLOPT_USERAGENT, "curl/7.82.0");
  curl_easy_setopt(hnd, CURLOPT_MAXREDIRS, 50L);
  curl_easy_setopt(hnd, CURLOPT_HTTP_VERSION, (long)CURL_HTTP_VERSION_2TLS);
  curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "POST");
  curl_easy_setopt(hnd, CURLOPT_FTP_SKIP_PASV_IP, 1L);
  curl_easy_setopt(hnd, CURLOPT_TCP_KEEPALIVE, 1L);
  curl_easy_setopt(hnd, CURLOPT_UNIX_SOCKET_PATH, "/run/layercake/podman.sock");

  ret = curl_easy_perform(hnd);

  curl_easy_getinfo(hnd, CURLINFO_RESPONSE_CODE, &responsecode);
  if(responsecode == 204){
    printf("Killed successfully\n");
  } else {
    printf("Failed to kill.  Response code: %d\n", responsecode);
  }

  curl_easy_cleanup(hnd);
  hnd = NULL;

  free(url);

  return (int)ret;
}


int delete_job_container(char *containername){
  CURLcode ret;
  CURL *hnd;
  long responsecode;
  char *url;
  size_t urllength;

  char urltemplate[] = "http://localhost/v4.0.0/libpod/containers/%s";

  urllength = snprintf(NULL, 0, urltemplate, containername);
  url = malloc((urllength + 1) * sizeof(char));
  sprintf(url, urltemplate, containername);

  hnd = curl_easy_init();
  curl_easy_setopt(hnd, CURLOPT_BUFFERSIZE, 102400L);
  curl_easy_setopt(hnd, CURLOPT_URL, url);
  curl_easy_setopt(hnd, CURLOPT_NOPROGRESS, 1L);
  curl_easy_setopt(hnd, CURLOPT_USERAGENT, "curl/7.82.0");
  curl_easy_setopt(hnd, CURLOPT_MAXREDIRS, 50L);
  curl_easy_setopt(hnd, CURLOPT_HTTP_VERSION, (long)CURL_HTTP_VERSION_2TLS);
  curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "DELETE");
  curl_easy_setopt(hnd, CURLOPT_FTP_SKIP_PASV_IP, 1L);
  curl_easy_setopt(hnd, CURLOPT_TCP_KEEPALIVE, 1L);
  curl_easy_setopt(hnd, CURLOPT_UNIX_SOCKET_PATH, "/run/layercake/podman.sock");

  ret = curl_easy_perform(hnd);

  curl_easy_getinfo(hnd, CURLINFO_RESPONSE_CODE, &responsecode);
  if(responsecode == 200){
    printf("Deleted successfully\n");
  } else {
    printf("Failed to delete.  Response code: %d\n", responsecode);
  }

  curl_easy_cleanup(hnd);
  hnd = NULL;

  free(url);

  return (int)ret;
}


int get_container_list()
{
  CURLcode ret;
  CURL *hnd;

  hnd = curl_easy_init();
  curl_easy_setopt(hnd, CURLOPT_URL, "http://localhost/v4.0.0/libpod/containers/json");
  curl_easy_setopt(hnd, CURLOPT_UNIX_SOCKET_PATH, "/run/layercake/podman.sock");
  curl_easy_setopt(hnd, CURLOPT_BUFFERSIZE, 102400L);
  curl_easy_setopt(hnd, CURLOPT_NOPROGRESS, 1L);
  curl_easy_setopt(hnd, CURLOPT_USERAGENT, "curl/7.81.0");
  curl_easy_setopt(hnd, CURLOPT_MAXREDIRS, 50L);
  curl_easy_setopt(hnd, CURLOPT_HTTP_VERSION, (long)CURL_HTTP_VERSION_2TLS);
  curl_easy_setopt(hnd, CURLOPT_FTP_SKIP_PASV_IP, 1L);
  curl_easy_setopt(hnd, CURLOPT_TCP_KEEPALIVE, 1L);


  /* Here is a list of options the curl code used that cannot get generated
     as source easily. You may choose to either not use them or implement
     them yourself.

  CURLOPT_WRITEDATA set to a objectpointer
  CURLOPT_INTERLEAVEDATA set to a objectpointer
  CURLOPT_WRITEFUNCTION set to a functionpointer
  CURLOPT_READDATA set to a objectpointer
  CURLOPT_READFUNCTION set to a functionpointer
  CURLOPT_SEEKDATA set to a objectpointer
  CURLOPT_SEEKFUNCTION set to a functionpointer
  CURLOPT_ERRORBUFFER set to a objectpointer
  CURLOPT_STDERR set to a objectpointer
  CURLOPT_HEADERFUNCTION set to a functionpointer
  CURLOPT_HEADERDATA set to a objectpointer

  */

  ret = curl_easy_perform(hnd);

  long httpcode;
  curl_easy_getinfo(hnd, CURLINFO_RESPONSE_CODE, &httpcode);

  printf("Response: %d\n", httpcode);

  curl_easy_cleanup(hnd);
  hnd = NULL;

  return (int)ret;
}
/**** End of sample code ****/


int main(int argc, char *argv[]){
  char imagename[] = "jobcontainer";
  char podname[] = "lc01";
  char containername[] = "libcurljobcontainer"; 

  get_container_list();
  create_job_container(imagename, podname, containername);
  start_job_container(containername);
  wait_job_container(containername, "running");
  kill_job_container(containername);
  wait_job_container(containername, "exited");
  delete_job_container(containername);
}
