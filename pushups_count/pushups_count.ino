#include <Arduino_LSM9DS1.h>

//Pushup Code
int repCount = 0;
bool t1, t2, t3 = false;

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
    //ax = ax * 100;
    ay = ay * 100;
    //az = az * 100;
    //Serial.print("AX:");
    //Serial.print(ax);
    //Serial.print("\tAY:");
    //Serial.print(ay);
    //Serial.print("\tAZ:");
    //Serial.print(az);
    //Serial.print("\t");
    
    IMU.readGyroscope(gx, gy, gz);
    //Serial.print("GX:");
    //Serial.print(gx);
    //Serial.print("\tGY:");
    //Serial.print(gy);
    //Serial.print("\tGZ:");
    //Serial.print(gz);
    //Serial.print("\n");           
  }

  if (gz > 100)
  {
    t1 = true;
    Serial.println("t1 is true");
    //From what the clips show, I believe what I was able to deduce was
    //the lowest part of the pushup is reached when Gz passes 100 on the graph.
    //The pushup seems to be complete when Gz passes -100 soon after. (The individual
    //successfully reached the starting position of the pushup. When another pushup is
    //started, Gz changes rapidly from -100 back to the postive region of our graph.
    //While the individual is pushing themselves up, Gz slowly moves into the negative
    //region of the graph.
    //I chose to start measuring for a pushup when Gz surpasses +100 because at this point,
    //the individual's arms are bent enough (they are low enough) to count as a successful pushup.
  }
  if (t1 && (ay > 100)) 
  {
    t2 = true;
    Serial.println("t2 is true");
    //Here, the individual pushes themself up off of the ground. Ay was consistently surpassing
    //+100 right before the indvidual reaches the starting position of the pushup again.  
  }
  if (t2 && (gy > 20))
  {
    t3 = true;
    Serial.print("t3 is true\t");
    Serial.print(t1);
    Serial.print("\t");
    Serial.println(t2);
    //Here, I am just checking to ensure Gy > 0, but I arbitrarily set the threshold to 20.
  }
  if (t1 && t2 && t3 && (gz < -85)) {
    repCount++;
    Serial.print("Pushup Successful!");
    Serial.print("Count:");
    Serial.print(repCount);
    Serial.print("\n");
    t1 = false;
    t2 = false;
    t3 = false;
  }
}
