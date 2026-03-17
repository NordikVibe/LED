#include <Arduino.h>
#include <FastLED.h>
#include "BLEhandler.h"
#include "Effects.h"
#include "misc.h"

//==========================LED Config==========================
#define LED_PIN             15
#define NUM_LEDS            29
#define INIT_BRIGHTNESS     200
#define LED_TYPE            WS2812B
#define COLOR_ORDER         GRB
//==========================Button Pin==========================
#define BUTTON_1            19
#define BUTTON_2            18
//==========================Device Name==========================
#define DEVICE_NAME         "Smart LED"


CRGB leds[NUM_LEDS];
CRGB leds_backup[NUM_LEDS];
std::function<void()> func = nullptr;
uint8_t old_brightness = INIT_BRIGHTNESS;
static uint8_t displayed_brightness = INIT_BRIGHTNESS;


void sendEffectsList(BLECharacteristic* charPtr) {
    std::vector<uint8_t> buffer;

    for (auto &e : effects) {
        buffer.push_back(e.hexCode);

        uint8_t nameLen = strlen(e.name);
        buffer.push_back(nameLen);

        for (uint8_t i = 0; i < nameLen; i++) { buffer.push_back(e.name[i]); }
    }
}

void setup() {
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(INIT_BRIGHTNESS);

  Serial.begin(115200);
  BLEhandler_init(DEVICE_NAME, NUM_LEDS);

  sendEffectsList(BLEhandler_getDialogCharacteristic());
}

void loop() {
  // Smooth brightness change using `fadeBrigtness` from misc.cpp
  displayed_brightness = fadeBrigtness(displayed_brightness, brightness, 1);
  if (displayed_brightness != old_brightness) {
    FastLED.setBrightness(displayed_brightness);
    old_brightness = displayed_brightness;
  }
  for (int i = 0; i < NUM_LEDS; ++i) {
    leds[i].r = fadeBrigtness(leds[i].r, targetColor[i].r, 1.);
    leds[i].g = fadeBrigtness(leds[i].g, targetColor[i].g, 1);
    leds[i].b = fadeBrigtness(leds[i].b, targetColor[i].b, 1);
  }
  

  switch (currentMode) {
    case 0xF1:
      if (func != nullptr) {
        func = nullptr;
      }
      dataStreamWorker(DataStreamPacket, leds, NUM_LEDS);
      break;
    case 0xF2:
      if (func != nullptr) {
        func();
      } else {
        std::copy(leds, leds + NUM_LEDS, leds_backup);
        func = effectWorkerFunc(DataStreamPacket, leds, NUM_LEDS);
      }
      break;
  }
  FastLED.show();
}
