#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#define DHT11PIN 6
RF24 radio(7, 8); // CE, CSN
const byte addresses [][6] = {"00001", "00002"};    //Setting the two addresses. One for transmitting and one for receiving
int datareceived[6];
int sendingBackdata[6];
int RunMotorSpeed; 
int TurnMotorSpeed; 
int in1Val,in2Val,in3Val,in4Val;
// Motor A
int enA = 9;
int in1 = 10;//8
int in2 = 6;//7
// Motor B
int enB = 3;
int in3 = 5;
int in4 = 4;

void setup() {
    
  Serial.begin(9600);
  // Set all the motor control pins to outputs
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  // Start with motors disabled and direction forward
  // Motor A - Run (Back))
  //digitalWrite(enA, LOW);
  //digitalWrite(in1, HIGH);
  //digitalWrite(in2, LOW);
  // Motor B - Turn
  //digitalWrite(enB, LOW);
  //digitalWrite(in3, HIGH);
  //digitalWrite(in4, LOW);

  radio.begin();                            //Starting the radio communication
  radio.setChannel(1);
  radio.setPALevel(RF24_PA_MAX);           // LOW If you want to save power use "RF24_PA_MIN" but keep in mind that reduces the module's range
  radio.setDataRate(RF24_2MBPS);
  radio.setAutoAck(1);                     // Ensure autoACK is enabled
  radio.setRetries(2, 15);                 // Optionally, increase the delay between retries & # of retries
  radio.setCRCLength(RF24_CRC_8);          // Use 8-bit CRC for performance  
  radio.openWritingPipe(addresses[0]);      //Setting the address at which we will send the data
  radio.openReadingPipe(1, addresses[1]);   //Setting the address at which we will receive the data
 
}
void loop() 
{
  radio.startListening();                    //This sets the module as receiver
  delay(20);
  if (radio.available()) {
    radio.read(datareceived, sizeof(datareceived) );
    RunMotorSpeed= datareceived[0] ;
    in1Val= datareceived[1] ;
    in2Val= datareceived[2] ;
    TurnMotorSpeed= datareceived[3] ;
    in3Val= datareceived[4] ;
    in4Val= datareceived[5] ;
    Serial.print("CarReceiver: ");Serial.print("RunMotorSpeed=");Serial.print(RunMotorSpeed);
    Serial.print("  in1Val=");Serial.print(in1Val);
    Serial.print("  in2Val=");Serial.print(in2Val);
    Serial.print("  TurnMotorSpeed=");Serial.print(TurnMotorSpeed);  
    Serial.print("  in3Val=");Serial.print(in3Val);  
    Serial.print("  in4Val=");Serial.println(in4Val); 
    //Run
    digitalWrite(enA, RunMotorSpeed);
		digitalWrite(in1, in1Val);
		digitalWrite(in2, in2Val);
    //Turn
    digitalWrite(enB, TurnMotorSpeed);
		digitalWrite(in1, in3Val);
		digitalWrite(in2, in4Val);         
   }
else   
{
  // Motor A - Run (Back))
  //digitalWrite(enA, LOW);
  //digitalWrite(in1, HIGH);
  //digitalWrite(in2, LOW);
  // Motor B - Turn
  //digitalWrite(enB, LOW);
  //digitalWrite(in3, HIGH);
  //digitalWrite(in4, LOW);
}
 //***********************************************************************************  
  radio.stopListening();
  sendingBackdata[0]=400;  
  sendingBackdata[1]=555;
  sendingBackdata[2]=666 ;
  //radio.write(sendingBackdata, sizeof(sendingBackdata));   //Sending the data
  delay(100);
  
  // Motor A - Run (Back))
 // digitalWrite(enA, 50);
  //digitalWrite(in1, HIGH);
  //digitalWrite(in2, LOW);
  // Motor B - Turn
  //digitalWrite(enB, LOW);
  //digitalWrite(in3, HIGH);
  //digitalWrite(in4, LOW);  
}
