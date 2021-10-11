#include <Wire.h>
#include <VirtualWire.h>

#define DRANK 300   // Define max value that we consider

const int SensorPin1 = 5;             // Head sensor
const int SensorPin2 = 6;             // Strap Sensor
const int MQ3 = A0;                   //Alcohol sensor
int st_int = 2;                       //ADXL345 Intrrupt 0 to pin 2
int HeadData,StrapData,AlcoData = 0;
int Head,Strap = 0;

int ADXL345 = 0x53;                   //ADXL345 i2c address

 char HeadDataCharMsg[5];
 char StrapDataCharMsg[5];
 char AlcoDataCharMsg[5];
 int intrid;

void alcoholsens();
void helmetandstrap();
void single_tap();
void intrres();

void setup()
{
    pinMode(SensorPin1,INPUT);
    pinMode(SensorPin2,INPUT);
 Serial.begin(9600);

 // VirtualWire setup
 vw_setup(2000);     // Bits per sec

 //ADXL345
 Wire.begin();
 Wire.beginTransmission(ADXL345);
 Wire.write(0x2D);
 Wire.write(8);
 Wire.endTransmission();
 delay(10);

 single_tap();
 intrres();
}
 
void loop()
{  
  helmetandstrap();
  alcoholsens();
  enabledisableint();
  
 
  // Convert integer data to Char array directly 
  itoa(HeadData,HeadDataCharMsg,5);
  itoa(StrapData,StrapDataCharMsg,5);
  itoa(AlcoData,AlcoDataCharMsg,5);
 
 vw_send((uint8_t *)HeadDataCharMsg, strlen(HeadDataCharMsg));  // send the message
 vw_wait_tx(); // Wait until the whole message is gone
 delay(100);
 vw_send((uint8_t *)StrapDataCharMsg, strlen(StrapDataCharMsg));  // send the message
 vw_wait_tx(); // Wait until the whole message is gone
 delay(100);
 vw_send((uint8_t *)AlcoDataCharMsg, strlen(AlcoDataCharMsg));  // send the message
 vw_wait_tx(); // Wait until the whole message is gone
 delay(100);
}

void alcoholsens(){
  int temp = 0;
  temp = analogRead(MQ3);
  Serial.print(temp);
  if(temp < 200){
    AlcoData = 06;
    }
  else
    AlcoData = 07;
  }

void helmetandstrap(){
  Head = digitalRead(SensorPin1);
  Strap = digitalRead(SensorPin2);

  if(Head == HIGH){
    HeadData = 02;
    }
    else{
    HeadData = 03;
    }
  if(Strap == HIGH){
    StrapData = 04;
    }
    else
    StrapData = 05;
  }

void enabledisableint(){
  if (HeadData == 02 && StrapData == 04 && AlcoData == 06)
  {
    attachInterrupt(digitalPinToInterrupt(st_int), ISRint, LOW);
  }
  else{
    detachInterrupt(digitalPinToInterrupt(st_int));
  }
 }


void single_tap(){
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

void intrres(){
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

void ISRint(){
  intrid = 10;
  //itoa(intrid,msg,5);
 vw_send((uint8_t *)intrid, strlen(intrid));  // send the message
 vw_wait_tx(); // Wait until the whole message is gone
 delay(1000);
 }
