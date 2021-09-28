#include <Wire.h>

int LED = 13;
int st_int = 2;
volatile int state = LOW;
 
int ADXL345 = 0x53;

float xout,yout,zout;
void setup() {
  // put your setup code here, to run once:
  pinMode(LED, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(st_int), blinking, CHANGE);
  
 Serial.begin(9600); 
 Wire.begin();
 Wire.beginTransmission(ADXL345);
 Wire.write(0x2D);
 Wire.write(8);
 Wire.endTransmission();
 delay(10);

 stap();
 intrr();
}

void loop() {
  // put your main code here, to run repeatedly:
digitalWrite(13, state);
 showaxis();
 
}

void stap(){
  //threshold
  Wire.beginTransmission(ADXL345);
  Wire.write(0x1D); // tap threshold res
  Wire.write(50);    //scale 62.5 mg
  Wire.endTransmission();
  delay(10);

  //duration
  Wire.beginTransmission(ADXL345);
  Wire.write(0x21); // tap duration res
  Wire.write(15);    //scale 625 ug
  Wire.endTransmission();
  delay(10);

  //tap axes
  Wire.beginTransmission(ADXL345);
  Wire.write(0x2A); // tap axes res
  Wire.write(7);    //00000111
  Wire.endTransmission();
  delay(10);
}

void intrr(){
  //int map
  Wire.beginTransmission(ADXL345);
  Wire.write(0x2F);    //int map res
  Wire.write(0xBF);    //single tap at int0
  Wire.endTransmission();
  delay(10);
  //int enable
  Wire.beginTransmission(ADXL345);
  Wire.write(0x2E);    //int enable res
  Wire.write(0x40);    //single tap
  Wire.endTransmission();
  delay(10);
  }

void showaxis(){
 Wire.beginTransmission(ADXL345);
 Wire.write(0x32);
 Wire.endTransmission(false);
 Wire.requestFrom(ADXL345,6,true);
 xout = (Wire.read()|Wire.read()<<8);
 xout = xout/256;
 yout = (Wire.read()|Wire.read()<<8);
 yout = yout/256;
 zout = (Wire.read()|Wire.read()<<8);
 zout = zout/256;

 Serial.print("X = ");
 Serial.print(xout);
 Serial.print(" ");
 Serial.print("Y = ");
 Serial.print(yout);
 Serial.print(" ");
 Serial.print("Z = ");
 Serial.println(zout);

 Wire.endTransmission();
 delay(100);
}
void blinking() { 
   //ISR function
   
   state = !state; //toggle the state when the interrupt occurs
   
}
