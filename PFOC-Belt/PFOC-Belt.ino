// Print Debug to Serial
#define DEBUG                1

// Bluetooth
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"

// Bluetooth setup
#define FACTORYRESET_ENABLE  1     // Reset BLE NVRAM
#define BUFSIZE            128   // Size of the read buffer for incoming data
#define BLUEFRUIT_SPI_CS     8
#define BLUEFRUIT_SPI_IRQ    7
#define BLUEFRUIT_SPI_RST    4
Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

// FastLED
#include "FastLED.h"
FASTLED_USING_NAMESPACE
#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

// Setup all pins/settings for LED strips
#include "beltConfig.h"

// BELT SELECT! See beltConfig.h
beltColor currentBelt = PURPLE;

#define RECOVERY_DELAY    2000
#define BRIGHTNESS          48
#define FRAMES_PER_SECOND  120
// Animation color settings
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

// Store last and current frame times to find frame interval
unsigned long lastMillis = 0;
unsigned long currentMillis = 0;
unsigned long frameMillis = 0;

// Main Setup
void setup() {
  // Setupl FastLED LED strip configuration
  FastLED.addLeds<BUCKLE_LED_TYPE,BUCKLE_DATA_PIN,BUCKLE_COLOR_ORDER>(buckle_leds, BUCKLE_NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<BELT_LED_TYPE,BELT_DATA_PIN,BELT_CLK_PIN,BELT_COLOR_ORDER>(belt_leds, belts[currentBelt].num).setCorrection(TypicalLEDStrip);

  // Set master brightness control.
  FastLED.setBrightness(BRIGHTNESS);
  setBuckle(0, CRGB::Green);

  // Delay for recovery mode
  delay(RECOVERY_DELAY);
  setBuckle(1, CRGB::Green);

  // Setup serial, no wait.
  Serial.begin(115200);
  Serial.println(F("PFOC-Belt Serial")); //F() stores string in Flash memory
  setBuckle(2, CRGB::Green);

  // Start BLE
  if (ble.begin(false)) {
    // Found Bluefruit
    setBuckle(3, CRGB::Green);
  }
  else setBuckle(3, CRGB::Red);

  // Factory Reset
  if (FACTORYRESET_ENABLE) {
    /* Perform a factory reset to make sure everything is in a known state */
    if (ble.factoryReset(true)) {
      // Reset Success
      setBuckle(4, CRGB::Green);
    }
    else setBuckle(4, CRGB::Red);
  }
  Serial.println(F("Switching to DATA mode"));
  ble.setMode(BLUEFRUIT_MODE_DATA);
  setBuckle(5, CRGB::Green);

  ble.setConnectCallback(callbackConnected);
  /* Disable command echo from Bluefruit */
  ble.echo(false);
  setBuckle(6, CRGB::Green);
  setBuckle(7, CRGB::Green);
}

// Sets Buckle LEDs slowly during setup()
void setBuckle(uint8_t led, CRGB color) {
  buckle_leds[led] = color;
  FastLED.show();
  delay(100);
}

// Main Loop
void loop() {
  // Delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND);
  //ble.update(100);

  // Calculate time since last frame.
  currentMillis = millis();
  frameMillis = currentMillis - lastMillis;

  findState();
  drawFrames();

  // Send the 'leds' arrays to the LED strips.
  FastLED.show();

  // Update lastMillis.
  lastMillis = currentMillis;
}

// Set the last Buckle LED to Blue when Bluetooth connects
// Callback set in setup()
void callbackConnected() {
  if (DEBUG) Serial.println(F("BLE Connected"));
  setBuckle(7, CRGB::Blue);
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


// DRAWING

// A test to draw at startup.
void drawTest() {
  for(int i = 0; i < BUCKLE_NUM_LEDS; i++) {
    buckle_leds[i] = CRGB::White;
  }
  for(int i = 0; i < belts[currentBelt].num; i++) {
    belt_leds[i] = CRGB::White;
  }
}

// Colored speckles that blink in and fade smoothly.
void drawConfetti() {
  fadeToBlackBy(buckle_leds, BUCKLE_NUM_LEDS, 5);
  int pos = random16(BUCKLE_NUM_LEDS);
  buckle_leds[pos] += CHSV( gHueMin + random8(range), 255, 255);

  fadeToBlackBy(belt_leds, belts[currentBelt].num, 5);
  int belt_pos = random16(belts[currentBelt].num);
  belt_leds[belt_pos] += CHSV( gHueMin + random8(range), 255, 255);
}

