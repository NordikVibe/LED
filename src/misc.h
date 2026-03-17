#ifndef MISC_H
#define MISC_H

#include <Arduino.h>
#include <FastLED.h>

void dataStreamWorker(std::string DataStreamPacket, CRGB* leds, uint8_t NUM_LEDS);
std::function<void()> effectWorkerFunc(std::string DataStreamPacket, CRGB* leds, uint8_t NUM_LEDS);
uint8_t fadeBrigtness(uint8_t currentBrightness, uint8_t targetBrightness, uint8_t step);
void smoothTransitionStep(CRGB* leds, const CRGB* target, uint8_t NUM_LEDS, uint8_t step);

extern CRGB targetColor[29];
#endif
