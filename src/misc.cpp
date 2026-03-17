#include "misc.h"
#include <FastLED.h>
#include "Effects.h"
#include <vector>

CRGB targetColor[29];

void dataStreamWorker(std::string DataStreamPacket, CRGB* leds, uint8_t NUM_LEDS) {
  if (DataStreamPacket.length() == NUM_LEDS * 3) {
    for (int i = 0, li = 0; i < (int)DataStreamPacket.length(); i += 3, ++li) {
      uint8_t r = (uint8_t)DataStreamPacket[i];
      uint8_t g = (uint8_t)DataStreamPacket[i + 1];
      uint8_t b = (uint8_t)DataStreamPacket[i + 2];
      targetColor[li] = CRGB(r, g, b);
    }
  }
}
std::function<void()> effectWorkerFunc(std::string DataStreamPacket, CRGB* leds, uint8_t NUM_LEDS) {
  if (DataStreamPacket.length() < 2) return nullptr;

  uint8_t effectCode = DataStreamPacket[1];
  uint8_t speed = DataStreamPacket[2];

  for (auto &e : effects) {
    if (e.hexCode == effectCode) {
        return [&leds, NUM_LEDS, speed, &e]() {
            e.function(leds, NUM_LEDS, speed);
        };
      break;
    }
  }
  return nullptr;
}
uint8_t fadeBrigtness(uint8_t currentBrightness, uint8_t targetBrightness, uint8_t step) {
  if (abs(currentBrightness - targetBrightness) < step) {
    return targetBrightness;
  }
  return currentBrightness < targetBrightness ? currentBrightness + step : currentBrightness - step;
}