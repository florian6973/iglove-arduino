  /*****************************************************************
    Source : https://github.com/jremington/LSM9DS1-AHRS
    Modified from LSM9DS1_Basic_I2C.ino
    SFE_LSM9DS1 Library Simple Example Code - I2C Interface
    Jim Lindblom @ SparkFun Electronics
    Original Creation Date: April 30, 2015
    https://github.com/sparkfun/LSM9DS1_Breakout
    SFE example Modified to print the raw sensor data
    using default settings. SJR 2/2021
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
    This code is beerware. If you see me (or any other SparkFun
    employee) at the local, and you've found our code helpful,
    please buy us a round!
    Distributed as-is; no warranty is given.
  *****************************************************************/
  // The SFE_LSM9DS1 library requires both Wire and SPI be
  // included BEFORE including the 9DS1 library.
#include <Arduino_LSM9DS1.h>
  //////////////////////////
  // LSM9DS1 Library Init //
  //////////////////////////
  // default settings gyro  245 d/s, accel = 2g, mag = 4G
  
  ///////////////////////
  // Example I2C Setup //
  ///////////////////////
  // SDO_XM and SDO_G are both pulled high, so our addresses are:
  // #define LSM9DS1_M  0x1E // Would be 0x1C if SDO_M is LOW
  // #define LSM9DS1_AG 0x6B // Would be 0x6A if SDO_AG is LOW
  
  ////////////////////////////
  // Sketch Output Settings //
  ////////////////////////////
  
  #define PRINT_SPEED 300 // 300 ms between prints
  static unsigned long lastPrint = 0; // Keep track of print time
  
  void setup()
  {
    Serial.begin(9600);
    while (!Serial); //wait for connection
  
   
  while (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    delay (100);
  }
  
    Serial.println("Collecting gyro data, hold still");
    delay(3000);
    // get gyro offset
    long gxa = 0, gya = 0, gza = 0;
    for (int i = 0; i < 300; i++) {
      
        float gx, gy, gz;
        IMU.readGyroscope(gx, gy, gz);
        gxa += gx;
        gya += gy;
        gza += gz;
      
    }
    Serial.println(F("gyro offsets"));
    Serial.print(gxa / 300);
    Serial.print(", ");
    Serial.print(gya / 300);
    Serial.print(", ");
    Serial.println(gza / 300);
    Serial.println();
    
    Serial.println(F("rotate slowly and carefully in 3D"));
    delay(3000);
    Serial.println("starting");
  }
  
  void loop()
  {
    static int n = 0; //count values transmitted
    // Update the sensor values whenever new data is available

  float aax, aay, aaz, mmx, mmy, mmz;  
    IMU.readAcceleration(aax, aay, aaz);
    IMU.readMagneticField(mmx, mmy, mmz);
  
    if (millis() - lastPrint > PRINT_SPEED)
    {
      Serial.print(aax);
      Serial.print(", ");
      Serial.print(aay);
      Serial.print(", ");
      Serial.print(aaz);
      Serial.print(", ");
      Serial.print(mmx);
      Serial.print(", ");
      Serial.print(mmy);
      Serial.print(", ");
      Serial.println(mmz);
      if (n++ > 299) {
        Serial.println("Done.");
        while (1); //hang here
      }
      lastPrint = millis(); // Update lastPrint time
    }
  }
