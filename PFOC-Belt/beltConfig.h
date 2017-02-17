enum beltColor {
  RED = 0,
  ORANGE,
  YELLOW,
  GREEN,
  BLUE,
  PURPLE,
  PINK,
  WHITE
};

struct belt {
  uint8_t hue;
  uint8_t num;
  const char *name;
};

#define NUM_BELTS               8
belt belts[NUM_BELTS] = {
  {0,   28, "AT+GAPDEVNAME=PFOC Red"},    // Confirmed length.
  {32,  28, "AT+GAPDEVNAME=PFOC Orange"}, // Confirmed length.
  {64,  28, "AT+GAPDEVNAME=PFOC Yellow"}, // Confirmed length.
  {96,  26, "AT+GAPDEVNAME=PFOC Green"},  // Confirmed length.
  {160, 26, "AT+GAPDEVNAME=PFOC Blue"},   // Confirmed length.
  {192, 32, "AT+GAPDEVNAME=PFOC Purple"}, // Confirmed length.
  {224, 24, "AT+GAPDEVNAME=PFOC Pink"},   // Confirmed length.
  {192, 34, "AT+GAPDEVNAME=PFOC White"},  // Confirmed length, no hue, special case
};

#define BUCKLE_DATA_PIN         6
#define BUCKLE_LED_TYPE    WS2812
#define BUCKLE_COLOR_ORDER    GRB
#define BUCKLE_NUM_LEDS         8
CRGB buckle_leds[BUCKLE_NUM_LEDS];

#define BELT_DATA_PIN          11
//Adafruit Feather 32u4
#define BELT_CLK_PIN            3
//Adafruit Feather M0
//#define BELT_CLK_PIN           21
#define BELT_LED_TYPE     LPD8806
#define BELT_COLOR_ORDER      GRB
#define BELT_NUM_LEDS_MAX      40
CRGB belt_leds[BELT_NUM_LEDS_MAX];
