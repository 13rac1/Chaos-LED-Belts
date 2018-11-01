enum beltColor { RED = 0, ORANGE, YELLOW, GREEN, BLUE, PURPLE, PINK, WHITE };

struct belt {
  uint8_t hue;
  uint8_t num;
  const char *name;
};

#define NUM_BELTS 8
belt belts[NUM_BELTS] = {
    {0, 28, "AT+GAPDEVNAME=Chaos Red"},
    {16, 28, "AT+GAPDEVNAME=Chaos Orange"},
    {64, 28, "AT+GAPDEVNAME=Chaos Yellow"},
    {96, 26, "AT+GAPDEVNAME=Chaos Green"},
    {160, 26, "AT+GAPDEVNAME=Chaos Blue"},
    {192, 28, "AT+GAPDEVNAME=Chaos Purple"},
    {224, 24, "AT+GAPDEVNAME=Chaos Pink"},
    {192, 28, "AT+GAPDEVNAME=Chaos White"},  // No hue, special case
};

#define BUCKLE_DATA_PIN 6
#define BUCKLE_LED_TYPE WS2812
#define BUCKLE_COLOR_ORDER GRB
#define BUCKLE_NUM_LEDS 8
CRGB buckle_leds[BUCKLE_NUM_LEDS];

#define BELT_DATA_PIN 11
// Check board type.
// Reference for compile-type variables:
// https://github.com/arduino/Arduino/wiki/Arduino-IDE-1.5-3rd-party-Hardware-specification#boardstxt

// Adafruit Feather 32u4
#ifdef ARDUINO_AVR_FEATHER32U4
#pragma message "Found ARDUINO_AVR_FEATHER32U4"
#define BELT_CLK_PIN 3
#endif
// Adafruit Feather M0
#ifdef ARDUINO_SAMD_FEATHER_M0
#pragma message "Found ARDUINO_SAMD_FEATHER_M0"
#define BELT_CLK_PIN 21
#endif

#define BELT_LED_TYPE LPD8806
#define BELT_COLOR_ORDER GRB
#define BELT_NUM_LEDS_MAX 40
CRGB belt_leds[BELT_NUM_LEDS_MAX];
