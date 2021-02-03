#include <curl/curl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../include/config.h"

// make curl shut up
size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp) {
  return size * nmemb;
}

// change brightness to specified value
int chngBri(CURL *curl, uint8_t val) {
  CURLcode resp;
  // request http url
  char request[255];
  sprintf(request, "%sA=%d", IP, val);

  // make request
  curl_easy_setopt(curl, CURLOPT_URL, request);
  resp = curl_easy_perform(curl);
  curl_easy_cleanup(curl);

  // check request
  if (resp != CURLE_OK) {
    exit(EXIT_FAILURE);
    return 1;
  } else {
    return 0;
  }
}

int main(int argc, char *argv[]) {

  CURL *curl;
  curl = curl_easy_init();
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);

  // check for if curl is OK
  if (curl) {
    int opt;
    // parse through options
    while ((opt = getopt(argc, argv, "o:bhsv")) != -1) {
      switch (opt) {
      case 'o':
        // change master brightness
        chngBri(curl, atoi(optarg));
        break;
      default:
        // default: print error
        fprintf(stderr, "Usage: %s [-obhs] [value]\n", argv[0]);
        exit(EXIT_FAILURE);
        break;
      }
    }
  }
  return 0;
}
