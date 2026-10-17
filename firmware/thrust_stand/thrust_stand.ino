#include <HX711.h>
HX711 scale;
const float maxRPM = 18000;
float pwmValue = 0;
const int DOUT_PIN = 2;
const int SCK_PIN = 3;
const int MOTOR = 5;

void setup() {
  pinMode(MOTOR, OUTPUT);
  Serial.begin(115200);
  scale.begin(DOUT_PIN, SCK_PIN);
  // scale.tare();   // DIAGNOSTIC: disabled so we see the raw value
}

void loop() {
  if (Serial.available() > 0) {
    pwmValue = Serial.parseFloat();
  }
  analogWrite(MOTOR, pwmValue);

  // --- DIAGNOSTIC OUTPUT ---
  bool ready = scale.is_ready();   // 1 = HX711 says a reading is waiting
  long raw = scale.read();         // raw 24-bit value, no tare, no scaling
  int voltsCounts = analogRead(A0); // jumper from HX711 VCC (or E+) to A0

  Serial.print(raw);
  Serial.print(",");
  Serial.print(voltsCounts);
  Serial.print(",");
  Serial.print(ready);
  Serial.println();

  delay(100);
}
