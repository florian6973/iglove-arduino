#ifndef IMUSensor_h
#define IMUSensor_h


#include <Arduino_LSM9DS1.h>

#include "Arduino.h"

class IMUSensor
{
  public:
    IMUSensor();
    float* getInfos();
  private:  
};


#endif
