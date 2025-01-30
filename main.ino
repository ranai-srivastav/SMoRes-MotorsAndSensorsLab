#define POT A0
#define LIGHT A1
#define IR A2
#define US A3
#define BTN 3

int state = 0;

volatile bool btn0IsPressed = false;
unsigned long debounce0 = 0;

const int MAX_PWM_VOLTAGE = 255;
const int NOM_PWM_VOLTAGE = 150;

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
  attachInterrupt(digitalPinToInterrupt(BTN), isr0, RISING);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (CheckForButton0Press()) {
    SwitchState();
  }

  if (state == 0) {
    ReadPot();
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

void ReadPot() {
  int val = analogRead(POT);
  int D = map(val, 0, 1023, -NOM_PWM_VOLTAGE, NOM_PWM_VOLTAGE);
  Serial.println(D);

  // //END A5 CONTROL SECTION

  // //Ensure that you don't go past the maximum possible command
  // if (D > MAX_PWM_VOLTAGE) {
  //     D = MAX_PWM_VOLTAGE;
  // }
  // else if (D < -MAX_PWM_VOLTAGE) {
  //     D = -MAX_PWM_VOLTAGE;
  // }

  // //Map the D value to motor directionality
  // //FLIP ENCODER PINS SO SPEED AND D HAVE SAME SIGN
  // if (D > 0) {
  //     ledcWrite(ledChannel_1, LOW);
  //     ledcWrite(ledChannel_2, D);
  // }
  // else if (D < 0) {
  //     ledcWrite(ledChannel_1, -D);
  //     ledcWrite(ledChannel_2, LOW);
  // }
  // else {
  //     ledcWrite(ledChannel_1, LOW);
  //     ledcWrite(ledChannel_2, LOW);
  // }
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