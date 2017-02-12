#include "FastLED.h"

FASTLED_USING_NAMESPACE

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define BUCKLE_DATA_PIN    6
#define BUCKLE_LED_TYPE    WS2812
#define BUCKLE_COLOR_ORDER GRB
#define BUCKLE_NUM_LEDS    8
CRGB buckle_leds[BUCKLE_NUM_LEDS];

#define BELT_DATA_PIN    11
#define BELT_CLK_PIN   3
#define BELT_LED_TYPE    LPD8806
#define BELT_COLOR_ORDER GRB
#define BELT_NUM_LEDS    32
CRGB belt_leds[BELT_NUM_LEDS];

#define BRIGHTNESS          48
#define FRAMES_PER_SECOND  120

void setup() {
  delay(2000);
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<BUCKLE_LED_TYPE,BUCKLE_DATA_PIN,BUCKLE_COLOR_ORDER>(buckle_leds, BUCKLE_NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<BELT_LED_TYPE,BELT_DATA_PIN,BELT_CLK_PIN,BELT_COLOR_ORDER>(belt_leds, BELT_NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
}

uint8_t gHue = 32;
uint8_t range = 32;
uint8_t gHueMin = gHue - range/2;
void loop()
{
  confetti();

  // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND); 

}

void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy(buckle_leds, BUCKLE_NUM_LEDS, 5);
  int pos = random16(BUCKLE_NUM_LEDS);
  buckle_leds[pos] += CHSV( gHueMin + random8(range), 255, 180);

  fadeToBlackBy(belt_leds, BELT_NUM_LEDS, 5);
  int belt_pos = random16(BELT_NUM_LEDS);
  belt_leds[belt_pos] += CHSV( gHueMin + random8(range), 255, 180);
}

