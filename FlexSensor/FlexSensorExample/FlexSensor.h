#ifndef FlexSensor_h
#define FlexSensor_h

#include "Arduino.h"

#define INDEX 0
#define AUTREDOIGT 1

class FlexSensor
{
  public:
    FlexSensor(int pin, int type);
    int getEtat();
    int getError();
    static bool pointe(FlexSensor index, FlexSensor majeur, FlexSensor quatrieme);
  private:
    int _pin;
    int _type;
};

#endif
