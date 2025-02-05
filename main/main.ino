#include <Servo.h>

#define       POT A0
#define     LIGHT A1
#define        IR A2
#define        US A3
#define       BTN  3

#define     DC_L1 12
#define     DC_L2 13
#define DC_ENABLE  6
#define   DC_ENCA  2
#define   DC_ENCB  7

#define     SERVO 11
#define stp 8
#define dir 9
#define MS1 4
#define MS2 5
#define EN  10
#define STEP_SIZE 1.8

int state = 0;

volatile bool btn0IsPressed = false;
unsigned long debounce0 = 0;

Servo Servo1;

//IR sensor moving average//
const int numReadings = 10;
int irReadings[numReadings];
int irReadIdx = 0;
long irTotal = 0;

volatile int encoderPosition = 0; // Tracks position
int lastEncoded = 0; // Previous state

void isr0() {
  btn0IsPressed = true;
}

void resetEDPins()
{
  digitalWrite(stp, LOW);
  digitalWrite(dir, LOW);
  digitalWrite(MS1, LOW);
  digitalWrite(MS2, LOW);
  digitalWrite(EN, HIGH);
}

void resetDCMotor(){
  analogWrite(DC_ENABLE, 0);  
}

void resetAll(){
  resetDCMotor();
}

int counter = 0;
int currentStateCLK;
int previousStateCLK;
String encdir ="";

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
//  pinMode(     DC_ENC1, OUTPUT);
//  pinMode(     DC_ENC2, OUTPUT);

  pinMode(stp, OUTPUT);
  pinMode(dir, OUTPUT);
  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  pinMode(EN, OUTPUT);
  pinMode(POT, INPUT);

  pinMode(DC_ENCA, INPUT_PULLUP);
  pinMode(DC_ENCB, INPUT_PULLUP);
//  attachInterrupt(digitalPinToInterrupt(DC_ENCA), updateEncoder, CHANGE);
//  attachInterrupt(digitalPinToInterrupt(DC_ENCB), updateEncoder, CHANGE);
  previousStateCLK = digitalRead(DC_ENCA);
  resetEDPins();
  digitalWrite(EN, LOW);
  attachInterrupt(digitalPinToInterrupt(BTN), isr0, RISING);

  Servo1.attach(SERVO);
}




void loop() {
    // Read the current state of inputCLK
//   currentStateCLK = digitalRead(DC_ENCA);
//   
//   if (currentStateCLK != previousStateCLK){
//       if (digitalRead(DC_ENCB) != currentStateCLK) {
//       counter--;
//       encdir ="CCW";
//       } else {
//       // Encoder is rotating clockwise
//       counter++;
//       encdir ="CW";       
//     }
//   }
//   // Update previousStateCLK with the current state
   previousStateCLK = currentStateCLK;
   
  if (CheckForButton0Press()) {
    SwitchState();
  }
  if (state == 0) {
      int pot_reading = analogRead(POT);
      int ms_wait = (int) 10*(1 - float(pot_reading/(float)1024));
      digitalWrite(stp,HIGH);
      delay(1);
      digitalWrite(stp,LOW);
      delay(ms_wait);
      Serial.print("0,");
      Serial.print(pot_reading);
      Serial.print("\n");
  }
  else if (state == 1) {
    int val = analogRead(IR);
    //float volt = map(val, 0, 1023, 0, 5);
    float volt = val * 0.0049;
    //Serial.println(val);
    Serial.print(volt);
    Serial.print("V, ");
    //float dist = -(log(volt - 0.4) - 1.4)/0.077;
    float dist = (6762/(val-9))-4;
    if (volt > 2.5 || volt < 0.4 || isnan(dist)) {
      Serial.println("out of detection range (<10cm or >80cm)");
    }
    else {
      //perform simple moving average//
      irTotal = irTotal - irReadings[irReadIdx];
      irReadings[irReadIdx] = dist;
      irTotal = irTotal + irReadings[irReadIdx];
      irReadIdx = (irReadIdx + 1)%numReadings;
      long average = irTotal/numReadings;
      //
  
      Serial.print(average);
      Serial.println("cm");
  
      if (average < 50) {
        float cmd = map(average, 10, 50, 0, 180);
        CmdServo(cmd);
      }
    }

  } 
  else if (state == 2) {
    int val = analogRead(LIGHT);
//    Serial.print("2,");
    int scaled = (val/(float)300) * 255;
    if(scaled > 255){
      scaled = 255;  
    }else if (scaled < 0){
      scaled = 0;
    }
    analogWrite(DC_ENABLE, scaled);
    digitalWrite(DC_L1, LOW);
    digitalWrite(DC_L2, HIGH);
//    Serial.print(scaled);
//    Serial.print("\n");
    Serial.println(counter);
  }
  else if (state == 3) {
    int val = analogRead(US);
    Serial.print(val);
    Serial.print("\n");
  }else if(state == 4){
    if (Serial.available()) {
        char buffer[20];
        size_t count = Serial.readBytesUntil('\n', buffer, 20);
        buffer[count] = '\0'; // Ensure null-termination
        char command[3];
        int number;
        if (sscanf(buffer, "%2s%d", command, &number) == 2) {
            Serial.print("Command: ");
            Serial.println(command);
            Serial.print("Number: ");
            Serial.println(number);
            if(command[0] == 's'){
              moveStepper(number);  
            }
            else if(command[0] == 'd'){
              moveDCMotor(number);  
            }
        }
    }  
  }
}

void moveDCMotor(int degrees){
    
}
void moveStepper(int degrees){
  int steps = degrees / (float) 1.8;
  if(steps < 0){
    digitalWrite(dir, LOW);
    steps = -steps;
  }
  else{
    digitalWrite(dir,HIGH);
  }
  Serial.println("Alternate between stepping forward and reverse.");
  for(int y=0; y<steps; y++){
    digitalWrite(stp,HIGH); //Trigger one step
    delay(1);
    digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
    delay(1);
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
  state = state % 5;
  Serial.print("Switching to state ");
  Serial.println(state);
  resetAll();
}

void ReadPot() {
  int val = analogRead(POT);
  int D = map(val, 0, 1023, 0, 180);
  Serial.println(D);

//  CmdServo(D);
}

void ReadLight() {
  int val = analogRead(LIGHT);
  Serial.println(val);
}

void ReadIR() {
  int val = analogRead(IR);
  //float volt = map(val, 0, 1023, 0, 5);
  float volt = val * 0.0049;
  //Serial.println(val);
  Serial.print(volt);
  Serial.print("V, ");
  //float dist = -(log(volt - 0.4) - 1.4)/0.077;
  float dist = (6762/(val-9))-4;
  if (volt > 2.5 || volt < 0.4 || isnan(dist)) {
    Serial.println("out of detection range (<10cm or >80cm)");
  }
  else {
    //perform simple moving average//
    irTotal = irTotal - irReadings[irReadIdx];
    irReadings[irReadIdx] = dist;
    irTotal = irTotal + irReadings[irReadIdx];
    irReadIdx = (irReadIdx + 1)%numReadings;
    long average = irTotal/numReadings;
    //

    Serial.print(average);
    Serial.println("cm");

    if (average < 50) {
      float cmd = map(average, 10, 50, 0, 180);
      //CmdServo(cmd);
    }
  }
}

void CmdServo(float cmd) {
  Servo1.write(cmd);
}

void updateEncoder() {
    int MSB = digitalRead(DC_ENCA); // Most significant bit
    int LSB = digitalRead(DC_ENCB); // Least significant bit
    int encoded = (MSB << 1) | LSB;       // Combine bits into a single number
    int sum = (lastEncoded << 2) | encoded; // Combine previous and current states

    if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) {
        encoderPosition++; // Clockwise
    } else if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) {
        encoderPosition--; // Counterclockwise
    }

    lastEncoded = encoded; // Save current state
}
//void ReadUS() {
//  int val = analogRead(US);
//  Serial.println(val)
//}
