#include "Arduino.h"
#include "ContactSensor.h"



ContactSensor::ContactSensor(int pin)
{
pinMode(pin,INPUT);
  _pin = pin;
 
}


 
int ContactSensor::getEtat()

{

int etat = 0;
int valeur = analogRead(_pin);
  
  if (valeur < 20){
    
    Serial.println("pas de contact");
    etat = 0;
    }
   else {

    Serial.println("il y a contact");
    etat = 1;
    }
 return etat;
}



int ContactSensor::getDoigt(){

 
int valeur = analogRead(_pin);
int doigt_connecte;
  
  if (valeur < 50){
    
    Serial.println("aucun doigt");
    doigt_connecte = 0;
    }
  if (valeur > 150 && valeur < 200){
    
    Serial.println("connectÃ© Ã  index");
    doigt_connecte = 1;
    }
  if (valeur > 600 && valeur < 700){
    
    Serial.println("connectÃ© Ã  majeur");
    doigt_connecte = 2;
    }
  if (valeur > 950){
    
    Serial.println("connectÃ© Ã  quatrieme");
    doigt_connecte = 3;
    }
   
  
  return doigt_connecte;
}
