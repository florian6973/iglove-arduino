#include "Arduino.h"
#include "BtBase.h"

BtBase* BtBase::main = nullptr;
    
BtBase::BtBase() : 
     acc_x(BLEFloatCharacteristic("a001",  // standard 16-bit characteristic UUID
        BLERead | BLENotify)),
     acc_y(BLEFloatCharacteristic("a002",  // standard 16-bit characteristic UUID
        BLERead | BLENotify)),
     acc_z(BLEFloatCharacteristic("a003",  // standard 16-bit characteristic UUID
        BLERead | BLENotify)),
     gyr_x(BLEFloatCharacteristic("b001",  // standard 16-bit characteristic UUID
        BLERead | BLENotify)),
     gyr_y(BLEFloatCharacteristic("b002",  // standard 16-bit characteristic UUID
        BLERead | BLENotify)),
     gyr_z(BLEFloatCharacteristic("b003",  // standard 16-bit characteristic UUID
        BLERead | BLENotify)),
     mag_x(BLEFloatCharacteristic("c001",  // standard 16-bit characteristic UUID
        BLERead | BLENotify)),
     mag_y(BLEFloatCharacteristic("c002",  // standard 16-bit characteristic UUID
        BLERead | BLENotify)),
     mag_z(BLEFloatCharacteristic("c003",  // standard 16-bit characteristic UUID
        BLERead | BLENotify)),        
     cmd( BLEStringCharacteristic("abcd",  // standard 16-bit characteristic UUID
        BLERead | BLEIndicate, 4)), 
     check(BLEStringCharacteristic("abce",  // standard 16-bit characteristic UUID
        BLERead | BLEWrite, 4))
{
  gloveService = BLEService("19B10010-E8F2-537E-4F6C-D104768A1214");
  
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");
    while (1);
  }
  
  BLE.setLocalName("iGlove");
  BLE.setDeviceName("iGlove-D");
  BLE.setAdvertisedService(gloveService); // add the service UUID 

  check.setEventHandler(BLEWritten, BtBase::checkWritten);
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

void BtBase::sendCmd(const char* command)
{  
  cmd.writeValue(command);
}

void BtBase::loop(void (*callback)())
{ 
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
      callback();
      BLE.poll();
    }

    // when the central disconnects, turn off the LED:
    digitalWrite(LED_BUILTIN, LOW);
    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
  }
}

void BtBase::checkWritten(BLEDevice central, BLECharacteristic characteristic) {
  // central wrote new value to characteristic, update LED
  Serial.print("Characteristic event, written: ");
  byte s = 0;
  main->check.readValue(s);
  Serial.println(s);
}

void BtBase::sendIMU(IMUSensor imu)
{
  float* data = imu.getInfos();
    acc_x.writeValue(data[0]);  
    acc_y.writeValue(data[1]);  
    acc_z.writeValue(data[2]);  
    
    gyr_x.writeValue(data[3]);  
    gyr_y.writeValue(data[4]);  
    gyr_z.writeValue(data[5]);  
     
    mag_x.writeValue(data[6]);  
    mag_y.writeValue(data[7]);  
    mag_z.writeValue(data[8]);     
}
