#include <imuFilter.h>

#include <Arduino_LSM9DS1.h>
/*
  Arduino LSM9DS1 - Simple Accelerometer

  This example reads the acceleration values from the LSM9DS1
  sensor and continuously prints them to the Serial Monitor
  or Serial Plotter.

  The circuit:
  - Arduino Nano 33 BLE Sense

  created 10 Jul 2019
  by Riccardo Rizzo

  This example code is in the public domain.
*/

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Started");

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  Serial.print("Accelerometer sample rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.println(" Hz");
  Serial.println();
  Serial.println("Acceleration in G's");
  Serial.println("X\tY\tZ");

  
  Serial.print("Gyroscope sample rate = ");
  Serial.print(IMU.gyroscopeSampleRate());
  Serial.println(" Hz");
  Serial.println();
  Serial.println("Gyroscope in degrees/second");
  Serial.println("X\tY\tZ");

  
}

float toDeg(float x)
{
  return x*180/M_PI;
}

void loop() {
  float x, y, z;

  /*if (Serial.available() > 0)
  {
    while (Serial.available())
    {
      Serial.read();
    }
    IMU.end();
    IMU.begin();
  }*/

  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x, y, z);

    float theta = atan(x/sqrt(y*y+z*z));
    float psi = atan(y/sqrt(x*x+z*z));
    float phi = atan(sqrt(y*y+x*x)/z);

    Serial.print(x);
    Serial.print('\t');
    Serial.print(y);
    Serial.print('\t');
    Serial.println(z);
    
    Serial.print(toDeg(theta));
    Serial.print('\t');
    Serial.print(toDeg(psi));
    Serial.print('\t');
    Serial.println(toDeg(phi));

    float e1 = sin(phi)*sin(psi);
    float e2 = -sin(phi)*sin(theta);
    float e3 = cos(theta)*cos(phi)*cos(psi);
    Serial.print(e1);
    Serial.print('\t');
    Serial.print(e2);
    Serial.print('\t');
    Serial.println(e3);
    
  }

  if (IMU.gyroscopeAvailable()) {
      IMU.readGyroscope(x, y, z);
  
      Serial.print(x);
      Serial.print('\t');
      Serial.print(y);
      Serial.print('\t');
      Serial.println(z);
    }

  
    Serial.println("");
  delay(1000);
}
