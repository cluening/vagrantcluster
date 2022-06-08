#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <slurm/spank.h>


void _prepare_curl_handle(CURL *hnd){
  curl_easy_setopt(hnd, CURLOPT_BUFFERSIZE, 102400L);
  curl_easy_setopt(hnd, CURLOPT_NOPROGRESS, 1L);
  curl_easy_setopt(hnd, CURLOPT_USERAGENT, "curl/7.82.0");
  curl_easy_setopt(hnd, CURLOPT_MAXREDIRS, 50L);
  curl_easy_setopt(hnd, CURLOPT_HTTP_VERSION, (long)CURL_HTTP_VERSION_2TLS);
  curl_easy_setopt(hnd, CURLOPT_FTP_SKIP_PASV_IP, 1L);
  curl_easy_setopt(hnd, CURLOPT_TCP_KEEPALIVE, 1L);
  curl_easy_setopt(hnd, CURLOPT_UNIX_SOCKET_PATH, "/run/layercake/podman.sock");
}


int _pull_job_container(char *imagename) {
  CURLcode ret;
  CURL *hnd;
  long responsecode;
  char *url;
  size_t urllength;

  // FIXME: clean up the `tlsVerify` parameter once a real registry is working
  char urltemplate[] = "http://localhost/v4.0.0/libpod/images/pull?tlsVerify=false&reference=%s";

  urllength = snprintf(NULL, 0, urltemplate, imagename);
  url = malloc((urllength + 1) * sizeof(char));
  sprintf(url, urltemplate, imagename);

  hnd = curl_easy_init();
  _prepare_curl_handle(hnd);
  curl_easy_setopt(hnd, CURLOPT_URL, url);
  curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "POST");

  slurm_info("Pulling image");

  ret = curl_easy_perform(hnd);

  if(ret != CURLE_OK){
    slurm_info("curl error %d: %s", ret, curl_easy_strerror(ret));
    return -1;
  }

  curl_easy_getinfo(hnd, CURLINFO_RESPONSE_CODE, &responsecode);
  if(responsecode == 200){
    slurm_info("Pulled successfully");
  } else {
    slurm_info("Failed to pull.  Response code: %d", responsecode);
  }

  curl_easy_cleanup(hnd);
  hnd = NULL;

  free(url);

  return (int)ret;
}


int _create_job_container(char *imagename, char *podname, char *containername){
  CURLcode ret;
  CURL *hnd;
  struct curl_slist *slist1;
  long responsecode;
  char *createjson;
  size_t createjsonlength;

  char createjsontemplate[] = 
    "{"
      "\"image\": \"%s\","
      "\"pod\": \"%s\","
      "\"name\": \"%s\","
      "\"mounts\": ["
      "{ \"Source\": \"/run/layercake\","
        "\"Destination\": \"/run/layercake\","
        "\"Type\": \"bind\" "
      "},"
      "{ \"Source\": \"/etc/layercake\","
        "\"Destination\": \"/etc/layercake\","
        "\"Type\": \"bind\" "
      "},"
      "{ \"Source\": \"/home\","
        "\"Destination\": \"/home\","
        "\"Type\": \"bind\" "
      "}"
      "]"
    "}";

  createjsonlength = snprintf(NULL, 0, createjsontemplate, imagename, podname, containername);
  createjson = malloc((createjsonlength + 1) * sizeof(char));
  sprintf(createjson, createjsontemplate, imagename, podname, containername);

  slist1 = NULL;
  slist1 = curl_slist_append(slist1, "Content-Type: application/json");

  hnd = curl_easy_init();
  _prepare_curl_handle(hnd);
  curl_easy_setopt(hnd, CURLOPT_URL, "http://localhost/v4.0.0/libpod/containers/create");
  curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, createjson);
  curl_easy_setopt(hnd, CURLOPT_POSTFIELDSIZE_LARGE, (curl_off_t)strlen(createjson));
  curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, slist1);
  curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "POST");

  ret = curl_easy_perform(hnd);

  if(ret != CURLE_OK){
    slurm_info("curl error %d: %s", ret, curl_easy_strerror(ret));
    return -1;
  }

  curl_easy_getinfo(hnd, CURLINFO_RESPONSE_CODE, &responsecode);
  if(responsecode == 201){
    slurm_info("Created successfully");
  } else {
    slurm_info("Failed to create.  Response code: %d", responsecode);
  }

  curl_easy_cleanup(hnd);
  hnd = NULL;
  curl_slist_free_all(slist1);
  slist1 = NULL;

  free(createjson);

  return (int)ret;
}


int _start_job_container(char *containername) {
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
  _prepare_curl_handle(hnd);
  curl_easy_setopt(hnd, CURLOPT_URL, url);
  curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "POST");

  ret = curl_easy_perform(hnd);

  if(ret != CURLE_OK){
    slurm_info("curl error %d: %s", ret, curl_easy_strerror(ret));
    return -1;
  }

  curl_easy_getinfo(hnd, CURLINFO_RESPONSE_CODE, &responsecode);
  if(responsecode == 204){
    slurm_info("Started successfully");
  } else {
    slurm_info("Failed to start.  Response code: %d", responsecode);
  }

  curl_easy_cleanup(hnd);
  hnd = NULL;

  free(url);

  return (int)ret;
}


int _wait_job_container(char *containername, char *containerstate){
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
  _prepare_curl_handle(hnd);
  curl_easy_setopt(hnd, CURLOPT_URL, url);
  curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "POST");

  ret = curl_easy_perform(hnd);

  if(ret != CURLE_OK){
    slurm_info("curl error %d: %s", ret, curl_easy_strerror(ret));
    return -1;
  }

  curl_easy_getinfo(hnd, CURLINFO_RESPONSE_CODE, &responsecode);
  if(responsecode == 200){
    slurm_info("Waited successfully");
  } else {
    slurm_info("Failed to wait.  Response code: %d", responsecode);
  }

  curl_easy_cleanup(hnd);
  hnd = NULL;

  free(url);

  return (int)ret;
}


int _kill_job_container(char *containername){
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
  _prepare_curl_handle(hnd);
  curl_easy_setopt(hnd, CURLOPT_URL, url);
  curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "POST");

  ret = curl_easy_perform(hnd);

  if(ret != CURLE_OK){
    slurm_info("curl error %d: %s", ret, curl_easy_strerror(ret));
    return -1;
  }

  curl_easy_getinfo(hnd, CURLINFO_RESPONSE_CODE, &responsecode);
  if(responsecode == 204){
    slurm_info("Killed successfully");
  } else {
    slurm_info("Failed to kill.  Response code: %d", responsecode);
  }

  curl_easy_cleanup(hnd);
  hnd = NULL;

  free(url);

  return (int)ret;
}


int _delete_job_container(char *containername){
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
  _prepare_curl_handle(hnd);
  curl_easy_setopt(hnd, CURLOPT_URL, url);
  curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "DELETE");

  ret = curl_easy_perform(hnd);

  if(ret != CURLE_OK){
    slurm_info("curl error %d: %s", ret, curl_easy_strerror(ret));
    return -1;
  }

  curl_easy_getinfo(hnd, CURLINFO_RESPONSE_CODE, &responsecode);
  if(responsecode == 200){
    slurm_info("Deleted successfully");
  } else {
    slurm_info("Failed to delete.  Response code: %d", responsecode);
  }

  curl_easy_cleanup(hnd);
  hnd = NULL;

  free(url);

  return (int)ret;
}
