#include <FastLED.h>
#include <vector>
#include <functional>

struct Effect {
  const char* name;
  std::function<void(CRGB*, uint16_t, uint8_t)> func;
  const uint8_t hexCode;
};

CRGBPalette16 auroraPalette = CRGBPalette16(
  CRGB(0, 10, 20), CRGB(0, 80, 20), CRGB(0, 180, 60), CRGB(40, 255, 120),
  CRGB(120, 180, 255), CRGB(160, 80, 255), CRGB(80, 20, 120), CRGB(0, 10, 20),
  CRGB(0, 10, 20), CRGB(0, 80, 20), CRGB(0, 180, 60), CRGB(40, 255, 120),
  CRGB(120, 180, 255), CRGB(160, 80, 255), CRGB(80, 20, 120), CRGB(0, 10, 20)
);

CRGBPalette16 nordicPalette = CRGBPalette16(
  CRGB(0, 0, 50), CRGB(0, 0, 120), CRGB(0, 50, 180), CRGB(0, 120, 220),
  CRGB(0, 180, 255), CRGB(40, 200, 255), CRGB(80, 220, 255), CRGB(120, 240, 255),
  CRGB(0, 0, 50), CRGB(0, 0, 120), CRGB(0, 50, 180), CRGB(0, 120, 220),
  CRGB(0, 180, 255), CRGB(40, 200, 255), CRGB(80, 220, 255), CRGB(120, 240, 255)
);
CRGBPalette16 campfirePalette = CRGBPalette16(
  CRGB(0, 0, 0), CRGB(30, 2, 0), CRGB(90, 8, 0), CRGB(180, 8, 0),
  CRGB(255, 16, 0), CRGB(255, 32, 0), CRGB(255, 27, 0), CRGB(255, 45, 0),
  CRGB(0, 0, 0), CRGB(30, 4, 0), CRGB(90, 8, 0), CRGB(180, 8, 0),
  CRGB(255, 16, 0), CRGB(255, 32, 0), CRGB(255, 27, 0), CRGB(255, 45, 0)
);
CRGB firePalette[56] = {
  // segment 0: 30,2 -> 90,8
  CRGB(30,2,0), CRGB(37,2,0), CRGB(45,3,0), CRGB(52,4,0),
  CRGB(60,5,0), CRGB(67,5,0), CRGB(75,6,0), CRGB(82,7,0),
  // segment 1: 90,8 -> 180,8
  CRGB(90,8,0), CRGB(101,8,0), CRGB(112,8,0), CRGB(123,8,0),
  CRGB(135,8,0), CRGB(146,8,0), CRGB(157,8,0), CRGB(168,8,0),
  // segment 2: 180,8 -> 255,16
  CRGB(180,8,0), CRGB(189,9,0), CRGB(198,10,0), CRGB(208,11,0),
  CRGB(217,12,0), CRGB(226,13,0), CRGB(236,14,0), CRGB(245,15,0),
  // segment 3: 255,16 -> 255,32
  CRGB(255,16,0), CRGB(255,18,0), CRGB(255,20,0), CRGB(255,22,0),
  CRGB(255,24,0), CRGB(255,26,0), CRGB(255,28,0), CRGB(255,30,0),
  // segment 4: 255,32 -> 255,27
  CRGB(255,32,0), CRGB(255,31,0), CRGB(255,30,0), CRGB(255,30,0),
  CRGB(255,29,0), CRGB(255,28,0), CRGB(255,28,0), CRGB(255,27,0),
  // segment 5: 255,27 -> 255,45
  CRGB(255,27,0), CRGB(255,29,0), CRGB(255,31,0), CRGB(255,33,0),
  CRGB(255,36,0), CRGB(255,38,0), CRGB(255,40,0), CRGB(255,42,0),
  // repeat last base color 8× (as in interpolation scheme)
  CRGB(255,45,0), CRGB(255,45,0), CRGB(255,45,0), CRGB(255,45,0),
  CRGB(255,45,0), CRGB(255,45,0), CRGB(255,45,0), CRGB(255,45,0)
};

static void applyBreathPalette(CRGB* leds, uint16_t numLeds, uint8_t breathSpeed, CRGBPalette16& palette, uint8_t& breath) {
  for (uint16_t i = 0; i < numLeds; ++i) {
    leds[i] = ColorFromPalette(palette, (i * 255 / numLeds + breath) % 255);
  }
  breath += breathSpeed;
}

void auroraBreath(CRGB* leds, uint8_t numLeds, uint8_t breathSpeed) {
  static uint8_t breath = 0;
  applyBreathPalette(leds, numLeds, breathSpeed, auroraPalette, breath);
}

void nordicBreath(CRGB* leds, uint8_t numLeds, uint8_t breathSpeed) {
  static uint8_t breath = 0;
  applyBreathPalette(leds, numLeds, breathSpeed, nordicPalette, breath);
}

void fireRunner(CRGB* leds, uint8_t numLeds, uint8_t breathSpeed) {
  static uint8_t breath = 0;
  applyBreathPalette(leds, numLeds, breathSpeed, campfirePalette, breath);
}
void fireEffect(CRGB* leds, uint8_t numLeds, uint16_t effectSpeed) {
  const int paletteSize = sizeof(firePalette) / sizeof(firePalette[0]);

  static std::vector<float> idxs;
  static std::vector<float> prevIdxs;
  if (idxs.size() != numLeds) {
    idxs.assign(numLeds, 0.0f);
    prevIdxs.assign(numLeds, 0.0f);
    for (uint16_t i = 0; i < numLeds; ++i) {
      idxs[i] = (float)(random(0, paletteSize));
      prevIdxs[i] = idxs[i];
    }
  }

  const float jitterScale = 1.0f; // increased jitter for faster changes
  const float temporalSmooth = 0.5f; // lower => faster response to new values

  int zoneCount = max(2, (int)(numLeds / 6));
  int zoneSize = (numLeds + zoneCount - 1) / zoneCount;
  static std::vector<float> zoneBases;
  if ((int)zoneBases.size() != zoneCount) {
    zoneBases.assign(zoneCount, 0.0f);
    for (int z = 0; z < zoneCount; ++z) zoneBases[z] = (float)random(0, paletteSize);
  }

  for (int z = 0; z < zoneCount; ++z) {
    float r = (float)(random(-100, 101)) / 100.0f;
    float delta = r * jitterScale * 0.8f;
    float v = zoneBases[z] + delta;
    if (v < 0.0f) v = 0.0f;
    if (v > (float)(paletteSize - 1)) v = (float)(paletteSize - 1);
    zoneBases[z] = zoneBases[z] * temporalSmooth + v * (1.0f - temporalSmooth);
  }

  for (uint16_t i = 0; i < numLeds; ++i) {
    int z = i / zoneSize;
    if (z >= zoneCount) z = zoneCount - 1;
    float localR = (float)(random(-100, 101)) / 100.0f;
    float localDelta = localR * jitterScale * 0.3f;
    float v = zoneBases[z] + localDelta;
    if (v < 0.0f) v = 0.0f;
    if (v > (float)(paletteSize - 1)) v = (float)(paletteSize - 1);
    idxs[i] = prevIdxs[i] * temporalSmooth + v * (1.0f - temporalSmooth);
  }

  std::vector<float> smoothIdxs(numLeds);
  for (uint16_t i = 0; i < numLeds; ++i) {
    float left = (i == 0) ? idxs[i] : idxs[i - 1];
    float right = (i + 1 < numLeds) ? idxs[i + 1] : idxs[i];
    smoothIdxs[i] = (left + 2.0f * idxs[i] + right) / 4.0f;
  }
  idxs.swap(smoothIdxs);

  std::vector<CRGB> newCols(numLeds);
  for (uint16_t i = 0; i < numLeds; ++i) {
    float fi = idxs[i];
    int a = (int)floor(fi);
    int b = a + 1;
    if (a < 0) a = 0;
    if (b >= paletteSize) b = paletteSize - 1;
    float t = fi - (float)a;
    CRGB ca = firePalette[a];
    CRGB cb = firePalette[b];
    uint8_t r = (uint8_t)((1.0f - t) * (int)ca.r + t * (int)cb.r);
    uint8_t g = (uint8_t)((1.0f - t) * (int)ca.g + t * (int)cb.g);
    uint8_t bl = (uint8_t)((1.0f - t) * (int)ca.b + t * (int)cb.b);
    newCols[i] = CRGB(r, g, bl);
  }

  prevIdxs = idxs;

  static std::vector<CRGB> prevCols;
  if (prevCols.size() != numLeds) prevCols.assign(numLeds, CRGB::Black);

  uint16_t sp = effectSpeed > 255 ? 255 : effectSpeed;
  uint8_t blendAmt = (uint8_t)(16 + ((uint32_t)sp * (240 - 16) / 255));

  for (uint16_t i = 0; i < numLeds; ++i) {
    CRGB blended = prevCols[i];
    nblend(blended, newCols[i], blendAmt);
    leds[i] = blended;
    prevCols[i] = blended;
  }

  delay(max<uint16_t>(1, effectSpeed));
}

Effect effects[] = {
  {"Aurora",[](CRGB* leds, uint8_t numLeds, uint8_t speed){auroraBreath(leds, numLeds, speed);},0x01},
  {"Nordic", [](CRGB* leds, uint8_t numLeds, uint8_t speed){nordicBreath(leds, numLeds, speed);}, 0x02},
  {"CampFire", [](CRGB* leds, uint8_t numLeds, uint8_t speed){fireEffect(leds, numLeds, speed);}, 0x03}
};