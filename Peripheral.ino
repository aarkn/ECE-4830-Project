/*
Gets acceleromter data from main device
*/

#include <ArduinoBLE.h>

BLEService peripheralService("19B10000-E8F2-537E-4F6C-D104768A1214"); // BLE LED Service

// BLE LED Switch Characteristic - custom 128-bit UUID, read and writable by central
BLEIntCharacteristic AxCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLEIntCharacteristic AyCharacteristic("19B10002-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLEIntCharacteristic AzCharacteristic("19B10003-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLEIntCharacteristic GxCharacteristic("19B10004-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLEIntCharacteristic GyCharacteristic("19B10005-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLEIntCharacteristic GzCharacteristic("19B10006-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);

const int ledPin = LED_BUILTIN; // pin to use for the LED

int ax, ay, az;
int gx, gy, gz;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // set LED pin to output mode
  pinMode(ledPin, OUTPUT);

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");

    while (1);
  }

  // set advertised local name and service UUID:
  BLE.setLocalName("LED");
  BLE.setAdvertisedService(peripheralService);

  // add the characteristic to the service
  peripheralService.addCharacteristic(AxCharacteristic);
  peripheralService.addCharacteristic(AyCharacteristic);
  peripheralService.addCharacteristic(AzCharacteristic);
  peripheralService.addCharacteristic(GxCharacteristic);
  peripheralService.addCharacteristic(GyCharacteristic);
  peripheralService.addCharacteristic(GzCharacteristic);

  // add service
  BLE.addService(peripheralService);

  // set the initial value for the characeristic:
  AxCharacteristic.writeValue(0);
  AyCharacteristic.writeValue(0);
  AzCharacteristic.writeValue(0);
  GxCharacteristic.writeValue(0);
  GyCharacteristic.writeValue(0);
  GzCharacteristic.writeValue(0);

  // start advertising
  BLE.advertise();

  Serial.println("BLE LED Peripheral");
}

void loop() {
  // listen for BLE peripherals to connect:
  BLEDevice central = BLE.central();

  // if a central is connected to peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());

    // while the central is still connected to peripheral:
    while (central.connected()) {
      // if the remote device wrote to the characteristic,
      // use the value to control the LED:
      if (AxCharacteristic.written()) {
          //Serial.print("AX: ");
          ax = AxCharacteristic.value();
          Serial.print(ax);
          Serial.print("\t");
      }
      if (AyCharacteristic.written()) {
        ay = AyCharacteristic.value();
        //Serial.print("AY: ");
        Serial.print(ay);
        Serial.print("\t");        
      }
      if (AzCharacteristic.written()) {
        az = AzCharacteristic.value();
        //Serial.print("AZ: ");
        Serial.print(az);
        Serial.print("\t");          
      }
      if (GxCharacteristic.written()) {
        gx = GxCharacteristic.value();
        //Serial.print("GX: ");
        Serial.print(gx);
        Serial.print("\t");          
      }
      if (GyCharacteristic.written()) {
        gy = GyCharacteristic.value();
        //Serial.print("GY: ");
        Serial.print(gy);
        Serial.print("\t");          
      }
      if (GzCharacteristic.written()) {
        gz = GzCharacteristic.value();
        //Serial.print("GZ: ");
        Serial.print(gz);
        Serial.print("\n");          
      }                   
    }

    // when the central disconnects, print it out:
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }
}
