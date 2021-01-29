#include <stdint.h>
#include <Wire.h>
#include <LIDARLite_v3HP.h>
LIDARLite_v3HP myLidarLite;
#define FAST_I2C


//Motor code variables
const int YS = 6; //defining pins
const int YD = 5;
const int PS = 3; //pitch step pin
const int PD = 2; //pitch direction pin

int motorSteps = 1600;       //no. motor steps for 1 rotation
unsigned long Speed = 1000000 / (motorSteps*.5);   //to change axis motor speed. number is hz

int yaw = 1;
int pitch = 1;
int yawPos = 0;
int pitchPos = 0;
int flag = 0;

//Serial data system variables
const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];        // temporary array for use when parsing

boolean newData = false;
int PS1;
int PS2;
int YS1;
int YS2;
int dist1 = 0;
int dist2 = 0;
int ontarget = 0;

void setup() {
  Serial.begin(115200);
  pinMode(YS, OUTPUT);
  pinMode(YD, OUTPUT);
  pinMode(PS, OUTPUT);
  pinMode(PD, OUTPUT);
  digitalWrite(YS, LOW);
  digitalWrite(PS, LOW);

  // Initialize Arduino I2C (for communication to LidarLite)
  Wire.begin();
#ifdef FAST_I2C
#if ARDUINO >= 157
  Wire.setClock(400000UL); // Set I2C frequency to 400kHz (for Arduino Due)
#else
  TWBR = ((F_CPU / 400000UL) - 16) / 2; // Set I2C frequency to 400kHz
#endif
#endif

  // Configure the LidarLite internal parameters so as to lend itself to
  // various modes of operation by altering 'configure' input integer to
  // anything in the range of 0 to 5. See LIDARLite_v3HP.cpp for details.
  myLidarLite.configure(2);
  //delay(1000)
}

void loop() {
  //Serial.println("<0,1,1>");

  recvWithStartEndMarkers();
  if (newData == true) {
    strcpy(tempChars, receivedChars);
    // this temporary copy is necessary to protect the original data
    //   because strtok() used in parseData() replaces the commas with \0
    parseData();
    //showParsedData();
    newData = false;
  }

  if (flag == 0) {
    sweep();
  }
  if (flag == 1 && dist1 == 0) {
    track();
    if (pitch == 1 && yaw == 1)
      ontarget++;
  }
  if (pitch != 1 || yaw != 1) {
    ontarget = 0;
  }
  if (ontarget == 1000) {
    dist1 = lidar();
    PS1 = pitchPos;
    YS1 = yawPos;
    ontarget = 0;
    Serial.print("distance1:");
    Serial.println(dist1);
    Serial.print("Pitch1:");
    Serial.println(PS1);
    Serial.print("Yaw1:");
    Serial.println(YS1);
  }
  if (flag == 2 && dist2 == 0) {
    track();
    if (pitch == 1 && yaw == 1)
      ontarget++;
  }
  if (pitch != 1 || yaw != 1) {
    ontarget = 0;
  }
  if (ontarget == 1000) {
    dist2 = lidar();
    PS2 = pitchPos;
    YS2 = yawPos;
    ontarget = 0;
    Serial.print("distance2:");
    Serial.println(dist2);
    Serial.print("Pitch2:");
    Serial.println(PS2);
    Serial.print("Yaw2:");
    Serial.println(YS2);
  }
}




// call lidar() to get average of 10 lidar measurements
int lidar() {
  int i = 0;
  int distance = 0;
  while (i < 10) {
    myLidarLite.waitForBusy();
    myLidarLite.takeRange();
    myLidarLite.waitForBusy(); //may be able to remove this for more speed. have not tested
    distance = distance + myLidarLite.readDistance();
    //Serial.println(distance);
    i++;
  }
  distance = distance / 10;
  return distance;
}
