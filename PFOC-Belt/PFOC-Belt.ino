#include "FastLED.h"
FASTLED_USING_NAMESPACE
#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif
#include "beltConfig.h"

// Set which belt this is for, see beltConfig.h
beltColor currentBelt = PURPLE;

#define BRIGHTNESS          48
#define FRAMES_PER_SECOND  120
#define RECOVERY_DELAY    2000

// Color settings
uint8_t gHue = belts[currentBelt].hue;
uint8_t range = 32;
uint8_t gHueMin = gHue - range/2;

// State Management
enum states {
  START,
  RUN,
  CONNECT,
  NODATA,
  DISCONNECT
};
states state = START;

// Store last and current frame times to find frame interval.
unsigned long lastMillis = 0;
unsigned long currentMillis = 0;
unsigned long frameMillis = 0;

void setup() {
  // Delay for recovery.
  delay(RECOVERY_DELAY);
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<BUCKLE_LED_TYPE,BUCKLE_DATA_PIN,BUCKLE_COLOR_ORDER>(buckle_leds, BUCKLE_NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<BELT_LED_TYPE,BELT_DATA_PIN,BELT_CLK_PIN,BELT_COLOR_ORDER>(belt_leds, belts[currentBelt].num).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
}

void loop() {
  // Calculate time since last frame.
  currentMillis = millis();
  frameMillis = currentMillis - lastMillis;

  findState();
  drawFrames();
  writeFrames();

  // Update lastMillis.
  lastMillis = currentMillis;
}

// Updates the global state variable.
void findState() {
  // Start mode for two seconds
  if (state == START && currentMillis>(RECOVERY_DELAY+2000)) {
    state = RUN;
  }
}

// Draws the frames depending on state.
void drawFrames() {
  switch (state) {
    case START:
      drawTest();
      break;
    case RUN:
      drawConfetti();
      break;
  }
}

// Output the frames to the LED strips.
void writeFrames() {
  // Send the 'leds' arrays to the LED strips.
  FastLED.show();
  // Delay to keep the framerate modest.
  FastLED.delay(1000/FRAMES_PER_SECOND);
}

// A test to draw at startup.
void drawTest () {
  for(int i = 0; i < BUCKLE_NUM_LEDS; i++) {
    buckle_leds[i] = CRGB::White;
  }
  for(int i = 0; i <belts[currentBelt].num; i++) {
    belt_leds[i] = CRGB::White;
  }
}

// Random colored speckles that blink in and fade smoothly.
void drawConfetti() {
  fadeToBlackBy(buckle_leds, BUCKLE_NUM_LEDS, 5);
  int pos = random16(BUCKLE_NUM_LEDS);
  buckle_leds[pos] += CHSV( gHueMin + random8(range), 255, 180);

  fadeToBlackBy(belt_leds, belts[currentBelt].num, 5);
  int belt_pos = random16(belts[currentBelt].num);
  belt_leds[belt_pos] += CHSV( gHueMin + random8(range), 255, 180);
}

