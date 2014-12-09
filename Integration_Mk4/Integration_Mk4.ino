#include "DualMC33926MotorShield.h"
#define p_pin 2
#define t_pin 3
#define p_pin_B 5
#define t_pin_B 6
#define pNum_ticks 720
#define tNum_ticks 1620
#define tPot A3
#define pPot A3
#define tlswitch 13
#define plswitch 11
#define marker A2

// Create the motor shield object with the default I2C address
DualMC33926MotorShield md;

// Encoder stuff (counters and ticks)
volatile int pCounter;
volatile int tCounter;
const byte pDivider = pNum_ticks / 360.;
const float tDivider = tNum_ticks / 360.;

// Angle measures
float pAngle;
float tAngle;
float pTarget_angle = 0;
float tTarget_angle = 0;

// PySerial stuff
String incoming = "";
String panString = "";
String tiltString = "";

// Printing timing
double time;
double prev_time;

// Motor control
int maxVelocity = 100;
double pError_p;
double tError_p;
const float pkp = 30;
const float tkp = 40;
//double pError_i;
//double tError_i;
//const float pki = 0;
//const float tki = 0;

// Motor velocities
double pVelocity;
double tVelocity;

//servo control
int pos;
//ServoTimer2 loadServo;

//marker control
long fireTime = 0;
int fireDelay = 1000;
boolean toFire = LOW;

void setup() {
  Serial.begin(9600);
  pinMode(p_pin, INPUT);
  pinMode(t_pin, INPUT);
  pinMode(p_pin_B, INPUT);
  pinMode(t_pin_B, INPUT);
  pinMode(tPot, INPUT);
  pinMode(pPot, INPUT);
  pinMode(marker, OUTPUT);
  pinMode(tlswitch, INPUT);
  //loadServo.attach(A3);
  // enable internal pullup resistors
  digitalWrite(p_pin, 1);
  digitalWrite(t_pin, 1);
  digitalWrite(p_pin_B, 1);
  digitalWrite(t_pin_B, 1);
  // using encoders with interrupts
  attachInterrupt(0, ISR_pan, CHANGE);
  attachInterrupt(1, ISR_tilt, CHANGE);
  //loadServo.write(544);
  // Start motor shield
  md.init();
  
  boolean zero = LOW;
  boolean tzero = HIGH;
  boolean pzero = HIGH;
  while (!zero) {
    delay(50);
    Serial.println("Zeroing");
    if (pzero) {
       md.setM1Speed(-100);
       pzero = digitalRead(plswitch);
    } else {
      md.setM1Speed(0);
    }
    if (tzero) {
       md.setM2Speed(-100);
       tzero = digitalRead(tlswitch);
    } else {
      md.setM2Speed(0);
    }
    if (!tzero && !pzero) {
      zero = HIGH;
    }
  } 
  pCounter=0;
  tCounter=0;
  //tell PySerial you're ready to go
  Serial.print("Initialized!");
}

void loop() {
  
  if (Serial.available() > 0) { //check for new data
    incoming = Serial.readString();
    panString = incoming.substring(0,3);
    tiltString = incoming.substring(3,6);
    pTarget_angle = panString.toInt();
    tTarget_angle = tiltString.toInt();
    
    //start fire timer
    fireTime = millis();
    toFire = HIGH;
    
    //Serial.println(pTarget_angle);
    //Serial.println(pAngle);
  }
  
  
  // calculating angle from counters by scaling it
  pAngle = (pCounter % pNum_ticks) / pDivider;
  tAngle = (tCounter % tNum_ticks) / tDivider;
  //pTarget_angle = map(analogRead(pPot),0,1023,0,30);
  //tTarget_angle = map(analogRead(tPot),0,1023,0,30);

  // calculating and setting error terms
  pError_p = pTarget_angle - pAngle;    
  tError_p = tTarget_angle - tAngle;

  
  pVelocity = pError_p*pkp;
  if (pVelocity > maxVelocity) {
      pVelocity = maxVelocity;
    }
  
    if (pVelocity < -maxVelocity) {
      pVelocity = -maxVelocity;
    }    
    md.setM1Speed(pVelocity); 
  
  
  tVelocity = tError_p*tkp; // + pError_i*pki;
  
    if (tVelocity > maxVelocity) {
      tVelocity = maxVelocity;
    }
  
    if (tVelocity < -maxVelocity) {
      tVelocity = -maxVelocity;
    }    
    md.setM2Speed(tVelocity); 
  

  time = millis();
  if (time - prev_time > 500) {
    Serial.println(pTarget_angle);
    Serial.println(pAngle);
    Serial.println(pVelocity);
    Serial.println(tTarget_angle);
    Serial.println(tAngle);
    Serial.println(tVelocity);
    Serial.println("");
    prev_time = time;
  }
  if ( (time-fireTime > fireDelay) && toFire) {
    toFire = LOW;
    //load();
    fire();
  }
  
}

void ISR_pan()
{
  if (digitalRead(p_pin) == digitalRead(p_pin_B)) {
    pCounter++;
  } else {
    pCounter--;
  }
}

void ISR_tilt()
{
  if (digitalRead(t_pin) == digitalRead(t_pin_B)) {
    tCounter++;
  } else {
    tCounter--;
  }
}

//void load() 
//{ 
//  for(pos = 544; pos < 2400; pos += 10)  // goes from 0 degrees to 180 degrees 
//  {                                  // in steps of 1 degree 
//    loadServo.write(pos);              // tell servo to go to position in variable 'pos' 
//    delay(10);                       // waits 15ms for the servo to reach the position 
//  }
//  for(pos = 2400; pos>= 544; pos -= 10)     // goes from 180 degrees to 0 degrees 
//  {                                
//    loadServo.write(pos);               // tell servo to go to position in variable 'pos' 
//    delay(10);                       // waits 15ms for the servo to reach the position 
//  }
//  delay(200);
//}

void fire() 
{
  digitalWrite(marker, HIGH);
  delay(2);
  digitalWrite(marker, LOW);
}

