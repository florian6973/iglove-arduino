#include "FlexSensor.h"

FlexSensor pouce(A2, FS_PETIT);
FlexSensor index(A1, FS_GRAND);
FlexSensor majeur(A0, FS_PETIT);

void setup() {
Serial.begin(9600);
}

void loop() {
Serial.println();
Serial.println("pouce");
Serial.println(pouce.getEtat());
Serial.println("index");
Serial.println(index.getEtat());
Serial.println("majeur");
Serial.println(majeur.getEtat());
delay(3000);
}
