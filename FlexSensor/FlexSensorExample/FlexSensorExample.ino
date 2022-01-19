#include "FlexSensor.h"

FlexSensor index(A2, INDEX);
FlexSensor majeur(A1, AUTREDOIGT);
//FlexSensor quatrieme(A0, AUTREDOIGT);


void setup() {
Serial.begin(9600);
}

void loop() {
Serial.println();
Serial.println("index");
Serial.println(index.getEtat());
Serial.println("majeur");
Serial.println(majeur.getEtat());
//Serial.println("quatrieme");
//Serial.println(quatrieme.getEtat());
Serial.println("pointage");
Serial.println(FlexSensor::pointe(index, majeur));
delay(3000);
}
