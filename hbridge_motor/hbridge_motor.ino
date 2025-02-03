#define     DC_L1 12
#define     DC_L2 13
#define DC_ENABLE  6
#define   DC_ENC1  0
#define   DC_ENC2  1

void setup() {
  pinMode(       DC_L1, OUTPUT);
  pinMode(       DC_L2, OUTPUT);
  pinMode(MOTOR_ENABLE, OUTPUT);

}

void loop() {
  digitalWrite(L1, LOW);
  digitalWrite(L2, LOW);
  analogWrite(MOTOR_ENABLE, 175);
  val1 = digitalRead(DC_ENC1)
  val2 = degitalRead(DC_ENC2)

  

}
