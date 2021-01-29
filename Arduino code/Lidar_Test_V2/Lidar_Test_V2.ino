#include <stdint.h>
#include <Wire.h>
#include <LIDARLite_v3HP.h>
LIDARLite_v3HP myLidarLite;
#define FAST_I2C
/*
Connections:
  LIDAR-Lite 5 Vdc (red) to Arduino 5v
  LIDAR-Lite I2C SCL (green) to Arduino SCL
  LIDAR-Lite I2C SDA (blue) to Arduino SDA
  LIDAR-Lite Ground (black) to Arduino GND
*/
  
//Motor code variables
const int YS = 6; //defining pins
const int YD = 5;
const int PS = 3; //pitch step pin
const int PD = 2; //pitch direction pin

int motorSteps = 1600;       //no. motor steps for 1 rotation. change based on microstepping
unsigned long Speed = 500000/(motorSteps*.5);     //change last number to change motor speed. number is hz

//Internal variables
int yaw = 1;
int pitch = 1;
int yawPos = 0;
int pitchPos = 0;

int distance;
int i = 2;

void setup() {
Serial.begin(115200); //initialize serial

pinMode(YS, OUTPUT); //define pins as outputs
pinMode(YD, OUTPUT);
pinMode(PS, OUTPUT);
pinMode(PD, OUTPUT);
digitalWrite(YS,LOW);
digitalWrite(PS,LOW);
digitalWrite(YD,HIGH);
digitalWrite(PD,LOW);

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
    delay(100);
    Startup();
}

void loop() {
      if (i == 2) {
     //lidar stuff
      myLidarLite.waitForBusy();
      myLidarLite.takeRange();
      myLidarLite.waitForBusy(); //may be able to remove this for more speed. have not tested
      distance = myLidarLite.readDistance();

      //print data to serial in form of yawsteps(3digits),pitchsteps(3),distance(5)
      //total of 13 characters (11+2 commas)-- potentially not true
      //padPrint(yawPos,3); 
      Serial.print(yawPos);
      Serial.print(",");
      //padPrint(pitchPos,3);
      Serial.print(pitchPos);
      Serial.print(",");
      Serial.print(distance);
      //padPrint(distance,5);
      Serial.println();
      //Serial.print("\n"); //uncomment if you need a line break after each string (didnt use when streaming into matlab)
      i = 0;
    }
        
    if (pitchPos != 2) {              //run the loop for 40 pitch increments downwards 
        if (yawPos != motorSteps) {
          digitalWrite(YS,HIGH);
          yawPos++;
        }

        if (yawPos == motorSteps) {
          digitalWrite(PS,HIGH);
          yawPos = 0;
          pitchPos++;
        }

      //step motor
      delayMicroseconds(Speed);
      digitalWrite(YS,LOW);
      digitalWrite(PS,LOW);
      delayMicroseconds(Speed);
    } 
}


void padPrint( int value, int width) {
// pads values with leading zeros to make the number a given width
// taken from https://forum.arduino.cc/index.php?topic=42155.0
char valueStr[6]; // large enough to hold an int
  itoa (value, valueStr, 10);
  int len = strlen(valueStr);
  if(len < width){
    len = width-len;
    while(len--)
     Serial.print('0');
  }
 Serial.print(valueStr);  
}


void Startup() {
  int k = 0;
  while (k == 0) {
    myLidarLite.waitForBusy();
    myLidarLite.takeRange();
    myLidarLite.waitForBusy(); //may be able to remove this for more speed. have not tested
    distance = myLidarLite.readDistance();
    if (distance != 0) {
      digitalWrite(PS,HIGH);
      delayMicroseconds(Speed*200);
      digitalWrite(PS,LOW);
      delayMicroseconds(Speed*200);
    }
    if (distance == 0) {
      k = 1;
    }
  }
  digitalWrite(PD,HIGH);
  
  for (int j = 0; j<43; j++) {
    digitalWrite(PS,HIGH);
    delayMicroseconds(Speed*100);
    digitalWrite(PS,LOW);
    delayMicroseconds(Speed*100);
  }
  digitalWrite(PD,HIGH);
}
