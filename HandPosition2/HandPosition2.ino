/*
    Arduino and ADXL345 Accelerometer - 3D Visualization Example 
     by Dejan, https://howtomechatronics.com
*/
#include <Arduino_LSM9DS1.h>

float X_out, Y_out, Z_out;  // Outputs
float roll,pitch,rollF,pitchF=0;

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

  Serial.print("Magnetic field sample rate = ");
  Serial.print(IMU.magneticFieldSampleRate());
  Serial.println(" Hz");
  Serial.println();
  Serial.println("Magnetic Field in uT");
  Serial.println("X\tY\tZ");
}

void loop() {
  // === Read acceleromter data === //
  float X_out, Y_out, Z_out;

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
    IMU.readAcceleration(X_out, Y_out, Z_out);
    
  }
  float x, y, z;

  if (IMU.magneticFieldAvailable()) {
    IMU.readMagneticField(x, y, z);

    Serial.print(x);
    Serial.print('\t');
    Serial.print(y);
    Serial.print('\t');
    Serial.println(z);
  }

  // Calculate Roll and Pitch (rotation around X-axis, rotation around Y-axis)
  roll = atan(Y_out / sqrt(pow(X_out, 2) + pow(Z_out, 2))) * 180 / PI;
  pitch = atan(-1 * X_out / sqrt(pow(Y_out, 2) + pow(Z_out, 2))) * 180 / PI;

  // Low-pass filter
  rollF = 0.94 * rollF + 0.06 * roll;
  pitchF = 0.94 * pitchF + 0.06 * pitch;

  Serial.print(rollF);
  Serial.print("/");
  Serial.println(pitchF);


  String cardinal = "";
  
  float headingRadians = atan2(y, x);
  float headingDegrees = headingRadians * 180 / PI;
  float declinationAngle = 11.41666666666667;

  headingDegrees += declinationAngle;

  if (headingDegrees < 0) {
    headingDegrees += 360;
  }

  if (headingDegrees > 348.75 || headingDegrees < 11.25) {
    cardinal = " N";
  }
  else if (headingDegrees > 11.25 && headingDegrees < 33.75) {
    cardinal = " NNE";
  }
  else if (headingDegrees > 33.75 && headingDegrees < 56.25) {
    cardinal = " NE";
  }
  else if (headingDegrees > 56.25 && headingDegrees < 78.75) {
    cardinal = " ENE";
  }
  else if (headingDegrees > 78.75 && headingDegrees < 101.25) {
    cardinal = " E";
  }
  else if (headingDegrees > 101.25 && headingDegrees < 123.75) {
    cardinal = " ESE";
  }
  else if (headingDegrees > 123.75 && headingDegrees < 146.25) {
    cardinal = " SE";
  }
  else if (headingDegrees > 146.25 && headingDegrees < 168.75) {
    cardinal = " SSE";
  }
  else if (headingDegrees > 168.75 && headingDegrees < 191.25) {
    cardinal = " S";
  }
  else if (headingDegrees > 191.25 && headingDegrees < 213.75) {
    cardinal = " SSW";
  }
  else if (headingDegrees > 213.75 && headingDegrees < 236.25) {
    cardinal = " SW";
  }
  else if (headingDegrees > 236.25 && headingDegrees < 258.75) {
    cardinal = " WSW";
  }
  else if (headingDegrees > 258.75 && headingDegrees < 281.25) {
    cardinal = " W";
  }
  else if (headingDegrees > 281.25 && headingDegrees < 303.75) {
    cardinal = " WNW";
  }
  else if (headingDegrees > 303.75 && headingDegrees < 326.25) {
    cardinal = " NW";
  }
  else if (headingDegrees > 326.25 && headingDegrees < 348.75) {
    cardinal = " NNW";
  }

  Serial.print("Heading: ");
  Serial.print(headingDegrees);
  Serial.println(cardinal);

  delay(1000);
}
