#ifndef EFFECTS_H
#define EFFECTS_H
#include <FastLED.h>
#include <vector>
#include <functional>

void auroraBreath(CRGB* leds, uint16_t numLeds, uint8_t breathSpeed);
void nordicBreath(CRGB* leds, uint16_t numLeds, uint8_t breathSpeed);
void fireEffect(CRGB* leds, uint16_t numLeds, uint16_t effectSpeed);

struct Effect {
	const char* name;
	std::function<void(CRGB*, uint16_t, uint8_t)> function;
    const uint8_t hexCode;
};

extern Effect effects[3];

#endif