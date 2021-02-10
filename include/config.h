#ifndef CONFIG_H
#define CONFIG_H
#include <stdint.h>
#include <stdlib.h>

/* USER SETTINGS */

// WLED HOST IP, SUFFIX HAS TO BE "/win&"
const static char *IP = "192.168.178.195/win&";
/* WLED COLOR
 * FORMATS:
 * HEX: "HRRGGBB"
 * HEX FOR RGBW: "HWWRRGGBB"
 */
const static char *COLOR1 = "H7957B9";
const static char *COLOR2 = "H9AEDFE";
const static char *COLOR3 = "HFF5555";

/* END OF USER SETTINGS */

// string type used to store xlm response
typedef struct {
  char *ptr;
  size_t len;
} string_t;

// xml option: has name and corresponding tag
typedef struct {
  char name[255];
  char tag[255];
} xml_element_t;

// list of all queriable tags
const static xml_element_t xml_arr[5] = {{"brightness", "<ac>"},
                                         {"speed", "<sx>"},
                                         {"intensity", "<ix>"},
                                         {"palette", "<fp>"},
                                         {"effect", "<fx>"}};
/* single effect: consists of ID and name
 * see https://github.com/Aircoookie/WLED/wiki/List-of-effects-and-palettes
 * for the definition of the effects and IDs
 */
typedef struct {
  uint8_t id;
  char name[255];
} effect_t;

// list of effects: there are 118 defined wled effects
// all defined effects with id and corresponding name
const static effect_t effect_arr[118] = {{0, "Solid"},
                                         {1, "Blink"},
                                         {2, "Breathe"},
                                         {3, "Wipe"},
                                         {4, "Wipe Random"},
                                         {5, "Random Colors"},
                                         {6, "Sweep"},
                                         {7, "Dynamic"},
                                         {8, "Colorloop"},
                                         {9, "Rainbow"},
                                         {10, "Scan"},
                                         {11, "Dual Scan"},
                                         {12, "Fade"},
                                         {13, "Theater"},
                                         {14, "Theater Rainbow"},
                                         {15, "Running"},
                                         {16, "Saw"},
                                         {17, "Twinkle"},
                                         {18, "Dissolve"},
                                         {19, "Dissolve Rnd"},
                                         {20, "Sparkle"},
                                         {21, "Sparkle Dark"},
                                         {22, "Sparkle+"},
                                         {23, "Strobe"},
                                         {24, "Strobe Rainbow"},
                                         {25, "Strobe Mega"},
                                         {26, "Blink Rainbow"},
                                         {27, "Android"},
                                         {28, "Chase"},
                                         {29, "Chase Random"},
                                         {30, "Chase Rainbow"},
                                         {31, "Chase Flash"},
                                         {32, "Chase Flash Rnd"},
                                         {33, "Rainbow Runner"},
                                         {34, "Colorful"},
                                         {35, "Traffic Light"},
                                         {36, "Sweep Random"},
                                         {37, "Running 2"},
                                         {38, "Aurora"},
                                         {39, "Stream"},
                                         {40, "Scanner"},
                                         {41, "Lighthouse"},
                                         {42, "Fireworks"},
                                         {43, "Rain"},
                                         {44, "Merry Christmas"},
                                         {45, "Fire Flicker"},
                                         {46, "Gradient"},
                                         {47, "Loading"},
                                         {48, "Police"},
                                         {49, "Police All"},
                                         {50, "Two Dots"},
                                         {51, "Two Areas"},
                                         {52, "Circus"},
                                         {53, "Halloween"},
                                         {54, "Tri Chase"},
                                         {55, "Tri Wipe"},
                                         {56, "Tri Fade"},
                                         {57, "Lightning"},
                                         {58, "ICU"},
                                         {59, "Multi Comet"},
                                         {60, "Scanner Dual"},
                                         {61, "Stream 2"},
                                         {62, "Oscillate"},
                                         {63, "Pride 2015"},
                                         {64, "Juggle"},
                                         {65, "Palette"},
                                         {66, "Fire 2012"},
                                         {67, "Colorwaves"},
                                         {68, "BPM"},
                                         {69, "Fill Noise"},
                                         {70, "Noise 1"},
                                         {71, "Noise 2"},
                                         {72, "Noise 3"},
                                         {73, "Noise 4"},
                                         {74, "Colortwinkles"},
                                         {75, "Lake"},
                                         {76, "Meteor"},
                                         {77, "Meteor Smooth"},
                                         {78, "Railway"},
                                         {79, "Ripple"},
                                         {80, "Twinklefox"},
                                         {81, "Twinklecat"},
                                         {82, "Halloween Eyes"},
                                         {83, "Solid Pattern"},
                                         {84, "Solid Pattern Tri"},
                                         {85, "Spots"},
                                         {86, "Spots Fade"},
                                         {87, "Glitter"},
                                         {88, "Candle"},
                                         {89, "Fireworks Starburst"},
                                         {90, "Fireworks 1D"},
                                         {91, "Bouncing Balls"},
                                         {92, "Sinelon"},
                                         {93, "Sinelon Dual"},
                                         {94, "Sinelon Rainbow"},
                                         {95, "Popcorn"},
                                         {96, "Drip"},
                                         {97, "Plasma"},
                                         {98, "Percent"},
                                         {99, "Ripple Rainbow"},
                                         {100, "Heartbeat"},
                                         {101, "Pacifica"},
                                         {102, "Candle Multi"},
                                         {103, "Solid Glitter"},
                                         {104, "Sunrise"},
                                         {105, "Phased"},
                                         {106, "TwinkleUp"},
                                         {107, "Noise Pal"},
                                         {108, "Sine"},
                                         {109, "Phased Noise"},
                                         {110, "Flow"},
                                         {111, "Chunchun"},
                                         {112, "Dancing Shadows"},
                                         {113, "Washing machine"},
                                         {114, "Candy Cane"},
                                         {115, "Blends"},
                                         {116, "TV Simulator"},
                                         {117, "Dynamic Smooth"}};
// list of palettes: there are 56 defined wled color palettes
typedef effect_t palette_list_t[56];
// all defined palettes with id and corresponding name
const static palette_list_t palette_arr = {{0, "Default"},
                                           {1, "Random Cycle"},
                                           {2, "Primary color"},
                                           {3, "Based on primary"},
                                           {4, "Set colors"},
                                           {5, "Based on set"},
                                           {6, "Party"},
                                           {7, "Cloud"},
                                           {8, "Lava"},
                                           {9, "Ocean"},
                                           {10, "Forest"},
                                           {11, "Rainbow"},
                                           {12, "Rainbow bands"},
                                           {13, "Sunset"},
                                           {14, "Rivendell"},
                                           {15, "Breeze"},
                                           {16, "Red & Blue"},
                                           {17, "Yellowout"},
                                           {18, "Analoguous"},
                                           {19, "Splash"},
                                           {20, "Pastel"},
                                           {21, "Sunset 2"},
                                           {22, "Beech"},
                                           {23, "Vintage"},
                                           {24, "Departure"},
                                           {25, "Landscape"},
                                           {26, "Beach"},
                                           {27, "Sherbet"},
                                           {28, "Hult"},
                                           {29, "Hult 64"},
                                           {30, "Drywet"},
                                           {31, "Jul"},
                                           {32, "Grintage"},
                                           {33, "Rewhi"},
                                           {34, "Tertiary"},
                                           {35, "Fire"},
                                           {36, "Icefire"},
                                           {37, "Cyane"},
                                           {38, "Light Pink"},
                                           {39, "Autumn"},
                                           {40, "Magenta"},
                                           {41, "Magred"},
                                           {42, "Yelmag"},
                                           {43, "Yelblu"},
                                           {44, "Orange & Teal"},
                                           {45, "Tiamat"},
                                           {46, "April Night"},
                                           {47, "Orangery"},
                                           {48, "C9"},
                                           {49, "Sakura"},
                                           {50, "Aurora"},
                                           {51, "Atlantica"},
                                           {52, "C9 2"},
                                           {53, "C9 New"},
                                           {54, "Temperature"},
                                           {55, "Aurora 2"}};
// rgb type 
typedef struct {
  uint8_t r;
  uint8_t g;
  uint8_t b;
} rgb_t;
// daylight type: rgb color and matching hour
typedef struct {
  rgb_t rgb;
  uint8_t hour;
} daylight_t;
// color gradient over time: list of rgb colors + time in h
const static daylight_t daylight_gradient[24] = {
    {{255, 70, 0}, 0},     {{255, 70, 0}, 1},     {{255, 70, 0}, 2},
    {{255, 70, 0}, 3},     {{255, 80, 0}, 4},     {{255, 90, 0}, 5},
    {{255, 108, 0}, 6},    {{255, 142, 27}, 7},   {{255, 177, 95}, 8},
    {{255, 200, 150}, 9},  {{255, 233, 215}, 10}, {{215, 225, 255}, 11},
    {{190, 215, 255}, 12}, {{190, 200, 255}, 13}, {{230, 245, 255}, 14},
    {{255, 240, 225}, 15}, {{255, 221, 195}, 16}, {{255, 185, 130}, 17},
    {{255, 150, 55}, 18},  {{255, 120, 0}, 19},   {{255, 86, 0}, 20},
    {{255, 70, 0}, 21},    {{255, 70, 0}, 22},    {{255, 70, 0}, 23}};
#endif
