
#include "Compass.h"

 float A_B[3]
 {   -0.04,    0.04,   -0.04};

 float A_Ainv[3][3]
  {{  1.05348,  0.01374, -0.03488},
  {  0.01374,  1.01381,  0.07739},
  { -0.03488,  0.07739,  0.98568}};

 float M_B[3]
 {   23.04,   15.41,   -6.91};

 float M_Ainv[3][3]
  {{  0.95170,  0.02063, -0.00337},
  {  0.02063,  0.94454,  0.00845},
  { -0.00337,  0.00845,  0.98290}};

// local magnetic declination in degrees
float declination = 1.35;

float p[] = {0, 1, 0};  //Y marking on sensor board points toward yaw = 0


// Returns a heading (in degrees) given an acceleration vector a due to gravity, a magnetic vector m, and a facing vector p.
int Compass::get_heading(float acc[3], float mag[3], float p[3])
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

void Compass::get_scaled_IMU(float Axyz[3], float Mxyz[3]) {
  float aax, aay, aaz, mmx, mmy, mmz;  
    IMU.readAcceleration(aax, aay, aaz); // TODO adapt with IMUSensor
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
void Compass::vector_cross(float a[3], float b[3], float out[3])
{
  out[0] = a[1] * b[2] - a[2] * b[1];
  out[1] = a[2] * b[0] - a[0] * b[2];
  out[2] = a[0] * b[1] - a[1] * b[0];
}

float Compass::vector_dot(float a[3], float b[3])
{
  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

void Compass::vector_normalize(float a[3])
{
  float mag = sqrt(vector_dot(a, a));
  a[0] /= mag;
  a[1] /= mag;
  a[2] /= mag;
}

Compass::Compass()
{}

float Compass::getHeading()
{
  
    float Axyz[3], Mxyz[3];
    get_scaled_IMU(Axyz, Mxyz);    
    Axyz[0] = -Axyz[0];
    float heading = get_heading(Axyz, Mxyz, p);
    float pitch = atan2(-Axyz[0], sqrt(Axyz[1] * Axyz[1] + Axyz[2] * Axyz[2]));
    float roll = atan2(Axyz[1], sqrt(Axyz[0] * Axyz[0] + Axyz[2] * Axyz[2]));
    heading = heading * M_PI / 180; // to radians
    float u = cos(heading)*cos(pitch);
    float v = sin(heading)*cos(pitch);
    float w = sin(pitch);
    return heading;
}
