#ifndef FlexSensor_h
#define FlexSensor_h

#include "Arduino.h"

#define FS_GRAND 0
#define FS_PETIT 1

class FlexSensor
{
  public:
    FlexSensor(int pin, int type);
    int getEtat();
  private:
    int _pin;
    int _type;
};

#endif
