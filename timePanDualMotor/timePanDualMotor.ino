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

// Create the motor shield object with the default I2C address
DualMC33926MotorShield md;

// Encoder stuff (counters and ticks)
volatile int pCounter;
volatile int tCounter;
const byte pDivider = pNum_ticks / 360.;
const float tDivider = tNum_ticks / 360.;
const byte marker = 4;

// Angle measures
float pAngle;
float tAngle;
float pTarget_angle = 0;
float tTarget_angle = 0;

// Printing timing
double time;
double prev_time;

// Motor control
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
  // enable internal pullup resistors
  digitalWrite(p_pin, 1);
  digitalWrite(t_pin, 1);
  digitalWrite(p_pin_B, 1);
  digitalWrite(t_pin_B, 1);
  // using encoders with interrupts
  attachInterrupt(0, ISR_pan, CHANGE);
  attachInterrupt(1, ISR_tilt, CHANGE);
  digitalWrite(marker,LOW);
  // Start motor shield
  md.init();
  
  boolean zero = LOW;
  boolean tzero = HIGH;
  boolean pzero = HIGH;
  while (!zero) {
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

String incoming = "0";

void loop() {
  
   if (Serial.available() > 0) { //check for new data
      incoming = Serial.readString();
      Serial.println(incoming);
      if (incoming == "1") {
        digitalWrite(marker, HIGH);
        delay(10);
        digitalWrite(marker, LOW);
        incoming = "0";
      }
   }
  
  
  // calculating angle from counters by scaling it
  pAngle = (pCounter % pNum_ticks) / pDivider;
  tAngle = (tCounter % tNum_ticks) / tDivider;
  pTarget_angle = map(analogRead(pPot),0,1023,0,30);
  tTarget_angle = map(analogRead(tPot),0,1023,0,30);

  // calculating and setting error terms
  pError_p = pTarget_angle - pAngle;    
  tError_p = tTarget_angle - tAngle;

  
  pVelocity = pError_p*pkp;
  if (pVelocity > 400) {
      pVelocity = 400;
    }
  
    if (pVelocity < -400) {
      pVelocity = -400;
    }    
    md.setM1Speed(pVelocity); 
  
  
  tVelocity = tError_p*tkp; // + pError_i*pki;
  
    if (tVelocity > 400) {
      tVelocity = 400;
    }
  
    if (tVelocity < -400) {
      tVelocity = -400;
    }    
    md.setM2Speed(tVelocity); 
  

  time = millis();
  if (time - prev_time > 500) {
    Serial.println(tTarget_angle);
    Serial.println(tAngle);
    Serial.println(tVelocity);
    Serial.println("");
    prev_time = time;
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

