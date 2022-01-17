
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
  int etat = 0;
int valeur = analogRead(_pin);
  //Serial.println(_pin);
  //Serial.println(valeur);

if (_type == FS_PETIT)
{
if (valeur < 140) {
  etat = 4;
  }
  
if (140 < valeur && valeur < 180) {
  etat = 3;
  }
  
if (180 < valeur && valeur < 220) {
  etat = 2;
  }

if (220 < valeur) {
  etat = 1;
  }
}
else
{
  if (valeur < 100) {
  etat = 1;
  }
  
if (100 < valeur && valeur < 200) {
  etat = 2;
  }
  
if (200 < valeur && valeur < 300) {
  etat = 3;
  }

if (300 < valeur) {
  etat = 4;
  }
}

  return etat;
}
