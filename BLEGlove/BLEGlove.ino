#include <Arduino_LSM9DS1.h>
#include <ArduinoBLE.h>

#include <Arduino_APDS9960.h>

#include "ContactSensor.h"

#include "FlexSensor.h"

FlexSensor index0(A0, INDEX);
FlexSensor majeur(A1, AUTREDOIGT);
FlexSensor quatrieme(A2, AUTREDOIGT);

ContactSensor mesure(A3);

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

void initProxSensor()
{
  //in-built LED
  pinMode(LED_BUILTIN, OUTPUT);
  //Red
  pinMode(LEDR, OUTPUT);
  //Green
  pinMode(LEDG, OUTPUT);
  //Blue
  pinMode(LEDB, OUTPUT);
  while (!Serial);
  if (!APDS.begin()) {
    Serial.println("Error initializing APDS9960 sensor!");
  }
  // for setGestureSensitivity(..) a value between 1 and 100 is required.
  // Higher values makes the gesture recognition more sensible but less accurate
  // (a wrong gesture may be detected). Lower values makes the gesture recognition
  // more accurate but less sensible (some gestures may be missed).
  // Default is 80
  //APDS.setGestureSensitivity(80);
  Serial.println("Detecting gestures ...");
  // Turining OFF the RGB LEDs
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDB, HIGH);
}

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
  initProxSensor();
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

   if (APDS.gestureAvailable()) {
    // a gesture was detected, read and print to serial monitor
    int gesture = APDS.readGesture();
    switch (gesture) {
      case GESTURE_UP:
        Serial.println("Detected UP gesture");
        digitalWrite(LEDR, LOW);
        delay(1000);
        digitalWrite(LEDR, HIGH);
        break;
      case GESTURE_DOWN:
        Serial.println("Detected DOWN gesture");
        digitalWrite(LEDG, LOW);
        delay(1000);
        digitalWrite(LEDG, HIGH);
        break;
      case GESTURE_LEFT:
        Serial.println("Detected LEFT gesture");
        digitalWrite(LEDB, LOW);
        delay(1000);
        digitalWrite(LEDB, HIGH);
        break;
      case GESTURE_RIGHT:
        Serial.println("Detected RIGHT gesture");
        digitalWrite(LED_BUILTIN, HIGH);
        delay(1000);
        digitalWrite(LED_BUILTIN, LOW);
        break;
      default:
        Serial.println("No gesture");
        break;
    }
  }

  if (mesure.getEtat() == 1)
  {
  Serial.println(mesure.getEtat());
  Serial.println(mesure.getDoigt());

  Serial.println();
Serial.println("index");
Serial.println(index0.getEtat());
Serial.println("majeur");
Serial.println(majeur.getEtat());
Serial.println("quatrieme");
Serial.println(quatrieme.getEtat());
Serial.println("pointage");
Serial.println(FlexSensor::pointe(index0, majeur, quatrieme));

      cmd.writeValue("TEST");
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
