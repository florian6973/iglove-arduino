
#include "IMUSensor.h"

IMUSensor::IMUSensor()
{
  pinMode(LED_BUILTIN, OUTPUT); // initialize the built-in LED pin to indicate when a central is connected

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  Serial.print("Accelerometer sample rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.println("Hz");
  Serial.print("Gyroscope sample rate = ");
  Serial.print(IMU.gyroscopeSampleRate());
  Serial.println("Hz");
  Serial.print("Magnetic field sample rate = ");
  Serial.print(IMU.magneticFieldSampleRate());
  Serial.println("Hz");
}

float* IMUSensor::getInfos() // only send when impulse
{  
  float data[9];
  
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(data[0], data[1], data[2]);  
  }
  delay(50);
  
  if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(data[3], data[4], data[5]);   
  }
  delay(50); // delay to avoid loss of data
  
  if (IMU.magneticFieldAvailable()) {
    IMU.readMagneticField(data[6], data[7], data[8]);  
  }
  delay(50);
  //cmd.writeValue("ok");

  return data;
}
