/*
  AnalogReadSerial

  Reads an analog input on pin 0, prints the result to the Serial Monitor.
  Graphical representation is available using Serial Plotter (Tools > Serial Plotter menu).
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/AnalogReadSerial
*/

#include "Arduino.h"

unsigned long myTime;

#define S0 10
#define S1 16
#define S2 14
#define S3 15
#define sig A2

#define numInputs 3

void updateMux(int muxNum) {
  digitalWrite(S0, bitRead(muxNum, 0));
  digitalWrite(S1, bitRead(muxNum, 1));
  digitalWrite(S2, bitRead(muxNum, 2));
  digitalWrite(S3, bitRead(muxNum, 3));
}

// the setup routine runs once when you press reset:
void setup() {
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  digitalWrite(S0, 0);
  digitalWrite(S1, 0);
  digitalWrite(S2, 0);
  digitalWrite(S3, 0);
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);
}

// the loop routine runs over and over again forever:
void loop() {
//  for(int i = 0; i < numInputs; i++)
//  {
//    updateMux(i);
//    myTime = millis();
//    while(millis() - myTime < 5000){
//    // read the input on analog pin 0:
      int sensorValue = analogRead(A2);
      // print out the value you read:
      if(sensorValue>0){
        Serial.println(sensorValue);
        delayMicroseconds(250);        // delay in between reads for stability
      }
//    }
//  }
}
