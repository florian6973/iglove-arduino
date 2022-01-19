
#include "Arduino.h"
#include "FlexSensor.h"



FlexSensor::FlexSensor(int pin, int type)
{
  pinMode(pin, INPUT);
  _pin = pin;
  _type = type;
}

int FlexSensor::getEtat()
{
  int etat = 2;
int valeur = analogRead(_pin);
  //Serial.println(_pin);
  //Serial.println(valeur);

if (_type == INDEX){
  


if (240 < valeur) {
  etat = 1;
  }

else{
  etat = 0;
  }
}



else
{
  if (180 < valeur) {
  etat = 1;
  }

else{
  etat = 0;
  }
  
}

  return etat;
}

static bool FlexSensor::pointe(FlexSensor index, FlexSensor majeur)
{
  bool pointage = (bool)index.getEtat()*(1-majeur.getEtat());
  return pointage;
}
