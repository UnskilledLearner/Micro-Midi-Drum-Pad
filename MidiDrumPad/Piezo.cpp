#include "Piezo.h"
#include "Arduino.h"

Piezo::Piezo(int sigPin, byte pitch, byte channel, int thresh, unsigned long tDelay, int maxValue)
{
  _pin = sigPin;
  pPitch = pitch;
  pChannel = channel;
  _thresh = thresh;
  _tDelay = tDelay;
  _tPeak = millis();
  _peak = 0;
  _maxValue = maxValue;
  _onFlag = 0;
}

/*  Reads value of signal pin and then checks if above user defined threshold.
 *  If above threshold: check if user defined time has elapsed since last sent message 
 *    If so, check if current read value is higher than previous peak value
 *      If so, update peak value to new value
 *      Else, peak has been reached, return velocity within valid range (0 to 127)
 *  Else, check if note is on
 *    If note is on, turn note flag to off and return 510 to signal program to send note off message
 *  A Return 511 is to signal program that no new change has occurred 
 */
int Piezo::velocity()
{
  int pValue = analogRead(_pin);
  if(pValue > _thresh)
  {
    unsigned long tTemp = millis();
    if((tTemp - _tPeak > _tDelay) {
      if(pValue > _peak) {
        _peak = pValue;  
      } else {
        _onFlag = 1;
          if(_peak > _maxValue) {
            _tPeak = millis();
            _peak = 0;
            return 127;
          }
        int pV = map(_peak, _thresh, _maxValue, 5, 127);
        _tPeak = millis();
        _peak = 0;
        return pV;
      }
    } 
  }
  else if(_onFlag)
  {
      _onFlag = 0;
      return 510;
  }
  return 511;
}
