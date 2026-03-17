#ifndef BLEHANDLER_H
#define BLEHANDLER_H

#include <NimBLEDevice.h>
#include <string>

#define SERVICE_UUID        "99b0bf79-fe33-4fa4-9e1c-263398667c40"
#define CHARACTERISTIC_UUID "e9a46c93-13f2-42c8-b7fd-4e9221b929f4"
#define LED_SIZE_UUID       "337597f2-02e7-4cc6-938b-e0125160161b"
#define DIALOG_UUID        "a7fed865-d364-424c-87d6-9e893fb661c4"
#define DATASTREAM_UUID     "2c0a8901-383b-4b83-a88b-6e024a71bc22"

void BLEhandler_init(const String& deviceName, uint8_t NUM_LEDS);
extern std::string DataStreamPacket;
extern std::string DialogPacket;
extern uint8_t currentMode;
extern uint8_t brightness;

NimBLECharacteristic* BLEhandler_getCharacteristic();
NimBLECharacteristic* BLEhandler_getDialogCharacteristic();
NimBLECharacteristic* BLEhandler_getDataCharacteristic();

#endif

