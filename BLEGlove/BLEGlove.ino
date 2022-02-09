#include "ContactSensor.h"
#include "ProxSensor.h"
#include "FlexSensor.h"
#include "IMUSensor.h"
#include "BtBase.h"
#include "Compass.h"

FlexSensor index0(A0, INDEX);
FlexSensor majeur(A1, AUTREDOIGT);
FlexSensor quatrieme(A2, AUTREDOIGT);
ContactSensor mesure(A3);
ProxSensor* ps = nullptr;
IMUSensor* imu = nullptr;
Compass* c = nullptr;

void setup() {
  Serial.begin(9600);    // initialize serial communication
  while (!Serial){
    Serial.begin(9600);
    delay(1000);
  }
  //ps = ProxSensor();
  imu = new IMUSensor();
  c = new Compass();
  ps = new ProxSensor();
  BtBase::main = new BtBase();
}

void loopAction()
{ 
  if (Serial.available())
  {
    delay(100);
    BtBase::main->sendIMU(*imu, *c);
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

  ps->update();

  if (FlexSensor::pointe(index0, majeur, quatrieme))
  {
    Serial.println("Pointage");
    BtBase::main->sendCmd("PNTE");
    
    delay(1000);
  }

  if (mesure.getEtat() == 1)
  {
  Serial.println(mesure.getEtat());
  Serial.println(mesure.getDoigt());
    if (mesure.getDoigt() == 3)
    {
    BtBase::main->sendCmd("POSI");
    }
    else if (mesure.getDoigt() == 2)
    {
    BtBase::main->sendCmd("LMPE");
    }
    else if (mesure.getDoigt() == 1)
    {
    BtBase::main->sendIMU(*imu, *c);
    BtBase::main->sendCmd("PNTE");
    }

  Serial.println();
Serial.println("index");
Serial.println(index0.getEtat());
Serial.println("majeur");
Serial.println(majeur.getEtat());
Serial.println("quatrieme");
Serial.println(quatrieme.getEtat());
Serial.println("pointage");
Serial.println(FlexSensor::pointe(index0, majeur, quatrieme));
    delay(1000);
  }

}

void loop() {  
  BtBase::main->loop(&loopAction);
Serial.println("index");
Serial.println(index0.getEtat());
Serial.println("majeur");
Serial.println(majeur.getEtat());
Serial.println("quatrieme");
Serial.println(quatrieme.getEtat());
Serial.println("pointage");
Serial.println(FlexSensor::pointe(index0, majeur, quatrieme));
  Serial.println(mesure.getEtat());
  Serial.println(mesure.getDoigt());
delay(1000);
}
