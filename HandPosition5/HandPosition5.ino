#include <Arduino_LSM9DS1.h>


#include <MadgwickAHRS.h>


// initialize a Madgwick filter:
Madgwick filter;

#define gscale 1;

void setup()
{
  Serial.begin(115200);
  // attempt to start the IMU:
  if (!IMU.begin())
  {
    Serial.println("Failed to initialize IMU");
    // stop here if you can't access the IMU:
    while (true);
  }
 
  //Accelerometer setup
  IMU.setAccelFS(2);
  IMU.setAccelODR(5);
  IMU.setAccelOffset(-0.019423, -0.012760, -0.007454);
  IMU.setAccelSlope (1.002830, 0.999749, 1.008102);
  //Gyrsoscope setup
  IMU.gyroUnit = DEGREEPERSECOND;
  IMU.setGyroFS(3);
  IMU.setGyroODR(5);
  IMU.setGyroOffset (-1.489929, -0.464600, 0.043884);
  IMU.setGyroSlope (1.166259, 1.144730, 1.149710);
  //Mangetometer setupde
  IMU.setMagnetFS(0);
  IMU.setMagnetODR(8);
  IMU.setMagnetOffset(-4.042358, 41.287231, -16.781616);
  IMU.setMagnetSlope (1.886174, 1.278351, 1.254920);

  //Serial.println("Gyro settting ");  
  //Serial.print("Gyroscope FS= ");    Serial.print(IMU.getGyroFS());
  //Serial.print("Gyroscope ODR=");  Serial.println(IMU.getGyroODR());
  //Serial.print("Gyro unit=");           Serial.println(IMU.gyroUnit);

  // The slowest ODR determines the sensor rate, Accel and Gyro share their ODR
  float sensorRate = min(IMU.getGyroODR(),IMU.getMagnetODR());
  // start the filter to run at the sample rate:
  filter.begin(sensorRate);
}

void loop()
{ 
  // values for acceleration & rotation:
  float xAcc, yAcc, zAcc;
  float xGyro, yGyro, zGyro;
  float xMag, yMag, zMag;
  static int count=0;  
  // values for orientation:
  float roll, pitch, heading;

  // check if the IMU is ready to read:
  if (IMU.accelerationAvailable() &&
      IMU.gyroscopeAvailable() && IMU.magneticFieldAvailable())
  {
    // read all 9 DOF of the IMU:
    IMU.readAcceleration(xAcc, yAcc, zAcc);
    IMU.readGyro(xGyro, yGyro, zGyro);
    IMU.readMagneticField(xMag, yMag, zMag);

    // update the filter, which computes orientation:
    // note X and Y are swapped, X is inverted
    filter.update( yGyro,xGyro, zGyro, yAcc, xAcc, zAcc, yMag, -xMag, zMag);

    // print the heading, pitch and roll
    roll = filter.getRoll();
    pitch = filter.getPitch();
    heading = filter.getYaw();
    count++;
    if (count > 20) 
    {  count = 0;
       //Serial.print("y");
       Serial.println(heading);
       //Serial.print("y,p");
       //Serial.print(pitch);
       //Serial.print("p,r");
       //Serial.print(roll);
       //Serial.println("r");
    }  
  }
  else
  {
    Serial.println("Can't read");
  }
  delay(1);
}
