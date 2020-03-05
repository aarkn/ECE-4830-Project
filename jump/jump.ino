#include <Arduino_LSM9DS1.h>

int repCount = 0;
bool t1, t2, t3, t4 = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while(!Serial);
  Serial.println("Started");

  if(!IMU.begin()) {
    Serial.println("Failed to initialize IMU");
    while(1);
  }

  Serial.println("Accelerometer and Gyroscopic Data Print");
  Serial.println("X\tY\tZ");

}

void loop() {
  // put your main code here, to run repeatedly:
  float ax, ay, az, gx, gy, gz;

  if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable()) {
    IMU.readAcceleration(ax, ay, az);
    ax = ax * 100;
    ay = ay * 100;
    az = az * 100;
    Serial.print("AX:");
    Serial.print(ax);
    Serial.print("\tAY:");
    Serial.print(ay);
    //Serial.print("\tAZ:");
    //Serial.print(az);
    //Serial.print("\t");
    
    IMU.readGyroscope(gx, gy, gz);
    //Serial.print("GX:");
    //Serial.print(gx);
    Serial.print("\tGY:");
    Serial.print(gy);
    Serial.print("\tGZ:");
    Serial.print(gz);
    Serial.print("\n");           
  }

  if (ax < -80)
  {
    t1 = true;
    Serial.println("t1 is true");
  }
  if (t1 && (ax > gz)) {
    t2 = true;
    Serial.println("t2 is true");
  }
  if (t2 && (gz > ay))
  {
    t3 = true;
    Serial.println("t3 is true\t");
    //Serial.print(t1);
    //Serial.print("\t");
    //Serial.println(t2);
  }
  if (t3 && (gz > ax)) {
    t4 = true;
    Serial.println("t4 is true");
  }
  if (t1 && t2 && t3 && t4 && (ay > gz)) {
    repCount++;
    Serial.print("Count:");
    Serial.print(repCount);
    Serial.print("\n");
    t1 = false;
    t2 = false;
    t3 = false;
    t4 = false;
  }

}
