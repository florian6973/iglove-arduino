#ifndef IMUSensor_h
#define IMUSensor_h


#include <Arduino_LSM9DS1.h>


class IMUSensor
{
  public:
    IMUSensor();
    float* getInfos();
  private:  
};


#endif
