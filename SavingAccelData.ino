#include <Arduino_LSM9DS1.h>
#include <SPI.h>
#include <SD.h>

const int chipSelect = 10;  //Use Digital Pin 10 (D10) for Uno and Nano 33
const int exComplete = 1;   //Arbitrarily chosen digital pin that we can have a pushbutton on.
 
void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Started");

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
  
  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    while (1);                                            //If card fails to be detected, code stops.
  } 
  Serial.println("card initialized.");                    //If card is detected, code continues.
}

void loop() {
  float x, y, z;

  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x, y, z);

    Serial.print(x);
    Serial.print('\t');
    Serial.print(y);
    Serial.print('\t');
    Serial.println(z);
  }
  
  String acceldataString = "";          //String to hold accelerometer data
  acceldataString += "x = "
  acceldataString += String(x);         //Save accel-x value into acceldataString
  acceldataString += ", y = ";              
  acceldataString += String(y);         //Save accel-y value into acceldataString
  acceldataString += ", z = ";
  acceldataString += String(z);         //Save accel-z value into acceldataString
  acceldataString += ", ";

  if(digitalRead(exComplete) == HIGH){  //Data will only be saved once user presses pushbutton to signify an exercise is complete.
                                        //SD Saving might draw a lot of current so we have to limit when we save.
    File accelFile = SD.open("acceldatalog.txt", FILE_WRITE);   //Files are opened 1 at a time. Have to close 1 before opening another.
    if (accelFile) {                                            //If the file is available, acceldataString will write to it.  
      accelFile.println(acceldataString);
      accelFile.close();
    }
    else {
      Serial.println("error opening acceldatalog.txt");         //Error opening file
    }
  }
}

//Here are all of the examples, tutorials, references, etc. I used.
//https://www.arduino.cc/en/Tutorial/Datalogger
//https://www.arduino.cc/en/Tutorial/ReadWrite
//https://www.arduino.cc/en/Tutorial/CardInfo
//https://www.arduino.cc/en/Reference/SD
//https://www.arduino.cc/en/Guide/NANO33BLESense

//This is for the SD Breakout Board
//https://learn.adafruit.com/adafruit-micro-sd-breakout-board-card-tutorial?view=all
