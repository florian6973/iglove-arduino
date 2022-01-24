#include <imuFilter.h>

/*
  Arduino LSM9DS1 - Simple Magnetometer

  This example reads the magnetic field values from the LSM9DS1
  sensor and continuously prints them to the Serial Monitor
  or Serial Plotter.

  The circuit:
  - Arduino Nano 33 BLE Sense

  created 10 Jul 2019
  by Riccardo Rizzo

  This example code is in the public domain.
*/

#include <Arduino_LSM9DS1.h>

#define DECLINATION 1.38
constexpr float GAIN = 0.1;     // Fusion gain, value between 0 and 1 - Determines response of heading correction with respect to gravity.
imuFilter <&GAIN> fusion;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Started");

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
  Serial.print("Magnetic field sample rate = ");
  Serial.print(IMU.magneticFieldSampleRate());
  Serial.println(" uT");
  Serial.println();
  Serial.println("Magnetic Field in uT");
  Serial.println("X\tY\tZ");

  float axx, ayy, azz;
  
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(axx, ayy, azz);
  }
    fusion.setup( axx, ayy, azz);     
}

void printAttitude(float ax, float ay, float az, float mx, float my, float mz)
{
  float roll = atan2(ay, az);
  float pitch = atan2(-ax, sqrt(ay * ay + az * az));

  float heading;
  if (my == 0)
    heading = (mx < 0) ? PI : 0;
  else
    heading = atan2(mx, my);

  heading -= DECLINATION * PI / 180;

  if (heading > PI) heading -= (2 * PI);
  else if (heading < -PI) heading += (2 * PI);


  float u = cos(heading)*cos(pitch);
  float v = sin(heading)*cos(pitch);
  float w = sin(pitch);
  Serial.print(u, 2);
  Serial.print(",");
  Serial.print(v, 2);
  Serial.print(",");
  Serial.println(w, 2);

  // Convert everything from radians to degrees:
  heading *= 180.0 / PI;
  pitch *= 180.0 / PI;
  roll  *= 180.0 / PI;

  Serial.print("Pitch:");
  Serial.print(pitch, 2);
  Serial.print(",");
  Serial.print("Roll:");
  Serial.print(roll, 2);
  Serial.print(",");
  Serial.print("Heading:"); 
  Serial.println(heading, 2);

}

void loop() {
  float accRawX, accRawY, accRawZ;
  
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(accRawX, accRawY, accRawZ);
  }

  
  float magRawX, magRawY, magRawZ;

  if (IMU.magneticFieldAvailable()) {
    IMU.readMagneticField(magRawX, magRawY, magRawZ);
  }
  
  float gxx, gyy, gzz;

  if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(gxx, gyy, gzz);
  }


printAttitude(accRawX, accRawY, accRawZ, -magRawY, -magRawX, magRawZ);

    fusion.update( gxx, gyy, gzz, accRawX, accRawY, accRawZ );    

  // Display angles:
  float pitch = fusion.pitch();
  float heading = fusion.yaw();
  Serial.print( fusion.pitch() );
  Serial.print( " " );
  Serial.print( fusion.yaw() );
  Serial.print( " " );
  Serial.print( fusion.roll() );
  Serial.println();
  float u = cos(heading)*cos(pitch);
  float v = sin(heading)*cos(pitch);
  float w = sin(pitch);
  Serial.print("V2: ");
  Serial.print(u, 2);
  Serial.print(",");
  Serial.print(v, 2);
  Serial.print(",");
  Serial.println(w, 2);

  /*imu.ax, imu.ay, imu.az,
                  -imu.my, -imu.mx, imu.mz);*/

delay(2000);
}
//https://forum.arduino.cc/t/lsm9ds1-heading-values-are-affected-by-pitch-and-roll-after-calibration/693653/14
