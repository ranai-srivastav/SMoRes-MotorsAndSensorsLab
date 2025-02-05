#define     DC_L1 12
#define     DC_L2 13
#define DC_ENABLE  6
#define   DC_ENC1  0
#define   DC_ENC2  1
#define        US A3

int t = 0;

void setup() {
  Serial.begin(9600);

  pinMode(       DC_L1, OUTPUT);
  pinMode(       DC_L2, OUTPUT);
  pinMode(   DC_ENABLE, OUTPUT);
  pinMode(     DC_ENC1, OUTPUT);
  pinMode(     DC_ENC2, OUTPUT);

  pinMode(US, INPUT);

}

// void loop () { 
//   // read_sensor(); 
//   print_range(); 
//   delay(100); 
//   } 

void loop() {
  analogWrite(DC_ENABLE, 255);
  digitalWrite(DC_L1, LOW);
  digitalWrite(DC_L2, HIGH);

  int val1 = digitalRead(DC_ENC1);
  int val2 = digitalRead(DC_ENC2);
}

void getCounts()
{
  
}

// void Ultrasound_DCMotor()
// {
//   // mV = mV/inch * inch + mV_bias ===> mV_bias = 0
//   // mV / (mV/inch) = inch
//   int US_ADC_counts = analogRead(US);
//   float US_voltage_mV = US_ADC_counts * 5.0 / 1023.0 * 1000;
//   float US_dist_inches = US_voltage_mV / 9.8;
//   Serial.print("US_voltage_mV: ");
//   Serial.print(US_voltage_mV);
//   Serial.print(" and US_dist_inches: ");
//   Serial.println(US_dist_inches);
// }

// void read_sensor (){ 
//   int anVolt = analogRead(US); 
//   float mm = anVolt*5; //Takes bit count and converts it to mm 
//   float inches = mm/25.4; //Takes mm and converts it to inches 
//   } 
