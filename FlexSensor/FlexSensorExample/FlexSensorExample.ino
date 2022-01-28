#include "FlexSensor.h"

FlexSensor index0(A0, INDEX);
FlexSensor majeur(A1, AUTREDOIGT);
FlexSensor quatrieme(A2, AUTREDOIGT);


void setup() {
Serial.begin(9600);
}

void loop() {
Serial.println();
Serial.println("index");
Serial.println(index0.getEtat());
Serial.println("majeur");
Serial.println(majeur.getEtat());
Serial.println("quatrieme");
Serial.println(quatrieme.getEtat());
Serial.println("pointage");
Serial.println(FlexSensor::pointe(index0, majeur, quatrieme));
delay(3000);
}
