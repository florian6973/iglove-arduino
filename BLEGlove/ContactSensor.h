#ifndef ContactSensor_h
#define ContactSensor_h

#include "Arduino.h"




class ContactSensor
{
  public:
    ContactSensor(int pin);
    int getEtat();
    int getDoigt();
  private:
    int _pin;
    int _type;
};

#endif
