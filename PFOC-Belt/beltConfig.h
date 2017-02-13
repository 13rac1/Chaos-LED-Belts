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
};

#define NUM_BELTS          8
belt belts[NUM_BELTS] = {
  {0,   32}, // Red
  {32,  28}, // Orange, confirmed.
  {64,  32}, // Yellow
  {96,  32}, // Green
  {160, 32}, // Blue
  {192, 36}, // Purple
  {224, 30}, // Pink
  {192, 34},  // White, no hue, special case
};

#define BUCKLE_DATA_PIN    6
#define BUCKLE_LED_TYPE    WS2812
#define BUCKLE_COLOR_ORDER GRB
#define BUCKLE_NUM_LEDS    8
CRGB buckle_leds[BUCKLE_NUM_LEDS];

#define BELT_DATA_PIN      11
#define BELT_CLK_PIN       3
#define BELT_LED_TYPE      LPD8806
#define BELT_COLOR_ORDER   GRB
#define BELT_NUM_LEDS_MAX  40
CRGB belt_leds[BELT_NUM_LEDS_MAX];
