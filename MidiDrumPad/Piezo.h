/*
Pads: 
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
*/
#ifndef Piezo_h
#define Piezo_h

#include "Arduino.h"

class Piezo
{
  public:
    Piezo(int sigPin, byte pitch, byte channel, int thresh, unsigned long tDelay, int maxValue);
    int velocity();
    byte pPitch;
    byte pChannel;
  private:
    int _pin;
    int _peak;
    unsigned long _tPeak;
    int _thresh;
    unsigned long _tDelay;
    int _maxValue;
    bool _onFlag;
};

#endif
