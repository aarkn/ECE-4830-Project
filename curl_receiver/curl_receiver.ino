/*
 * Curl count receiver
 */

#include <ArduinoBLE.h>

BLEService curlCountService("19B10000-E8F2-537E-4F6C-D104768A1214");

BLEIntCharacteristic CurlCount("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial);

  if (!BLE.begin()) {
    Serial.println("Starting BLE failed");
    while(1);
  }

  BLE.setLocalName("Curl Count Receiver");
  BLE.setAdvertisedService(curlCountService);

  curlCountService.addCharacteristic(CurlCount);

  BLE.addService(curlCountService);

  CurlCount.writeValue(0);

  BLE.advertise();

  Serial.println("This device will receive data");

}

void loop() {
  // put your main code here, to run repeatedly:
  BLEDevice central = BLE.central();

  if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());

    while (central.connected()) {
      if (CurlCount.written()) {
        Serial.print("Curl Count: ");
        Serial.print(CurlCount.value());
        Serial.println();
      }
    }
  }

}
