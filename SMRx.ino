#include <VirtualWire.h>
int SensorData;          // RF receiver input
char SensorCharMsg[5];   // RF Transmission container
//String Incoming_value; //Variable for storing Incoming_value

const int Headled = 2;
const int Strapled = 3;
const int Alcoled = 4;
const int acc_led = 5;
const int start = 6;

bool Head,Strap,Alco = false;
void BT();
bool check();
void allSet();
void notSet();

void setup() {

  for(int i = 2; i < 7; i++){
      pinMode(i, OUTPUT);
    }
    
  for(int i = 2; i < 7; i++){
      digitalWrite(i, LOW);
    }

  Serial.begin(9600); 
  
  vw_set_ptt_inverted(true);
  // Bits per sec
  vw_setup(2000);
  // Start the receiver PLL running
  vw_rx_start();
}

void loop() {

  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;

    if (vw_get_message(buf, &buflen))
  {
    for (int i = 0; i < buflen; i++)
    {
      // Fill SensorCharMsg Char array with corresponding
      // chars from buffer.
      SensorCharMsg[i] = char(buf[i]);
      }
      
    
     SensorCharMsg[buflen] = '\0';
    // Convert Sensor1CharMsg Char array to integer
    SensorData = atoi(SensorCharMsg);
    Serial.println(SensorData);
  }

  if(check() == true){
     allSet();
    }
  else{
     notSet();
   }
   
  if (SensorData == 10)
  {
    digitalWrite(acc_led, HIGH);
    Serial.println("accident"); 
    delay(1000);
  }
/*---------------------------------------------*/
  
  BT(); //Bluetooth TX RX function

  delay(100);
}

bool check(){
    if (SensorData == 02)
  {
    Head = true;
    digitalWrite(Headled, HIGH);
  }
   if (SensorData == 03)
  {
    Head = false;
    digitalWrite(Headled, LOW);
  }
  
    if (SensorData == 04)
  {
    Strap = true;
    digitalWrite(Strapled, HIGH);
  }
    if (SensorData == 05)
  {
    Strap = false;
    digitalWrite(Strapled, LOW);
  }
  
    if (SensorData == 06)
  {
    Alco = true;
    digitalWrite(Alcoled, HIGH);
  }
    if (SensorData == 07)
  {
    Alco = false;
    digitalWrite(Alcoled, LOW);
  }
  delay(50);
  
  if (Head == true && Strap == true && Alco == true)
  {
    return true;
  }
  else{
    return false;
  }
}

void allSet(){
    digitalWrite(start, HIGH);
    Serial.println("All ok");
}

void notSet(){
    digitalWrite(start, LOW);
    Serial.println("All not ok");
}
 
void BT(){
  if(Serial.available() > 0)  
  {
    //Serial.println(SensorData);      //Read the incoming data and store it into variable Incoming_value
    if(Head == true){
      Serial.println("Helmettrue"); 
      }
    if(Head == false){
      Serial.println("Helmetfalse"); 
      }
    if(Strap == true){
      Serial.println("Straptrue"); 
      }
    if(Strap == false){
      Serial.println("Strapfalse"); 
      }
    if(Alco == true){
      Serial.println("Alcoholtrue"); 
      }
    if(Alco == false){
      Serial.println("Alcoholfalse"); 
      }
  }
 }
