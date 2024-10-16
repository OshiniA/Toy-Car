#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8); // CE, CSN
const byte addresses [][6] = {"00001", "00002"};  //Setting the two addresses. One for transmitting and one for receiving
int BaseData[6];
int ReceiviedData[6];
  
// Motor A - Run
bool in1Val = false;
bool in2Val = false;
 
// Motor B - Turn
bool in3Val = false;
bool in4Val = false;
 
// Joystick Input - Run
int RunjoyVert = A0; // Vertical  
int RunjoyHorz = A1; // Horizontal

// Joystick Input - Turn
int TurnjoyVert = A2; // Vertical  
int TurnjoyHorz = A3; // Horizontal
 
// Motor Speed Values - Start at zero
int RunMotorSpeed = 0;
int TurnMotorSpeed = 0;
 
// Joystick Values - Start at 512 (middle position)
int RunjoyposVert = 512;
int RunjoyposHorz = 512;
int TurnjoyposVert = 512;
int TurnjoyposHorz = 512;  
 
void setup()
{
  Serial.begin(9600);  
  radio.begin();                           //Starting the radio communication
  radio.setChannel(1);
  radio.setPALevel(RF24_PA_MAX);           // If you want to save power use "RF24_PA_MIN" but keep in mind that reduces the module's range
  radio.setDataRate(RF24_2MBPS);
  radio.setAutoAck(1);                     // Ensure autoACK is enabled
  radio.setRetries(2, 15);                 // Optionally, increase the delay between retries & # of retries
  radio.setCRCLength(RF24_CRC_8);          // Use 8-bit CRC for performance
  radio.openWritingPipe(addresses[1]);     //Setting the address at which we will send the data
  radio.openReadingPipe(1, addresses[0]);  //Setting the address at which we will receive the data
}
 
void loop() {
 	// Read the Joystick X and Y positions
  RunjoyposVert = 0;
  RunjoyposHorz = 0;
  TurnjoyposVert = 0;
  TurnjoyposHorz = 0;
  in1Val = LOW;
  in2Val = LOW;
  in3Val = LOW;
  in4Val = LOW;    
  RunMotorSpeed = 0;
  TurnMotorSpeed =0;  
   
 	RunjoyposVert = analogRead(RunjoyVert); 
	RunjoyposHorz = analogRead(RunjoyHorz);
 	TurnjoyposVert = analogRead(TurnjoyVert); 
	TurnjoyposHorz = analogRead(TurnjoyHorz);

	// Determine if this is a forward or backward motion
	// Do this by reading the Verticle Value
	// Apply results to MotorSpeed and to Direction
	if (RunjoyposVert < 460)
	{
		// This is Backward
		// Set Motor A backward
    in1Val = LOW;
    in2Val = HIGH;
		//Determine Motor Speeds , As we are going backwards we need to reverse readings
 		RunjoyposVert = RunjoyposVert - 460; // This produces a negative number
		RunjoyposVert = RunjoyposVert * -1;  // Make the number positive
		RunMotorSpeed = map(RunjoyposVert, 0, 460, 0, 255);
 	}
	else if (RunjoyposVert > 564)
	{
		// This is Forward
 		// Set Motor A forward
    in1Val = HIGH;
    in2Val = LOW;    
		//Determine Motor Speeds
		RunMotorSpeed = map(RunjoyposVert, 564, 1023, 0, 255);
 	}
	else
	{
		// This is Stopped
		RunMotorSpeed = 0;
	}
	// Now do the steering
	// The Horizontal position will "weigh" the motor speed
	// Values for each motor
	if (TurnjoyposHorz < 460)
	{
		// Move Left
		// Set Motor A backward
    in3Val = LOW;
    in4Val = HIGH;    
 		// As we are going left we need to reverse readings
 		TurnjoyposHorz = TurnjoyposHorz - 1023; // This produces a negative number
		TurnjoyposHorz = TurnjoyposHorz * -1;  // Make the number positive
		TurnMotorSpeed = map(TurnjoyposHorz, 564, 1023, 0, 110);
		// Don't exceed range of 0-255 for motor speeds
		//if (TurnMotorSpeed > 50) TurnMotorSpeed = 50;
	}
	else if (TurnjoyposHorz > 564)
	{
		// Move Right
		// Set Motor A backward
    in3Val = HIGH;
    in4Val = LOW;    
 		// Map the number to a value of 255 maximum
 		TurnjoyposHorz = map(TurnjoyposHorz, 564, 1023, 0, 110);
		TurnMotorSpeed = TurnMotorSpeed - TurnjoyposHorz;
		TurnMotorSpeed = TurnMotorSpeed * -1;  // Make the number positive    
		// Don't exceed range of 0-255 for motor speeds
		if (TurnMotorSpeed < 0)TurnMotorSpeed = 0;		 
	}
	// Adjust to prevent "buzzing" at very low speed
	if (RunMotorSpeed < 8)  RunMotorSpeed = 0;
	if (TurnMotorSpeed < 8) TurnMotorSpeed = 0;
// New code
  Serial.print("CarTansmitter: ");Serial.print("RunMotorSpeed=");Serial.print(RunMotorSpeed);
  Serial.print("  in1Val=");Serial.print(in1Val);
  Serial.print("  in2Val=");Serial.print(in2Val);

  Serial.print("  TurnMotorSpeed=");Serial.print(TurnMotorSpeed);
  Serial.print("  in3Val=");Serial.print(in3Val);
  Serial.print("  in4Val=");Serial.println(in4Val);
  
  BaseData[0]=RunMotorSpeed; //MotorSpeed1
  BaseData[1]=in1Val; // in1 value
  BaseData[2]=in2Val; // in2 value

  BaseData[3]=TurnMotorSpeed;  //MotorSpeed2
  BaseData[4]=in3Val; // in3 value
  BaseData[5]=in4Val; // in4 value

  radio.stopListening();
  radio.write(BaseData, sizeof(BaseData) );
//New code end
}