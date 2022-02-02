#ifndef BtBase_h
#define BtBase_h

#include "Arduino.h"
#include "IMUSensor.h"


#include <ArduinoBLE.h>

#include "Compass.h"

class BtBase
{
  public:
    static BtBase* main;
    
    BtBase();
    void sendCmd(const char* command);
    void loop(void (*callback)());
    void sendIMU(IMUSensor imu, Compass c);
  private:  
    
    static void checkWritten(BLEDevice central, BLECharacteristic characteristic);
  
    BLEService gloveService; // create service 
    
    BLEFloatCharacteristic acc_x;
    BLEFloatCharacteristic acc_y;
    BLEFloatCharacteristic acc_z;
    
    BLEFloatCharacteristic gyr_x;
    BLEFloatCharacteristic gyr_y;
    BLEFloatCharacteristic gyr_z;
    
    BLEFloatCharacteristic mag_x;
    BLEFloatCharacteristic mag_y;
    BLEFloatCharacteristic mag_z;
    
    BLEStringCharacteristic cmd;
    
    BLEStringCharacteristic check;
    
    BLEFloatCharacteristic heading;
};

#endif
