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
  // request http url
  char request[255];
  sprintf(request, "%sA=%d", IP, val);
  // make request
  return req(curl, request);
}

// change effect preset
int chngEff(CURL *curl, char *eff_name) {
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

// change palette
int chngPal(CURL *curl, char *pal_name) {
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

// change effect speed
int chngSpeed(CURL *curl, uint8_t val) {
  // request http url
  char request[255];
  sprintf(request, "%sSX=%d", IP, val);
  // make request
  return req(curl, request);
}

// change effect intensity
int chngStrg(CURL *curl, uint8_t val) {
  // request http url
  char request[255];
  sprintf(request, "%sIX=%d", IP, val);
  // make request
  return req(curl, request);
}

// set default color palette to user defined colors
// used on startup
int setUserColors(CURL *curl) {
  // request http url
  char request[255];
  sprintf(request, "%sCL=%s&C2=%s&C3=%s", IP, COLOR1, COLOR2, COLOR3);
  // make request
  return req(curl, request);
}

int chngHueSat(CURL *curl, char *hue_sat) {
  // request http url
  char request[255];
  // tokenize input string
  const char *tok = ":";
  char *input = hue_sat;
  uint hs[2];
  char *seg = strtok(hue_sat, tok);
  // get segments from string
  for (uint8_t i = 0; i < 2; i++) {
    // check if input is correct
    if (seg == NULL) {
      fprintf(stderr, "Invalid input: %s\nFormat is: 'HUE:SAT'\n", input);
      return 2;
    }
    hs[i] = atoi(seg);
    seg = strtok(NULL, tok);
  }
  sprintf(request, "%sHU=%d&SA=%d", IP, hs[0], hs[1]);
  // make request
  return req(curl, request);
}

int main(int argc, char *argv[]) {
  // setup curl
  CURL *curl = curl_easy_init();
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
  // set user colors
  if (setUserColors(curl)) {
    fprintf(stderr, "Couldn't set user colors\n");
  }
  // check if curl is OK
  if (curl) {
    extern char *optarg;
    int opt;
    // parse through options
    while ((opt = getopt(argc, argv, "b:e:p:f:i:h:")) != -1) {
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
      case 'f':
        // change master effect speed
        if (chngSpeed(curl, atoi(optarg))) {
          fprintf(stderr, "An error occoured while making the http request\n");
          break;
        }
        break;
      case 'i':
        // change master effect intensity
        if (chngStrg(curl, atoi(optarg))) {
          fprintf(stderr, "An error occoured while making the http request\n");
          break;
        }
        break;
      case 'h':
        // change master color hue and saturation
        if (chngHueSat(curl, optarg) == 1) {
          fprintf(stderr, "An error occoured while making the http request\n");
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
