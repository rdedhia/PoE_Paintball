// Include necessary modules for Adafruit motor shield
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"

#define p_pin 2
#define t_pin 3
#define p_pin_B 4
#define t_pin_B 5
#define pNum_ticks 720
#define tNum_ticks 1620
#define tPot A1
#define pPot A2

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

// Select which 'port' M1, M2, M3 or M4. In this case, M1
Adafruit_DCMotor *panMotor = AFMS.getMotor(1);
Adafruit_DCMotor *tiltMotor = AFMS.getMotor(2);

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
  // enable internal pullup resistors
  digitalWrite(p_pin, 1);
  digitalWrite(t_pin, 1);
  digitalWrite(p_pin_B, 1);
  digitalWrite(t_pin_B, 1);
  // using encoders with interrupts
  attachInterrupt(0, ISR_pan, CHANGE);
  attachInterrupt(1, ISR_tilt, CHANGE);
  // Start motor shield
  AFMS.begin();
  //tell PySerial you're ready to go
  Serial.print("Initialized!");
}

void loop() {
  // calculating angle from counters by scaling it
  pAngle = (pCounter % pNum_ticks) / pDivider;
  tAngle = (tCounter % tNum_ticks) / tDivider;
  pTarget_angle = map(analogRead(pPot),0,255,0,15);
  tTarget_angle = map(analogRead(tPot),0,255,0,100);

  // calculating and setting error terms
  pError_p = pTarget_angle - pAngle;
  //  pError_i += pError_p;
  //  if (pError_i > 1000) {
  //    pError_i = 1000;
  //  }
    
    tError_p = tTarget_angle - tAngle;
  //  tError_i += tError_p;
  //  if (tError_i > 1000) {
  //    tError_i = 1000;
  //  }
  
  // calculating velocities and running motors from error terms
  pVelocity = pError_p*pkp; // + pError_i*pki;
  if (pVelocity > 0) {
    if (pVelocity > 255) {
      pVelocity = 255;
    }
    panMotor->run(FORWARD);
    panMotor->setSpeed(pVelocity);
  } else {
    if (pVelocity < -255) {
      pVelocity = -255;
    }    
    panMotor->run(BACKWARD);
    panMotor->setSpeed(-1*pVelocity); 
  }
  
  tVelocity = tError_p*tkp; // + pError_i*pki;
  if (tVelocity > 0) {
    if (tVelocity > 255) {
      tVelocity = 255;
    }
    tiltMotor->run(FORWARD);
    tiltMotor->setSpeed(tVelocity);
  } else {
    if (tVelocity < -255) {
      tVelocity = -255;
    }    
    tiltMotor->run(BACKWARD);
    tiltMotor->setSpeed(-1*tVelocity); 
  }

  time = millis();
  if (time - prev_time > 500) {
    Serial.println(pAngle);
    Serial.println(pError_p); 
    Serial.println(pVelocity);
    Serial.println(tAngle);
    Serial.println(tError_p);
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

