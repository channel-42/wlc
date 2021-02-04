#include <curl/curl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../include/config.h"

// number of x - 1: used for loop exit conditions
#define EFFECT_NUM 177
#define PALETTE_NUM 55

// make a curl http get request
int req(CURL *curl, char *url) {
  CURLcode resp;
  curl_easy_setopt(curl, CURLOPT_URL, url);
  resp = curl_easy_perform(curl);
  // check request
  if (resp != CURLE_OK) {
    return 1;
  } else {
    return 0;
  }
}
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
  return req(curl, request);
}

int chngEff(CURL *curl, char *eff_name) {
  CURLcode resp;
  // request http url
  char request[255];

  // look if effect exists
  uint8_t i = 0;
  while (strcmp(effect_arr[i].name, eff_name)) {
    if (i > EFFECT_NUM) {
      return 1;
    }
    i++;
  }
  // request http url
  sprintf(request, "%sFX=%d", IP, i);
  // make request
  return req(curl, request);
}

int chngPal(CURL *curl, char *pal_name) {
  CURLcode resp;
  // request http url
  char request[255];

  // look if palette exists
  uint8_t i = 0;
  while (strcmp(palette_arr[i].name, pal_name)) {
    if (i > PALETTE_NUM) {
      return 1;
    }
    i++;
  }
  // request http url
  sprintf(request, "%sFP=%d", IP, i);
  // make request
  return req(curl, request);
}
int main(int argc, char *argv[]) {

  CURL *curl = curl_easy_init();
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);

  // check for if curl is OK
  if (curl) {
    extern char *optarg;
    int opt;
    // parse through options
    while ((opt = getopt(argc, argv, "b:e:p:")) != -1) {
      switch (opt) {
      case 'b':
        // change master brightness
        chngBri(curl, atoi(optarg));
        break;
      case 'e':
        // change master effect
        if (chngEff(curl, optarg)) {
          fprintf(stderr, "Unknown effect: %s\n", optarg);
          break;
        }
        break;
      case 'p':
        // change master color palette
        if (chngPal(curl, optarg)) {
          fprintf(stderr, "Unknown color palette: %s\n", optarg);
          break;
        }
        break;
      default:
        // default: print error
        curl_easy_cleanup(curl);
        fprintf(stderr, "Usage: %s [-obhs] [value]\n", argv[0]);
        exit(EXIT_FAILURE);
        break;
      }
    }
  }
  curl_global_cleanup();
  curl_easy_cleanup(curl);
  return 0;
}
