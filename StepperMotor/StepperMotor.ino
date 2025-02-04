#include<Stepper.h>

#define STEP_STEP_SIZE  8   // yellow
#define       STEP_MS1  4   // white, digital
#define       STEP_MS2  5   // light orange, PWM
#define       STEP_DIR  9   // grey, PWM
#define    STEP_ENABLE 10   // Red, PWM

void setup() {
  pinMode(STEP_STEP_SIZE, OUTPUT);
  pinMode(      STEP_MS1, OUTPUT);
  pinMode(      STEP_MS2, OUTPUT);
  pinMode(      STEP_DIR, OUTPUT);
  pinMode(   STEP_ENABLE, OUTPUT);
}

void loop() {
  digitalWrite(STEP_STEP_SIZE, LOW);
  digitalWrite(L2, LOW);
  digitalWrite(L2, LOW);
  digitalWrite(L2, LOW);
  analogWrite(MOTOR_ENABLE, 175);

}
