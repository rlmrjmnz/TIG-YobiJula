#include <Wire.h>

// Gyro constants
#define GYRO 0x68
#define A_R 16384.0
#define G_R 131.0
#define RAD_A_DEG = 57.295779

// Force Sensitive Resistor: FSR
int FSR_Pin = A0; //analog pin 0
int Lamina = A1;
int FSR_Led = 11;
int Lam_Led = A4;
int SVM1_Pin = 8; // digital pin 9 use for send V to motor
int SVM2_Pin = 9;

// set values for send to Shaftless Vibration Motor: SVM
double min_volt = 1.3;
double max_volt = 3.8;

// Gyro vars
int16_t AcX, AcY, AcZ, GyX, GyY;
float Gy[2];
float Acc[2];
float Angle[2];

int led_on = 50;

void setup(){

  //debugging
  Serial.begin(9600);
  
  //SVM Pins
  pinMode(SVM1_Pin, OUTPUT);   // sets the pin as output
  pinMode(SVM2_Pin, OUTPUT);   // sets the pin as output
 
  //Led Pins
  pinMode(FSR_Led, OUTPUT);
 
  //testing led
  digitalWrite(FSR_Led, true);
  delay(1000);
  digitalWrite(FSR_Led, false);
 
  //gyro
  init_gyro();
}


void loop(){
  // readjust multipliers for gyro readings
  float lat_mul = 1.0;
  float upw_mut = 1.0;
 
  // read initial glove position
  read_gyro();
  float init_angle1 = Angle[0];
  float init_angle2 = Angle[1];
 
  // read fingertips pressure
  int FSRReading = analogRead(FSR_Pin);
  int LamReading = analogRead(Lamina);

  // calcs vibration to apply
  double voltage1 = (((max_volt - min_volt) / -922) * FSRReading) + max_volt;
  double voltage2 = (((max_volt - min_volt) / -140) * LamReading) + max_volt;

  int analog1 = (voltage1 * 255) / 5; //analog data must be 65 - 160
  int analog2 = (voltage2 * 255) / 5; //analog data must be 65 - 160

  // read actual glove position
  read_gyro();
  
  // readjust vibration based on glove position 
  // the gyro is inverted in the gauntlet so we can invert data axis
  if(Angle[0]<init_angle1)
    //left
    // original lat_mul = 1.3;
    //lat_mul = 0.7;
    analog1 *= 1.3;
  else if(Angle[0]>init_angle1)
    //right
    // original lat_mul = 0.7;
    //lat_mul = 1.3;
    analog2 *= 1.3;
  if(Angle[1]<init_angle2)
    //up
    // original analog1 *= 1.3;
    //analog2 *= 1.3;
    lat_mul = 1.3;
  else if(Angle[1]>init_angle2)
    //down
    // original analog2 *= 1.3;
    //analog1 *= 1.3;
    lat_mul = 0.7;

  analog1 *= lat_mul;
  analog2 *= lat_mul;

  // apply vibrations on fingertips
  if(FSRReading != 1023){
    analogWrite(SVM1_Pin, analog1);
    digitalWrite(FSR_Led, true);
    delay(led_on);
  }else{
    analogWrite(SVM1_Pin, 0);
    digitalWrite(FSR_Led, false);
  }
 

  if(LamReading >=180){
    analogWrite(SVM2_Pin, 0);
    digitalWrite(FSR_Led, false);
  }else{
    analogWrite(SVM2_Pin, analog2);
    digitalWrite(FSR_Led, true);
    delay(led_on);
  }

  // debugging
  //Serial.println(FSRReading);
  //Serial.println(LamReading);
  
  delay(250); //just here to slow down the output for easier reading
}

// init gyro communication
void init_gyro(){

  Wire.begin();
  Wire.beginTransmission(GYRO);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
}

// read gyro position
void read_gyro(){

  // Accel readings - needed for better gyro angles
  Wire.beginTransmission(GYRO);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(GYRO,6,true);
  AcX=Wire.read()<<8|Wire.read();
  AcY=Wire.read()<<8|Wire.read();
  AcZ=Wire.read()<<8|Wire.read();
 
  Acc[1] = atan(-1*(AcX/A_R)/sqrt(pow((AcY/A_R),2) + pow((AcZ/A_R),2)))*RAD_TO_DEG;
  Acc[0] = atan((AcY/A_R)/sqrt(pow((AcX/A_R),2) + pow((AcZ/A_R),2)))*RAD_TO_DEG;

  // Gyro readings
  Wire.beginTransmission(GYRO);
  Wire.write(0x43);
  Wire.endTransmission(false);
  Wire.requestFrom(GYRO,4,true);
  GyX=Wire.read()<<8|Wire.read();
  GyY=Wire.read()<<8|Wire.read();

  // Gyro angles
  Gy[0] = GyX/G_R;
  Gy[1] = GyY/G_R;
 
  Angle[0] = 0.98 *(Angle[0]+Gy[0]*0.010) + 0.02*Acc[0];
  Angle[1] = 0.98 *(Angle[1]+Gy[1]*0.010) + 0.02*Acc[1];
}


