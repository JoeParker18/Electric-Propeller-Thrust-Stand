#include <HX711.h>
HX711 scale;

// ---- Diagnostic mode: true = print raw HX711 data, false = normal CSV for MATLAB ----
const bool DEBUG_RAW = true;

const float maxRPM = 18000;
const int DOUT_PIN = 3;            // HX711 DT  -> D3
const int SCK_PIN  = 2;            // HX711 SCK -> D2
const int MOTOR = 5;
const int RAMP_STEP = 10;

int Target_Motor_Output = 0;
int Current_Motor_Output = 0;

void setup() {
  pinMode(MOTOR, OUTPUT);
  analogWrite(MOTOR, 0);           // motor held off during startup/tare
  Serial.begin(115200);

  scale.begin(DOUT_PIN, SCK_PIN);
  delay(500);                      // let the HX711 wake up and settle before taring
  scale.set_scale(197.0);
  scale.tare();

  if (DEBUG_RAW) {
    Serial.print("Tare offset: ");
    Serial.println(scale.get_offset());
    Serial.println("raw,thrust,estRPM,time_ms");
  }
}

void loop() {
  // 1. Read a new command (only touches the target)
  if (Serial.available() > 0) {
    Target_Motor_Output = Serial.parseFloat();
    Target_Motor_Output = constrain(Target_Motor_Output, 0, 255);
    while (Serial.available() > 0) Serial.read();   // clear leftover \n / \r
  }

  // 2. Ramp current toward target
  if (Target_Motor_Output == 0) {
    Current_Motor_Output = 0;                        // typing 0 = immediate stop
  } else if (Current_Motor_Output < Target_Motor_Output) {
    Current_Motor_Output = min(Current_Motor_Output + RAMP_STEP, Target_Motor_Output);
  } else if (Current_Motor_Output > Target_Motor_Output) {
    Current_Motor_Output = max(Current_Motor_Output - RAMP_STEP, Target_Motor_Output);
  }

  // 3. Drive motor
  analogWrite(MOTOR, Current_Motor_Output);

  unsigned long timeStamp = millis();
  float estimatedRPM = (Current_Motor_Output / 255.0) * maxRPM;

  // 4. One HX711 read per loop, with a timeout so a dropout can't freeze the loop
  if (scale.wait_ready_timeout(200)) {
    long raw = scale.read();
    float thrust = (raw - scale.get_offset()) / scale.get_scale();

    if (DEBUG_RAW) {
      Serial.print(raw);
      Serial.print(",");
    }
    Serial.print(thrust);
  } else {
    if (DEBUG_RAW) Serial.print("NOT_READY,");
    Serial.print("NaN");
  }

  // 5. Log
  Serial.print(",");
  Serial.print(estimatedRPM);
  Serial.print(",");
  Serial.println(timeStamp);

  delay(100);
}