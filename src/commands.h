// Modified from:
// https://github.com/adafruit/Bluefruit_LE_Connect_Android/blob/master/app/src/main/assets/neopixel/Neopixel_Arduino.zip

// Little wrapper for ble writes.
void sendResponse(char *response) {
  ble.write(response, strlen(response) * sizeof(char));
}

void commandVersion() {
  if (DEBUG) Serial.println(F("Command: Version check"));
  sendResponse("Neopixel v1.0");  // Cake to work with the Bluefruit LE app
}

// Stores values from the app.
uint8_t width = 0;
uint8_t height = 0;
uint8_t components = 3;  // only 3 and 4 are valid values
uint8_t stride;
void commandSetup() {
  if (DEBUG) Serial.println(F("Command: Setup"));

  width = ble.read();
  height = ble.read();
  components = ble.read();
  stride = ble.read();
  // Throw some out
  ble.read();  // PixelType > /dev/null
  ble.read();  // PixelType<<8 > /dev/null

  sendResponse("OK");
}

// Sets all pixel colors to the provided values until overwritten by an
// animation.
// TODO: Something else?
#define MAXCOMPONENTS 4
void commandClearColor() {
  if (DEBUG) Serial.println(F("Command: ClearColor"));

  // Read color
  uint8_t color[MAXCOMPONENTS];
  for (int j = 0; j < components;) {
    if (ble.available()) {
      color[j] = ble.read();
      j++;
    }
  }

  // Find the hue and change the animation colors
  CHSV hsv = rgb2hsv_approximate(CRGB(color[0], color[1], color[2]));

  gHue = hsv.h;
  gHueMin = gHue - range / 2;

  sendResponse("OK");
}

// Sets the overall brightness
void commandSetBrightness() {
  if (DEBUG) Serial.println(F("Command: SetBrightness"));

  uint8_t brightness = ble.read();
  FastLED.setBrightness(brightness);

  sendResponse("OK");
}

// Sets a pixel color to the provided value until overwritten by an animation.
void commandSetPixel() {
  if (DEBUG) Serial.println(F("Command: SetPixel"));

  // Read position
  uint8_t x = ble.read();
  uint8_t y = ble.read();
  uint32_t pixelIndex = y * width + x;

  // Read color
  uint8_t color[MAXCOMPONENTS];
  for (int j = 0; j < components;) {
    if (ble.available()) {
      color[j] = ble.read();
      j++;
    }
  }

  // Set color, if possible.
  if (pixelIndex < belts[currentBelt].num) {
    belt_leds[pixelIndex] = CRGB(color[0], color[1], color[2]);
  }

  sendResponse("OK");
}

// Gets image data, and drops it all.
void commandImage() {
  if (DEBUG) Serial.println(F("Command: Image"));

  // Receive new pixel buffer
  int size = width * height;
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < components;) {
      if (ble.available()) {
        ble.read();  // > /dev/null
        j++;
      }
    }
  }

  sendResponse("OK");  // Cake
}
