#include <stdint.h>
#include <Wire.h>
#include <LIDARLite.h>
LIDARLite lidarLite;
#define FAST_I2C

//Motor code variables
const int YS = 6; //defining pins
const int YD = 5;
const int PS = 3; //pitch step pin
const int PD = 2; //pitch direction pin

int motorSteps = 800;       //no. motor steps for 1 rotation. change based on microstepping
unsigned long Speed = 500000/(motorSteps*0.25);     //change last number to change motor speed. number is hz

//Internal variables
int yaw = 1;
int pitch = 1;
int yawPos = 0;
int pitchPos = 0;

int i = 0;
int j = 0;
int distance;

void setup() {
Serial.begin(115200); //initialize serial
pinMode(YS, OUTPUT); //define pins as outputs
pinMode(YD, OUTPUT);
pinMode(PS, OUTPUT);
pinMode(PD, OUTPUT);
digitalWrite(YS,LOW);
digitalWrite(PS,LOW);


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
    Serial.begin(115200); // Initialize serial connection to display distance readings

    lidarLite.begin(0, true); // Set configuration to default and I2C to 400 kHz
    lidarLite.configure(0); // Change this number to try out alternate configurations
}

void loop() {
    
    if (i != 15) {              //run the loop for 15 side to side scans
      
       if (yawPos == 0) {       //if at 0 steps pitch up and sweep right
        digitalWrite(YD,HIGH);
        digitalWrite(PS,HIGH);
        pitchPos++;
        j = 0;
        i++;
       }    
       if (yawPos == motorSteps/4) {     //if at 1/4 revolution (90deg) pitch up and sweep left
        digitalWrite(YD,LOW);
        digitalWrite(PS,HIGH);
        pitchPos++;
        j = 1;
        i++;
       }

       if (j==0) {              
        digitalWrite(YS,HIGH);
        yawPos++; //increment yaw (CW)
       }
       if (j==1) {              
        digitalWrite(YS,HIGH);
        yawPos--; //decrement yaw (CCW)
       }

      //step motor
      delayMicroseconds(Speed);
      digitalWrite(YS,LOW);
      digitalWrite(PS,LOW);
      delayMicroseconds(Speed);


     //lidar stuff
      //lidarLite.waitForBusy();
      //lidarLite.takeRange();
      //lidarLite.waitForBusy(); //may be able to remove this for more speed. have not tested
      distance = lidarLite.distance();


      //print data to serial in form of yawsteps(3digits),pitchsteps(3),distance(5)
      //total of 13 characters (11+2 commas)
      padPrint(yawPos,3); //degrees
      Serial.print(",");
      padPrint(pitchPos,3); //degrees
      Serial.print(",");
      padPrint(distance,5); //cm
      Serial.println("");
      //Serial.print("\n"); //uncomment if you need a line break after each string (didnt use when streaming into matlab)
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
