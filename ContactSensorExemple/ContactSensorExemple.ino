#include "ContactSensor.h"

ContactSensor mesure(A3);


void setup() {
Serial.begin(9600);
}

void loop() {
Serial.println();

Serial.println(mesure.getEtat());
Serial.println(mesure.getDoigt());

delay(3000);
}
