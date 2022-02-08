/***********************************************************
Project Name: Smart Helmet (Minor Project)
Code name: Smart Helmet receiver code
Developed by: Ankit
github link: https://github.com/ankit-021/Smart-Helmet-using-Iot
***********************************************************/
#include <VirtualWire.h>
int SensorData;          // RF receiver input
char SensorCharMsg[10];   // RF Transmission container

const int Headled = 2;
const int Strapled = 3;
const int Alcoled = 4;
const int acc_led = 5;
const int start = 6;

bool Head,Strap,Alco = false;
bool check();
void BT();
void allSet();
void notSet();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  for(int i = 2; i < 7; i++){
      pinMode(i, OUTPUT);
    }
    
  for(int i = 2; i < 7; i++){
      digitalWrite(i, LOW);
    }
  vw_set_ptt_inverted(true);
  vw_set_rx_pin(11);
  // Bits per sec
  vw_setup(2000);
  // Start the receiver PLL running
  vw_rx_start();
}

void loop() {
  // put your main code here, to run repeatedly:
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
delay(100);
 BT();
 delay(300);
}

/***********Check incomming data********/
bool check(){
  switch(SensorData){
    //Accident case
    case 9:
    Head = false;
    Strap = false;
    Alco = false;
    digitalWrite(Headled, LOW);
    digitalWrite(Strapled, LOW);
    digitalWrite(Alcoled, LOW);
    digitalWrite(acc_led, HIGH);
    Serial.print("acc\n"); 
    delay(2000);
    break;

    //Normal case
    case 1:
    Head = true;
    Strap = false;
    Alco = false;
    digitalWrite(Headled, HIGH);
    digitalWrite(Strapled, LOW);
    digitalWrite(Alcoled, LOW);
    digitalWrite(acc_led, LOW);
    break;
    
    case 2:
    Head = false;
    Strap = true;
    Alco = false;
    digitalWrite(Headled, LOW);
    digitalWrite(Strapled, HIGH);
    digitalWrite(Alcoled, LOW);
    digitalWrite(acc_led, LOW);
    break;
    
    case 3:
    Head = false;
    Strap = false;
    Alco = true;
    digitalWrite(Headled, LOW);
    digitalWrite(Strapled, LOW);
    digitalWrite(Alcoled, HIGH);
    digitalWrite(acc_led, LOW);
    break;
    
    case 4:
    Head = true;
    Strap = true;
    Alco = false;
    digitalWrite(Headled, HIGH);
    digitalWrite(Strapled, HIGH);
    digitalWrite(Alcoled, LOW);
    digitalWrite(acc_led, LOW);
    break;
    
    case 5:
    Head = false;
    Strap = true;
    Alco = true;
    digitalWrite(Headled, LOW);
    digitalWrite(Strapled, HIGH);
    digitalWrite(Alcoled, HIGH);
    digitalWrite(acc_led, LOW);
    break;
    
    case 6:
    Head = true;
    Strap = false;
    Alco = true;
    digitalWrite(Headled, HIGH);
    digitalWrite(Strapled, LOW);
    digitalWrite(Alcoled, HIGH);
    digitalWrite(acc_led, LOW);
    break;
    
    case 7:
    Head = true;
    Strap = true;
    Alco = true;
    digitalWrite(Headled, HIGH);
    digitalWrite(Strapled, HIGH);
    digitalWrite(Alcoled, HIGH);
    digitalWrite(acc_led, LOW);
    break;
    
    /*case 18:
    Head = false;
    Strap = false;
    Alco = false;
    digitalWrite(Headled, LOW);
    digitalWrite(Strapled, LOW);
    digitalWrite(Alcoled, LOW);
    digitalWrite(acc_led, LOW);
    break;*/
    default:
    Head = false;
    Strap = false;
    Alco = false;
    digitalWrite(Headled, LOW);
    digitalWrite(Strapled, LOW);
    digitalWrite(Alcoled, LOW);
    digitalWrite(acc_led, LOW);
    break;
  }
  delay(100);
  
  if (Head == true && Strap == true && Alco == true)
  {
    return true;
  }
  else{
    return false;
  }
}

/***********all ok or not********/
void allSet(){
    digitalWrite(start, HIGH);
    Serial.println("All ok\n");
}

void notSet(){
    digitalWrite(start, LOW);
    Serial.println("All not ok\n");
}

/***********bluetooth data TX RX********/
void BT(){
  if(Serial.available() > 0)  
  {
    //Serial.println(SensorData);      //Read the incoming data and store it into variable Incoming_value
    if(Head == false && Strap == false && Alco == false){
      Serial.print("hf\n");
      Serial.print("sf\n");
      Serial.print("af\n"); 
      }
      else if(Head == false && Strap == false && Alco == true){
      Serial.print("hf\n");
      Serial.print("sf\n");
      Serial.print("at\n"); 
      }
      else if(Head == false && Strap == true && Alco == false){
      Serial.print("hf\n");
      Serial.print("st\n");
      Serial.print("af\n"); 
      }
      else if(Head == false && Strap == true && Alco == true){
      Serial.print("hf\n");
      Serial.print("st\n");
      Serial.print("at\n"); 
      }
      else if(Head == true && Strap == false && Alco == false){
      Serial.print("ht\n");
      Serial.print("sf\n");
      Serial.print("af\n"); 
      }
      else if(Head == true && Strap == false && Alco == true){
      Serial.print("ht\n");
      Serial.print("sf\n");
      Serial.print("at\n"); 
      }
      else if(Head == true && Strap == true && Alco == false){
      Serial.print("ht\n");
      Serial.print("st\n");
      Serial.print("af\n"); 
      }
      else if(Head == true && Strap == true && Alco == true){
      Serial.print("ht\n");
      Serial.print("st\n");
      Serial.print("at\n"); 
      }

      
    
  }
  delay(100);
 }
