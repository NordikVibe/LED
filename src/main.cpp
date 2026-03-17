#include <Arduino.h>
#include <FastLED.h>
#include "BLEhandler.h"
#include "Effects.h"

//==========================LED Config==========================
#define LED_PIN             15
#define NUM_LEDS            29
#define BRIGHTNESS          200
#define LED_TYPE            WS2812B
#define COLOR_ORDER         GRB
//==========================Button Pin==========================
#define BUTTON_1            19
#define BUTTON_2            18
//==========================Device Name==========================
#define DEVICE_NAME         "Smart LED"
CRGB leds[NUM_LEDS];

uint16_t noiseSpeed = 1;

volatile uint8_t currentMode = 0;

void setup() {
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);

  Serial.begin(115200);
  BLEhandler_init(DEVICE_NAME, NUM_LEDS);
  
  Serial.println("BLE started:"+String(DEVICE_NAME));
}

void loop() {
  // if (DataStreamPacket.length() == NUM_LEDS * 3) {
  //   for (int i = 0; i < DataStreamPacket.length(); i += 3) {
  //     int ledIndex = i / 3;
  //     leds[ledIndex] = CRGB(DataStreamPacket[i], DataStreamPacket[i + 1], DataStreamPacket[i + 2]);
  //   }
  // }
  fireEffect(leds, NUM_LEDS, 10);
  FastLED.show();
}
