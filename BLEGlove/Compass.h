#ifndef Compass_h
#define Compass_h

#include "Arduino.h"



#include <Arduino_LSM9DS1.h>

class Compass
{
  public:
    Compass();
    float getHeading();
  private:
    int get_heading(float acc[3], float mag[3], float p[3]);
    void get_scaled_IMU(float Axyz[3], float Mxyz[3]);
    void vector_cross(float a[3], float b[3], float out[3]);
    float vector_dot(float a[3], float b[3]);
    void vector_normalize(float a[3]);

};

#endif
