#include <Servo.h>

#define       POT A0
#define     LIGHT A1
#define        IR A2
#define        US A3
#define       BTN  3
#define     DC_L1 12
#define     DC_L2 13
#define DC_ENABLE  6
#define   DC_ENC1  0
#define   DC_ENC2  1
#define     SERVO 11

int state = 0;

volatile bool btn0IsPressed = false;
unsigned long debounce0 = 0;

Servo Servo1;

void isr0() {
  btn0IsPressed = true;
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(POT, INPUT);
  pinMode(LIGHT, INPUT);
  pinMode(IR, INPUT);
  pinMode(US, INPUT);
  pinMode(BTN, INPUT);
    
  pinMode(       DC_L1, OUTPUT);
  pinMode(       DC_L2, OUTPUT);
  pinMode(   DC_ENABLE, OUTPUT);
  pinMode(     DC_ENC1, OUTPUT);
  pinMode(     DC_ENC2, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(BTN), isr0, RISING);

  Servo1.attach(SERVO);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (CheckForButton0Press()) {
    SwitchState();
  }

  if (state == 0) {
    PotServo();
  }
  else if (state == 1) {
    ReadIR();
  } 
  else if (state == 2) {
    ReadLight();
  }
  else if (state == 3) {
    ReadUS();
  }
}

bool CheckForButton0Press() {
  if (btn0IsPressed) {
    if (debounce0 == 0) {
      debounce0 = millis();
      return false;
    } else {
      return millis() - debounce0 > 100UL;
    }
  } else {
    return false;
  }
}

void SwitchState() {
  btn0IsPressed = false;
  debounce0 = 0;
  state += 1;
  state = state % 4;
  Serial.print("Switching to state ");
  Serial.println(state);
}

void PotServo() {
  int val = analogRead(POT);
  int D = map(val, 0, 1023, 5, 175);
  Serial.println(D);

  Servo1.write(D);
}

void ReadLight() {
  int val = analogRead(LIGHT);
  Serial.println(val);
}

void ReadIR() {
  int val = analogRead(IR);
  Serial.println(val);
}

void ReadUS() {
  int val = analogRead(US);
  Serial.println(val);
}