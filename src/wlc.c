#include <curl/curl.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "../include/config.h"

// number of x - 1: used for loop exit conditions
#define EFFECT_NUM 117
#define PALETTE_NUM 55
#define QUERY_NUM 5

// usage and help messages
const char *USAGE = "\
usage: wlc [OPTION] [VALUE] ...\n\
try 'wlc -h' for more information.\n\
";
const char *HELP = "\
usage: wlc [OPTION] [VALUE] ...\n\
Control a WLED host through the HTTP API.\n\n\
  -h            display this help\n\
  -b NUM        set master brightness.\n\
  -e NAME       set master effect.\n\
  -p NAME       set color palette.\n\
  -f NUM        set effect speed.\n\
  -i NUM        set effect intensity.\n\
  -m HUE:SAT    set master hue and saturation.\n\
  -s SEG        choose segment to aplly the changes.\n\
  -l OPTION     list all 'effects' or 'palettes'.\n\
  -t            set master color by current time.\n\
  -q QUERY      query the WLED host\n\n\
  NUM, SAT:   integer value between 0 and 255.\n\
  SEG:        integer value between 0 and 9.\n\
  HUE:        integer value between 0 and 65535.\n\
  NAME:       string. Space-delimited strings need to enclosed in 'quotes'.\n\n\
  QUERY:      string. One of these available options:\n\
              'brightness', 'speed', 'intensity', palette', 'effect'\n\n\
For more information on how to configure the cli,\n\
at <https://github.com/channel-42/wlc/blob/master/README.md>\n\
";

// print help message
void printHelp(void) { fprintf(stdout, "%s", HELP); }

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

// init string struct
void init_string(string_t *s) {
  s->len = 0;
  s->ptr = malloc(s->len + 1);
  if (s->ptr == NULL) {
    fprintf(stderr, "string malloc() failed\n");
    exit(EXIT_FAILURE);
  }
  s->ptr[0] = '\0';
}
// write get response to passed string
size_t write_data(void *ptr, size_t size, size_t nmemb, string_t *s) {
  size_t new_len = s->len + size * nmemb;
  s->ptr = realloc(s->ptr, new_len + 1);
  if (s->ptr == NULL) {
    fprintf(stderr, "realloc() failed\n");
    exit(EXIT_FAILURE);
  }
  memcpy(s->ptr + s->len, ptr, size * nmemb);
  s->ptr[new_len] = '\0';
  s->len = new_len;

  return size * nmemb;
}

// find and return array id by passed name
int findFromName(const effect_t array[], char *query, int limit) {
  uint8_t i = 0;
  while (strcmp(array[i].name, query)) {
    if (i > limit) {
      return -1;
    }
    i++;
  }
  return i;
}

// list available effects and palettes
int listOptions(char *opt) {
  if (!strcmp(opt, "effects")) {
    for (uint8_t i = 0; i <= EFFECT_NUM; i++) {
      fprintf(stdout, "%s\n", effect_arr[i].name);
    }
    return 0;
  } else if (!strcmp(opt, "palettes")) {
    for (uint8_t i = 0; i <= PALETTE_NUM; i++) {
      fprintf(stdout, "%s\n", palette_arr[i].name);
    }
    return 0;
  } else {
    return 1;
  }
}

// change brightness to specified value
int chngBri(CURL *curl, uint8_t val) {
  // request http url
  char request[255];
  sprintf(request, "%sA=%d", IP, val);
  // make request
  return req(curl, request);
}

// set segment to change
int setSegm(CURL *curl, uint8_t id) {
  // request http url
  char request[255];
  sprintf(request, "%sSS=%d", IP, id);
  // make request
  return req(curl, request);
}

// change effect preset
int chngEff(CURL *curl, char *eff_name) {
  // request http url
  char request[255];
  // request http url
  sprintf(request, "%sFX=%d", IP,
          findFromName(effect_arr, eff_name, EFFECT_NUM));
  // make request
  return req(curl, request);
}

// change palette
int chngPal(CURL *curl, char *pal_name) {
  // request http url
  char request[255];
  // request http url
  sprintf(request, "%sFP=%d", IP,
          findFromName(palette_arr, pal_name, PALETTE_NUM));
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
// !used on startup
int setUserColors(CURL *curl) {
  // request http url
  char request[255];
  sprintf(request, "%sCL=%s&C2=%s&C3=%s", IP, COLOR1, COLOR2, COLOR3);
  // make request
  return req(curl, request);
}

// change hue and saturation
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
    // save value to hue-sat array
    hs[i] = atoi(seg);
    // get next token
    seg = strtok(NULL, tok);
  }
  // request http url
  sprintf(request, "%sHU=%d&SA=%d", IP, hs[0], hs[1]);
  // make request
  return req(curl, request);
}

int followDaylight(CURL *curl) {
  // get current hour
  time_t rawtime;
  struct tm *tm_struct;
  rawtime = time(NULL);
  tm_struct = localtime(&rawtime);
  uint8_t hour = tm_struct->tm_hour;
  // request string
  char request[255];
  // mode has to be 0/Solid!
  sprintf(request, "%sFX=%d&R=%d&G=%d&B=%d", IP, 0,
          daylight_gradient[hour].rgb.r, daylight_gradient[hour].rgb.g,
          daylight_gradient[hour].rgb.b);
  return req(curl, request);
}

int queryHost(CURL *curl, char *opt, string_t str) {
  // check for legal query
  uint8_t i = 0;
  while (strcmp(xml_arr[i].name, opt)) {
    if (i > QUERY_NUM) {
      return 1;
    }
    i++;
  }
  // parse string for requested info
  // get pointer to requested xml-tag (opening tag)
  char *temp_str = strstr(str.ptr, xml_arr[i].tag);
  uint8_t value = 0;
  // find first number after xml-tag
  sscanf(temp_str, "%*[^>]>%" SCNu8 " ", &value);
  // print out name for effect/palette or just the value
  if (!strcmp(xml_arr[i].name, "effect")) {
    fprintf(stdout, "%s\n", effect_arr[value].name);
  } else if (!strcmp(xml_arr[i].name, "palette")) {
    fprintf(stdout, "%s\n", palette_arr[value].name);
  } else {
    fprintf(stdout, "%u\n", value);
  }
  return 0;
}

// main program
int main(int argc, char *argv[]) {
  // speed up help print in case the host doesn't respond
  if (argc == 2 && !strcmp("-h", argv[1])) {
    // only if only "-h" is provided!
    printHelp();
    exit(EXIT_SUCCESS);
  } else {
    // setup curl
    CURL *curl = curl_easy_init();
    string_t str;
    init_string(&str);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &str);
    // set user colors
    if (setUserColors(curl)) {
      fprintf(stderr, "Couldn't set user colors\n");
    }
    // check if curl is OK
    if (curl) {
      extern char *optarg;
      int opt;
      // parse through options
      while ((opt = getopt(argc, argv, "b:e:p:f:i:m:s:l:q:th")) != -1) {
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
            fprintf(stderr,
                    "An error occurred while making the http request\n");
            break;
          }
          break;
        case 'i':
          // change master effect intensity
          if (chngStrg(curl, atoi(optarg))) {
            fprintf(stderr,
                    "An error occurred while making the http request\n");
            break;
          }
          break;
        case 'm':
          // change master color hue and saturation
          if (chngHueSat(curl, optarg) == 1) {
            fprintf(stderr,
                    "An error occurred while making the http request\n");
            break;
          }
          break;
        case 's':
          // set segment to apply api call
          if (setSegm(curl, atoi(optarg))) {
            fprintf(stderr,
                    "An error occurred while making the http request\n");
            break;
          }
          break;
        case 'l':
          // list available palettes and effects
          if (listOptions(optarg)) {
            fprintf(stderr,
                    "Unknown option. Choose either 'effects' or 'palettes'\n");
            break;
          }
          break;
        case 'q':
          // query the wled host
          if (queryHost(curl, optarg, str)) {
            fprintf(stderr, "Unknown query term: %s\n", optarg);
            break;
          }
          break;
        case 't':
          if (followDaylight(curl)) {
            fprintf(stderr,
                    "An error occurred while making the http request\n");
            break;
          }
          break;
        case 'h':
          printHelp();
          break;
        default:
          // default, when an option is unknown
          // print usage and exit
          fprintf(stderr, "%s", USAGE);
          exit(EXIT_FAILURE);
          break;
        }
      }
      // cleanup curl
      free(str.ptr);
      curl_global_cleanup();
      curl_easy_cleanup(curl);
    }
  }
  return 0;
  exit(EXIT_SUCCESS);
}
