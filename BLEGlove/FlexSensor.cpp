
#include "Arduino.h"
#include "FlexSensor.h"



FlexSensor::FlexSensor(int pin, int type)
{
  pinMode(pin, INPUT);
  _pin = pin;
  _type = type;
}

int FlexSensor::getError()

{
//Serial.print("pin");
//Serial.println(_pin);
int valeur = analogRead(_pin);
//Serial.println(valeur);
int error = 0; 
  //Serial.println(_pin);
  //Serial.println(valeur);

if (valeur <80) {
  

error = 1;
  }

else{
  error = 0;
  }

return error;
}



int FlexSensor::getEtat()
{
  int etat = 2;
int valeur = analogRead(_pin);
  //Serial.println(_pin);
  //Serial.println(valeur);


if (_type == INDEX){
  


if (210 < valeur) {
  etat = 1;
  }

else{
  etat = 0;
  }
}



else
{
  if (160 < valeur || valeur <80) {
  etat = 1;
  }
  
  else{
    etat = 0;
    }
    
}

  return etat;
}

bool FlexSensor::pointe(FlexSensor index, FlexSensor majeur, FlexSensor quatrieme)
{
 if (index.getError() == 1 || majeur.getError() == 1 || quatrieme.getError() == 1){
  Serial.println("il y a une erreur");
  return false;
 }

 else {
  bool pointage = (bool)index.getEtat() * (1-majeur.getEtat()) * (1-quatrieme.getEtat());
  return pointage;
  }
}
