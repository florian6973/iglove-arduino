//
// Tilt compensated compass  S.J. Remington 2/2021
// Requires the Sparkfun LSM9DS1 library

// Both the accelerometer and magnetometer MUST be properly calibrated for this program to work.
// Follow the procedure described in http://sailboatinstruments.blogspot.com/2011/08/improved-magnetometer-calibration.html
// or in more detail, the tutorial https://thecavepearlproject.org/2015/05/22/calibrating-any-compass-or-accelerometer-for-arduino/
//
// To collect data for calibration, use the companion program LSM9DS1_cal_data
//
/*
  Adafruit 3V or 5V board
  Hardware setup: This library supports communicating with the
  LSM9DS1 over either I2C or SPI. This example demonstrates how
  to use I2C. The pin-out is as follows:
  LSM9DS1 --------- Arduino
   SCL ---------- SCL (A5 on older 'Duinos')
   SDA ---------- SDA (A4 on older 'Duinos')
   VIN ------------- 5V
   GND ------------- GND
   CSG, CSXM, SDOG, and SDOXM should all be pulled high.
   pullups on the ADAFRUIT breakout board do this.
*/
// The SFE_LSM9DS1 library requires both Wire and SPI be
// included BEFORE including the 9DS1 library.
#include <Arduino_LSM9DS1.h>

//////////////////////////
// LSM9DS1 Library Init //
//////////////////////////
// default settings gyro  245 d/s, accel = 2g, mag = 4G

// SDO_XM and SDO_G are both pulled high, so our addresses are:
#define LSM9DS1_M  0x1E // Would be 0x1C if SDO_M is LOW
#define LSM9DS1_AG 0x6B // Would be 0x6A if SDO_AG is LOW

// VERY IMPORTANT!
//These are the previously determined offsets and scale factors for accelerometer and magnetometer, using MPU9250_cal and Magneto
//The compass will NOT work well or at all if these are not correct

//Accel scale 16457.0 to normalize
 float A_B[3]
 {   -0.06,    0.02,   -0.02};

 float A_Ainv[3][3]
  {{  1.04475, -0.00328, -0.04027},
  { -0.00328,  0.97819, -0.01222},
  { -0.04027, -0.01222,  1.00217}};

 float M_B[3]
 {   19.11,   23.44,   -9.72};

 float M_Ainv[3][3]
  {{  1.22688,  0.05174,  0.09785},
  {  0.05174,  1.18980, -0.04532},
  {  0.09785, -0.04532,  1.25663}};

// local magnetic declination in degrees
float declination = 1.35;

/*
  This tilt-compensated code assumes that the Adafruit LSM9DS1 sensor board is oriented with Y pointing
  to the North, X pointing West, and Z pointing up.
  The code compensates for tilts of up to 90 degrees away from horizontal.
  Facing vector p is the direction of travel and allows reassigning these directions.
  It should be defined as pointing forward,
  parallel to the ground, with coordinates {X, Y, Z} (in magnetometer frame of reference).
*/
float p[] = {0, 1, 0};  //Y marking on sensor board points toward yaw = 0

#define PRINT_SPEED 2000 // ms between prints
static unsigned long lastPrint = 0; // Keep track of print time


void setup()
{
  Serial.begin(9600);
  while (!Serial); //wait for connection

  while (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    delay (100);
  }
  
  /*Serial.println("Initializing the LSM9DS1");
  uint16_t status = initLSM9DS1();
  Serial.print("LSM9DS1 WHO_AM_I's returned: 0x");
  Serial.println(status, HEX);
  Serial.println("Should be 0x683D");
  Serial.println();*/

  /*if (imu.begin() == false) // with no arguments, this uses default addresses (AG:0x6B, M:0x1E) and i2c port (Wire).
  {
    Serial.println(F("LSM9DS1 not detected"));
    while (1);
  }*/
  delay(500);
}

void loop()
{
  float Axyz[3], Mxyz[3]; //centered and scaled accel/mag data

  // Update the sensor values whenever new data is available
  //if ( IMU.accelerationAvailable() ) imu.readAccel();
  //if ( IMU.magneticFieldAvailable() )   imu.readMag();

  /*if (millis() - lastPrint > PRINT_SPEED)
  {
    get_scaled_IMU(Axyz, Mxyz);

    // correct accelerometer handedness
    // Note: the illustration in the LSM9DS1 data sheet implies that the magnetometer 
    // X and Y axes are rotated with respect to the accel/gyro X and Y, but this is not case.
    
    Axyz[0] = -Axyz[0]; //fix accel handedness
    
    /*Serial.print(Axyz[0]);
    Serial.print(", ");
    Serial.print(Axyz[1]);
    Serial.print(", ");
    Serial.print(Axyz[2]);
    Serial.print(", ");
    Serial.print(Mxyz[0]);
    Serial.print(", ");
    Serial.print(Mxyz[1]);
    Serial.print(", ");
    Serial.println(Mxyz[2]);*//*
    //  get heading in degrees
    Serial.print("Heading: ");
    Serial.println(get_heading(Axyz, Mxyz, p));
    lastPrint = millis(); // Update lastPrint time
  }*/

  if (Serial.available() > 0)
  {
    while (Serial.available())
    {
    Serial.read();
    }
    get_scaled_IMU(Axyz, Mxyz);    
    Axyz[0] = -Axyz[0];
    float heading = get_heading(Axyz, Mxyz, p);
    float pitch = atan2(-Axyz[0], sqrt(Axyz[1] * Axyz[1] + Axyz[2] * Axyz[2]));
    float roll = atan2(Axyz[1], sqrt(Axyz[0] * Axyz[0] + Axyz[2] * Axyz[2]));

    Serial.println(heading);
    Serial.println(pitch);
    Serial.println(roll);
    heading = heading * M_PI / 180; // to radians
    float u = cos(heading)*cos(pitch);
    float v = sin(heading)*cos(pitch);
    float w = sin(pitch);
    Serial.print("V2: ");
    Serial.print(u, 2);
    Serial.print(",");
    Serial.print(v, 2);
    Serial.print(",");
    Serial.println(w, 2);
  }
  delay(100);
  // consider averaging a few headings for better results
}

// Returns a heading (in degrees) given an acceleration vector a due to gravity, a magnetic vector m, and a facing vector p.
int get_heading(float acc[3], float mag[3], float p[3])
{
  float W[3], N[3]; //derived direction vectors

  // cross "Up" (acceleration vector, g) with magnetic vector (magnetic north + inclination) with  to produce "West"
  vector_cross(acc, mag, W);
  vector_normalize(W);

  // cross "West" with "Up" to produce "North" (parallel to the ground)
  vector_cross(W, acc, N);
  vector_normalize(N);

  // compute heading in horizontal plane, correct for local magnetic declination
  
  int heading = round(atan2(vector_dot(W, p), vector_dot(N, p)) * 180 / M_PI + declination);
  heading = -heading; //conventional nav, heading increases North to East
  heading = (heading + 720)%360; //apply compass wrap
  return heading;
}

// subtract offsets and correction matrix to accel and mag data

void get_scaled_IMU(float Axyz[3], float Mxyz[3]) {
  float aax, aay, aaz, mmx, mmy, mmz;  
    IMU.readAcceleration(aax, aay, aaz);
    IMU.readMagneticField(mmx, mmy, mmz);
  
  byte i;
  float temp[3];
    Axyz[0] = aax;
    Axyz[1] = aay;
    Axyz[2] = aaz;
    Mxyz[0] = mmx;
    Mxyz[1] = mmy;
    Mxyz[2] = mmz;
  //apply offsets (bias) and scale factors from Magneto
  for (i = 0; i < 3; i++) temp[i] = (Axyz[i] - A_B[i]);
  Axyz[0] = A_Ainv[0][0] * temp[0] + A_Ainv[0][1] * temp[1] + A_Ainv[0][2] * temp[2];
  Axyz[1] = A_Ainv[1][0] * temp[0] + A_Ainv[1][1] * temp[1] + A_Ainv[1][2] * temp[2];
  Axyz[2] = A_Ainv[2][0] * temp[0] + A_Ainv[2][1] * temp[1] + A_Ainv[2][2] * temp[2];
  vector_normalize(Axyz);

  //apply offsets (bias) and scale factors from Magneto
  for (int i = 0; i < 3; i++) temp[i] = (Mxyz[i] - M_B[i]);
  Mxyz[0] = M_Ainv[0][0] * temp[0] + M_Ainv[0][1] * temp[1] + M_Ainv[0][2] * temp[2];
  Mxyz[1] = M_Ainv[1][0] * temp[0] + M_Ainv[1][1] * temp[1] + M_Ainv[1][2] * temp[2];
  Mxyz[2] = M_Ainv[2][0] * temp[0] + M_Ainv[2][1] * temp[1] + M_Ainv[2][2] * temp[2];
  vector_normalize(Mxyz);
}

// basic vector operations
void vector_cross(float a[3], float b[3], float out[3])
{
  out[0] = a[1] * b[2] - a[2] * b[1];
  out[1] = a[2] * b[0] - a[0] * b[2];
  out[2] = a[0] * b[1] - a[1] * b[0];
}

float vector_dot(float a[3], float b[3])
{
  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

void vector_normalize(float a[3])
{
  float mag = sqrt(vector_dot(a, a));
  a[0] /= mag;
  a[1] /= mag;
  a[2] /= mag;
}
