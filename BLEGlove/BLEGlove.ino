#include <Arduino_LSM9DS1.h>
#include <ArduinoBLE.h>

//BLEService gloveService("1111");
BLEService gloveService("19B10010-E8F2-537E-4F6C-D104768A1214"); // create service 

BLEFloatCharacteristic acc_x("a001",  // standard 16-bit characteristic UUID
    BLERead | BLENotify);
BLEFloatCharacteristic acc_y("a002",  // standard 16-bit characteristic UUID
    BLERead | BLENotify);
BLEFloatCharacteristic acc_z("a003",  // standard 16-bit characteristic UUID
    BLERead | BLENotify);

BLEFloatCharacteristic gyr_x("b001",  // standard 16-bit characteristic UUID
    BLERead | BLENotify);
BLEFloatCharacteristic gyr_y("b002",  // standard 16-bit characteristic UUID
    BLERead | BLENotify);
BLEFloatCharacteristic gyr_z("b003",  // standard 16-bit characteristic UUID
    BLERead | BLENotify);

BLEFloatCharacteristic mag_x("c001",  // standard 16-bit characteristic UUID
    BLERead | BLENotify);
BLEFloatCharacteristic mag_y("c002",  // standard 16-bit characteristic UUID
    BLERead | BLENotify);
BLEFloatCharacteristic mag_z("c003",  // standard 16-bit characteristic UUID
    BLERead | BLENotify);

BLEStringCharacteristic cmd("abcd",  // standard 16-bit characteristic UUID
    BLERead | BLEIndicate, 4);

BLEStringCharacteristic check("abce",  // standard 16-bit characteristic UUID
    BLERead | BLEWrite, 4);

void initBluetooth()
{
  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");
    while (1);
  }
  
  BLE.setLocalName("iGlove");
  BLE.setDeviceName("iGlove-D");
  BLE.setAdvertisedService(gloveService); // add the service UUID 

  check.setEventHandler(BLEWritten, checkWritten);
  check.setValue("0");
  
  gloveService.addCharacteristic(acc_x);
  gloveService.addCharacteristic(acc_y);
  gloveService.addCharacteristic(acc_z);
  gloveService.addCharacteristic(gyr_x);
  gloveService.addCharacteristic(gyr_y);
  gloveService.addCharacteristic(gyr_z);
  gloveService.addCharacteristic(mag_x);
  gloveService.addCharacteristic(mag_y);
  gloveService.addCharacteristic(mag_z);
  gloveService.addCharacteristic(cmd);
  gloveService.addCharacteristic(check);
  
  BLE.addService(gloveService); // Add the service

  /* Start advertising BLE.  It will start continuously transmitting BLE
     advertising packets and will be visible to remote BLE central devices
     until it receives a new connection */

  // start advertising
  BLE.advertise();

  Serial.println("Bluetooth device active, waiting for connections...");
}

void initComponents()
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

void setup() {
  Serial.begin(9600);    // initialize serial communication
  while (!Serial);

  initComponents();
  initBluetooth();
}

void sendIMU() // only send when impulse
{  
  float x, y, z;
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x, y, z);  
    acc_x.writeValue(x);  
    acc_y.writeValue(y);  
    acc_z.writeValue(z);  
  }
  delay(50);
  
  float x2, y2, z2;
  if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(x2, y2, z2);   
    gyr_x.writeValue(x2);  
    gyr_y.writeValue(y2);  
    gyr_z.writeValue(z2);   
  }
  delay(50); // delay to avoid loss of data
  
  float x3, y3, z3;
  if (IMU.magneticFieldAvailable()) {
    IMU.readMagneticField(x3, y3, z3);  
    mag_x.writeValue(x3);  
    mag_y.writeValue(y3);  
    mag_z.writeValue(z3);     
  }
  delay(50);
  //cmd.writeValue("ok");
}

void sendInfos()
{
  Serial.println("Sending infos...");
  sendIMU();
  
}


void checkWritten(BLEDevice central, BLECharacteristic characteristic) {
  // central wrote new value to characteristic, update LED
  Serial.print("Characteristic event, written: ");
  byte s = 0;
  check.readValue(s);
  Serial.println(s);
}

void loopAction()
{ 
  if (Serial.available())
  {
    delay(100);
    sendInfos();
      while(Serial.available() > 0) {
        char t = Serial.read();
      }
      delay(50);
      cmd.writeValue("TEST");
      Serial.println("Test envoyé");
      delay(50);
      cmd.writeValue("end");
      delay(50);
  }
}

void loop() {  
  // wait for a BLE central
  BLEDevice central = BLE.central();

  // if a central is connected to the peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's BT address:
    Serial.println(central.address());
    // turn on the LED to indicate the connection:
    digitalWrite(LED_BUILTIN, HIGH);

    // check the battery level every 200ms
    // while the central is connected:
    while (central.connected()) {
      loopAction();
      BLE.poll();
    }

    // when the central disconnects, turn off the LED:
    digitalWrite(LED_BUILTIN, LOW);
    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
  }
}
