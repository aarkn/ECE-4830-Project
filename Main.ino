/*
  LED Control

  This example scans for BLE peripherals until one with the advertised service
  "19b10000-e8f2-537e-4f6c-d104768a1214" UUID is found. Once discovered and connected,
  it will remotely control the BLE Peripheral's LED, when the button is pressed or released.

  The circuit:
  - Arduino MKR WiFi 1010, Arduino Uno WiFi Rev2 board, Arduino Nano 33 IoT,
    Arduino Nano 33 BLE, or Arduino Nano 33 BLE Sense board.
  - Button with pull-up resistor connected to pin 2.

  You can use it with another board that is compatible with this library and the
  Peripherals -> LED example.

  This example code is in the public domain.
*/

#include <ArduinoBLE.h>
#include <Arduino_LSM9DS1.h>

// variables for button
const int buttonPin = 2;
int oldButtonState = LOW;
float ax, ay, az;
float gx, gy, gz;
float testF = 64.2;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // configure the button pin as input
  pinMode(buttonPin, INPUT);

  // initialize the BLE hardware
  BLE.begin();

  Serial.println("BLE Central - LED control");

  // start scanning for peripherals
  BLE.scanForUuid("19b10000-e8f2-537e-4f6c-d104768a1214");

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  Serial.print("Accelerometer sample rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.println(" Hz");
  Serial.println();
  Serial.println("Acceleration in G's");
  Serial.println("X\tY\tZ");
  Serial.print("Gyroscope sample rate = ");
  Serial.print(IMU.gyroscopeSampleRate());
  Serial.println(" Hz");
  Serial.println();
  Serial.println("Gyroscope in degrees/second");
  Serial.println("X\tY\tZ");

}

void loop() {
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(ax, ay, az);
    //Serial.print(x);
    //Serial.print("\t");
    //Serial.print(y);
    //Serial.print("\t");
    //Serial.print(z);
    //Serial.print("\n");        
  }

  if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(gx, gy, gz);
  }
  // check if a peripheral has been discovered
  BLEDevice peripheral = BLE.available();

  if (peripheral) {
    // discovered a peripheral, print out address, local name, and advertised service
    Serial.print("Found ");
    Serial.print(peripheral.address());
    Serial.print(" '");
    Serial.print(peripheral.localName());
    Serial.print("' ");
    Serial.print(peripheral.advertisedServiceUuid());
    Serial.println();

    if (peripheral.localName() != "LED") {
      return;
    }

    // stop scanning
    BLE.stopScan();

    sendData(peripheral);

    // peripheral disconnected, start scanning again
    BLE.scanForUuid("19b10000-e8f2-537e-4f6c-d104768a1214");
  }
}

void sendData(BLEDevice peripheral) {
  // connect to the peripheral
  Serial.println("Connecting ...");

  if (peripheral.connect()) {
    Serial.println("Connected");
  } else {
    Serial.println("Failed to connect!");
    return;
  }

  // discover peripheral attributes
  Serial.println("Discovering attributes ...");
  if (peripheral.discoverAttributes()) {
    Serial.println("Attributes discovered");
  } else {
    Serial.println("Attribute discovery failed!");
    peripheral.disconnect();
    return;
  }

  // retrieve the LED characteristic
  BLECharacteristic AxCharacteristic = peripheral.characteristic("19b10001-e8f2-537e-4f6c-d104768a1214");
  BLECharacteristic AyCharacteristic = peripheral.characteristic("19b10002-e8f2-537e-4f6c-d104768a1214");
  BLECharacteristic AzCharacteristic = peripheral.characteristic("19b10003-e8f2-537e-4f6c-d104768a1214");
  BLECharacteristic GxCharacteristic = peripheral.characteristic("19b10004-e8f2-537e-4f6c-d104768a1214");
  BLECharacteristic GyCharacteristic = peripheral.characteristic("19b10005-e8f2-537e-4f6c-d104768a1214");
  BLECharacteristic GzCharacteristic = peripheral.characteristic("19b10006-e8f2-537e-4f6c-d104768a1214");

  while (peripheral.connected()) {
    // while the peripheral is connected
    float fx, fy, fz;
    float fgx, fgy, fgz;
    
    int ix, iy, iz;
    int igx, igy, igz;

    // read the button pin
    int buttonState = digitalRead(buttonPin);

    //if (oldButtonState != buttonState) {
      // button changed
      //oldButtonState = buttonState;

      //if (buttonState) {
        //Serial.println("button pressed");

        // button is pressed, write 0x01 to turn the LED on
        //for (int i = 0; i < 60; i++) {
          IMU.readAcceleration(ax, ay, az);
          fx = ax * 100;
          fy = ay * 100;
          fz = az * 100;
          
          ix = fx;
          iy = fy;
          iz = fz;

          AxCharacteristic.writeValue((byte)ix);
          AyCharacteristic.writeValue((byte)iy);
          AzCharacteristic.writeValue((byte)iz);

          IMU.readGyroscope(gx, gy, gz);

          fgx = gx * 100;
          fgy = gy * 100;
          fgz = gz * 100;

          igx = fgx;
          igy = fgy;
          igz = fgz;

          GxCharacteristic.writeValue((byte)igx);
          GyCharacteristic.writeValue((byte)igy);
          GzCharacteristic.writeValue((byte)igz);
        //}
        
      //} else {
       // Serial.println("button released");

      //}
    //}
  }

  Serial.println("Peripheral disconnected");
}
