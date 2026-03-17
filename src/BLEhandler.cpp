#include "BLEhandler.h"
#include <Arduino.h>
#include <NimBLEServer.h>
#include <NimBLEUtils.h>

static NimBLEServer* pServer = nullptr;
static NimBLEService* pService = nullptr;
static NimBLECharacteristic* pCharacteristic = nullptr;
static NimBLECharacteristic* ledSizeChar = nullptr;
static NimBLECharacteristic* dialog = nullptr;
static NimBLECharacteristic* dataStream = nullptr;

uint8_t currentMode = 0xF1;
uint8_t brightness = 200;
std::string DataStreamPacket = "";
std::string DialogPacket = "";

class ServerCallbacks: public NimBLEServerCallbacks {
	void onDisconnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo, int reason) override {
		(void)pServer; (void)connInfo; (void)reason;
		delay(100);
		NimBLEDevice::startAdvertising();
	}
};

class DataStreamCallbacks: public NimBLECharacteristicCallbacks {
	void onWrite(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo) override {
		(void)connInfo;
		DataStreamPacket = pCharacteristic->getValue();
	//Tiny chunk by Shadow🩶"
	//char buf[128];
	//snprintf(buf, sizeof(buf), "[%lu] Received message: %s", (unsigned long)millis(), value.c_str());
	//Serial.println(buf);
	}
};

class DialogCallbacks: public NimBLECharacteristicCallbacks {
	void onWrite(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo) override {
		(void)connInfo;
		std::string DialogPacket = pCharacteristic->getValue();
		for (size_t i = 0; i < DialogPacket.length(); ++i) {
			Serial.print((uint8_t)DialogPacket[i], HEX);
			Serial.print(" ");
		}
		Serial.println();
		switch (DialogPacket[0]) {
			case 0xF1:
				currentMode = 0xF1;
				break;
			case 0xF2:
				currentMode = 0xF2;
				break;
			case 0xC0:
				brightness = uint8_t(DialogPacket[1]);
				break;
			default:
				currentMode = 0xF1;
		}
	}
};

void BLEhandler_init(const String& deviceName, uint8_t NUM_LEDS) {
	NimBLEDevice::init(deviceName.c_str());
	NimBLEDevice::setMTU(247);
	NimBLEDevice::setDeviceName(deviceName.c_str());
	pServer = NimBLEDevice::createServer();
	pServer->setCallbacks(new ServerCallbacks);
	pService = pServer->createService(SERVICE_UUID);

	ledSizeChar = pService->createCharacteristic(LED_SIZE_UUID, NIMBLE_PROPERTY::READ);
	dialog = pService->createCharacteristic(DIALOG_UUID, NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY);
	dataStream = pService->createCharacteristic(DATASTREAM_UUID, NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::NOTIFY);

	ledSizeChar->setValue(NUM_LEDS);

	dialog->setCallbacks(new DialogCallbacks);
	dataStream->setCallbacks(new DataStreamCallbacks);

	pService->start();
	NimBLEAdvertising *pAdvertising = NimBLEDevice::getAdvertising();
	pAdvertising->addServiceUUID(SERVICE_UUID);
	pAdvertising->enableScanResponse(true);
	pAdvertising->setPreferredParams(0x06, 0x06);
	pAdvertising->start();
    Serial.println("BLE started:"+String(deviceName));
}

NimBLECharacteristic* BLEhandler_getCharacteristic() { return pCharacteristic; }
NimBLECharacteristic* BLEhandler_getDialogCharacteristic() { return dialog; }
NimBLECharacteristic* BLEhandler_getDataCharacteristic() { return dataStream; }

