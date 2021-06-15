/* Thigns to implement
 * Pads: 
 *  - Register when hit
 *    - Velocity which depends on voltage
 *    - Reading voltage levels 
 *      = Reading until peak which is then used for velocity
 *  - Send out a midi message when hit
 *    - Through USB
 *    - Notes
 *  - Ignore small vibrations
 *    - Set a minumum threshold
 *  - Ignore residual vibration
 *    - ? Set a timer that ignores a certain amount of time after highest peak
 *    
 * Multiplex:
 *  - Advance to next piezo
 *  - Loop for valid inputs
 *    - Only read for the amount of pads used
 */

#include "Piezo.h"
#include "Arduino.h"
#include <MIDIUSB.h>

// Define physical pins used on Pro Mirco board
#define S0 10
#define S1 16
#define S2 14
#define S3 15
#define sig A2

// Number of inputs used on multiplexor
#define numInputs 2

// Piezo::Piezo(int sigPin, byte pitch, byte channel, int thresh, unsigned long tDelay, int maxValue)
/*  sigPin: Analog input pin used on Arduino
 *  pitch: Musical note being sent. pitch of 60 corresponds to musical note C4. pitch of 48 corresponds to C3 (middle C)
 *  channel: MIDI channel which message should be sent on (0 to 15) 
 *  thresh: Minimum threshold for signal to register a hit. Adjust higher to get rid of "ghost" hits
 *  tDelay: Minimum time delay (ms) allowed for next message to be sent. Adjust higher to get rid of "ghost" hits
 *  maxValue: Maximum signal value (0 to 1023). If signal is read higher than maxValue, velocity of 127 (highest) is sent. Adjust to change dynamic range sensitivity.
 */
Piezo PI1(sig, 64, 0, 20, 40, 300);
Piezo PI2(sig, 65, 0, 20, 50, 240);
Piezo PI3(sig, 66, 0, 40, 40, 400);
Piezo PI4(sig, 67, 0, 40, 50, 400);
Piezo PI5(sig, 68, 0, 40, 40, 400);
Piezo PI6(sig, 69, 0, 40, 50, 400);
Piezo PI7(sig, 70, 0, 40, 40, 400);
Piezo PI8(sig, 71, 0, 40, 50, 400);
Piezo PI9(sig, 72, 0, 40, 50, 400);
Piezo PI10(sig, 73, 0, 40, 50, 400);
Piezo PI11(sig, 74, 0, 40, 50, 400);
Piezo PI12(sig, 75, 0, 40, 50, 400);
Piezo PI13(sig, 76, 0, 40, 50, 400);
Piezo PI14(sig, 77, 0, 40, 50, 400);
Piezo PI15(sig, 78, 0, 40, 50, 400);
Piezo PI16(sig, 79, 0, 40, 50, 400);

// Makes an array of Piezos to iterate over
Piezo *piezo1[] {&PI1, &PI2, &PI3, &PI4, &PI5, &PI6, &PI7, &PI8};

// Sends 4 bit signal to mux to choose which piezo to read
void updateMux(int muxNum) {
  digitalWrite(S0, bitRead(muxNum, 0));
  digitalWrite(S1, bitRead(muxNum, 1));
  digitalWrite(S2, bitRead(muxNum, 2));
  digitalWrite(S3, bitRead(muxNum, 3));
}

// Sends MIDI note on message through USB
void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

// Sends MIDI not off message through USB
void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}

// Sends MIDI control change message though USB. Not used yet. 
void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}

void setup() {
  // put your setup code here, to run once:
  // Sets output pins and initilises them to LOW
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  digitalWrite(S0, 0);
  digitalWrite(S1, 0);
  digitalWrite(S2, 0);
  digitalWrite(S3, 0);
  
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  // Iterate over used/listed Piezos
  for(int i = 0; i < numInputs; i++)
  {
    updateMux(i);                               // Sends singals to mux to get the specified connected Piezo
    int piezoVelocity = piezo1[i]->velocity();  // Calls Piezo Class velocity function to read how hard Piezo is hit
    if(piezoVelocity == 510)                    // Value of 510 means that noteOff message should be sent
    {
/*      
      Serial.print(i);
      Serial.print(" ");
      Serial.println("off");
*/      
      noteOff(piezo1[i]->pChannel, piezo1[i]->pPitch, 64);  // Sends note off 
//      noteOn(piezo1[i]->pChannel, piezo1[i]->pPitch, 0); // Sets note velocity to 0. More commonly used(?) than note off. Pick either one but not both.
      MidiUSB.flush();
    }
    else if (piezoVelocity != 511)               // Any value other than 510 and 511 (hopefully) means that a hit was registered and mapped to a certain velocity
    {  
/*      
      Serial.print(i);
      Serial.print(" ");
      Serial.println(piezoVelocity);
*/
      noteOn(piezo1[i]->pChannel, piezo1[i]->pPitch, piezoVelocity);  // Sends note on message
//      Serial.println(analogRead(sig));
      MidiUSB.flush();
    }
  }
  
}
