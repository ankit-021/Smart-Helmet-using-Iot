/***********************************************************
Project Name: Smart Helmet (Minor Project)
Code name: Smart Helmet Transmitter code
Developed by: Ankit
github link: https://github.com/ankit-021/Smart-Helmet-using-Iot
***********************************************************/
#include <VirtualWire.h>
#include <Wire.h>
#include <SparkFun_ADXL345.h>
#define DRANK 750   // Define max value that we consider
#define MQ3 A0

const int SensorPin1 = 5;             // Head sensor
const int SensorPin2 = 6;             // Strap Sensor
//const int MQ3 = 7;                   //Alcohol sensor
int st_int = 3;                       //ADXL345 Intrrupt 1 to pin 3
int sdata = 0;
int Head,Strap,Alco = 0;
//bool sensorcon = false;

ADXL345 adxl = ADXL345();
 char SensorCharMsg[5];
 //char inttMsg[5];
 //int intrid;

void Sensordata();
//void enabledisableint();
void ADXL_ISR();

void setup() {
  pinMode(SensorPin1,INPUT);
  pinMode(SensorPin2,INPUT);
  //pinMode(MQ3,INPUT);
  
  // VirtualWire setup
  vw_set_ptt_inverted(true);
  vw_set_tx_pin(12);
 vw_setup(2000);     // Bits per sec
 //Serial.begin(9600);
adxl.powerOn();                     // Power on the ADXL345

  adxl.setRangeSetting(2);           // Give the range settings
                                      // Accepted values are 2g, 4g, 8g or 16g
                                      // Higher Values = Wider Measurement Range
                                      // Lower Values = Greater Sensitivity

  adxl.setSpiBit(0);                  // Configure the device to be in 4 wire SPI mode when set to '0' or 3 wire SPI mode when set to 1
                                      // Default: Set to 1
                                      // SPI pins on the ATMega328: 11, 12 and 13 as reference in SPI Library 
   
  adxl.setActivityXYZ(1, 0, 0);       // Set to activate movement detection in the axes "adxl.setActivityXYZ(X, Y, Z);" (1 == ON, 0 == OFF)
  adxl.setActivityThreshold(75);      // 62.5mg per increment   // Set activity   // Inactivity thresholds (0-255)
 
  adxl.setInactivityXYZ(1, 0, 0);     // Set to detect inactivity in all the axes "adxl.setInactivityXYZ(X, Y, Z);" (1 == ON, 0 == OFF)
  adxl.setInactivityThreshold(75);    // 62.5mg per increment   // Set inactivity // Inactivity thresholds (0-255)
  adxl.setTimeInactivity(10);         // How many seconds of no activity is inactive?

  adxl.setTapDetectionOnXYZ(0, 0, 1); // Detect taps in the directions turned ON "adxl.setTapDetectionOnX(X, Y, Z);" (1 == ON, 0 == OFF)
 
  // Set values for what is considered a TAP and what is a DOUBLE TAP (0-255)
 adxl.setTapThreshold(50);           // 62.5 mg per increment
  adxl.setTapDuration(15);            // 625 μs per increment
  adxl.setDoubleTapLatency(80);       // 1.25 ms per increment
  adxl.setDoubleTapWindow(200);       // 1.25 ms per increment
 
  // Set values for what is considered FREE FALL (0-255)
  adxl.setFreeFallThreshold(7);       // (5 - 9) recommended - 62.5mg per increment
  adxl.setFreeFallDuration(30);       // (20 - 70) recommended - 5ms per increment
 
  // Setting all interupts to take place on INT1 pin
  adxl.setImportantInterruptMapping(1, 2, 1, 1, 1);     // Sets "adxl.setEveryInterruptMapping(single tap, double tap, free fall, activity, inactivity);" 
                                                        // Accepts only 1 or 2 values for pins INT1 and INT2. This chooses the pin on the ADXL345 to use for Interrupts.
                                                        // This library may have a problem using INT2 pin. Default to INT1 pin.
  
  // Turn on Interrupts for each mode (1 == ON, 0 == OFF)
  adxl.InactivityINT(0);
  adxl.ActivityINT(0);
  adxl.FreeFallINT(0);
  adxl.doubleTapINT(1);
  adxl.singleTapINT(0);
  
  //attachInterrupt(digitalPinToInterrupt(st_int), ADXL_ISR, CHANGE);  

}

void loop() {
  // put your main code here, to run repeatedly:
  Sensordata();
  delay(100);
  //enabledisableint();
  ADXL_ISR();
 delay(100);
}

/***********Sensor data input and send********/
void Sensordata(){
  Head = digitalRead(SensorPin1);
  Strap = digitalRead(SensorPin2);
  Alco = analogRead(MQ3);

  if(Head == HIGH && Strap == LOW && Alco>DRANK){
    sdata = 1;
    //sensorcon = false;
    }
  else if(Head == LOW && Strap == HIGH && Alco>DRANK){
    sdata = 2;
    //sensorcon = false;
    }
  else if(Head == LOW && Strap == LOW && Alco<DRANK){
    sdata = 3;
    //sensorcon = false;
    }
  else if(Head == HIGH && Strap == HIGH && Alco>DRANK){
    sdata = 4;
    //sensorcon = false;
    }
  else if(Head == LOW && Strap == HIGH && Alco<DRANK){
    sdata = 5;
    //sensorcon = false;
    }
  else if(Head == HIGH && Strap == LOW && Alco<DRANK){
    sdata = 6;
    //sensorcon = false;
    }
  else if(Head == HIGH && Strap == HIGH && Alco<DRANK){
    sdata = 7;
    //sensorcon = true;
  }
  else if(Head == LOW && Strap == LOW && Alco>DRANK){
    sdata = 8;
    //sensorcon = false;
  }
  
  //Serial.println(SensorData);
  itoa(sdata,SensorCharMsg,10);
  //Serial.println(SensorCharMsg);
  delay(1000);
  vw_send((uint8_t *)SensorCharMsg, strlen(SensorCharMsg));  // send the message
  vw_wait_tx(); // Wait until the whole message is gone
  delay(500);
 }

/***********Interrupt function********/
  void ADXL_ISR() {
  
  // getInterruptSource clears all triggered actions after returning value
  // Do not call again until you need to recheck for triggered actions
  detachInterrupt(digitalPinToInterrupt(st_int)); 
  byte interrupts = adxl.getInterruptSource();
  
  // Tap Detection
  if(adxl.triggered(interrupts, ADXL345_DOUBLE_TAP)){
    //Serial.print(intrid);
    
  sdata = 9;
  itoa(sdata,SensorCharMsg,10);
  //Serial.print(inttMsg); 
 vw_send((uint8_t *)SensorCharMsg, strlen(SensorCharMsg));  // send the message
 vw_wait_tx(); // Wait until the whole message is gone
 delay(2000);
 
  
  else{
    delay(100);
    } 
}
