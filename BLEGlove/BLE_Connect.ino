#include "ContactSensor.h"
#include "ProxSensor.h"
#include "FlexSensor.h"
#include "IMUSensor.h"
#include "BtBase.h"

FlexSensor index0(A0, INDEX);
FlexSensor majeur(A1, AUTREDOIGT);
FlexSensor quatrieme(A2, AUTREDOIGT);
ContactSensor mesure(A3);
ProxSensor ps = ProxSensor();
IMUSensor imu = IMUSensor();

void setup() {
  Serial.begin(9600);    // initialize serial communication
  while (!Serial);
  BtBase::main = new BtBase();
}

void loopAction()
{ 
  if (Serial.available())
  {
    delay(100);
    BtBase::main->sendIMU(imu);
      while(Serial.available() > 0) {
        Serial.read();
      }
      delay(50);
      BtBase::main->sendCmd("TEST");
      Serial.println("Test envoyé");
      delay(50);
      BtBase::main->sendCmd("end");
      delay(50);
  }

  ps.update();

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
    BtBase::main->sendCmd("TEST");
  }

}

void loop() {  
  BtBase::main->loop(&loopAction);
}
