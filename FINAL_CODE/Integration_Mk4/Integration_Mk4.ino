#include "DualMC33926MotorShield.h"

#define p_pin 2
#define t_pin 3
#define p_pin_B 5
#define t_pin_B 6
#define pNum_ticks 720
#define tNum_ticks 1620
#define marker 11
#define tlswitch 13
#define plswitch A2

// Create the motor shield object with the default I2C address
DualMC33926MotorShield md;

// Encoder stuff (counters and ticks)
volatile int pCounter;
volatile int tCounter;

// For converting from encoder ticks to angles
const byte pDivider = pNum_ticks / 360.;
const float tDivider = tNum_ticks / 360.;

// Angle measures
float pAngle;
float tAngle;
float pTarget_angle;
float tTarget_angle;

// PySerial stuff
String incoming = "";
String panString = "";
String tiltString = "";

// Motor control
int maxVelocity = 250;
double pError_p;
double tError_p;
const float pkp = 130;
const float tkp = 230;

// Motor velocities
double pVelocity;
double tVelocity;

//marker control
double time;
long fireTime = 0;
int fireDelay = 2000;
boolean toFire = LOW;

void setup() {
  Serial.begin(9600);
  pinMode(p_pin, INPUT);
  pinMode(t_pin, INPUT);
  pinMode(p_pin_B, INPUT);
  pinMode(t_pin_B, INPUT);
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
  // Start motor shield
  md.init();
  
  // Allow pan and tilt to hit our limit switches for zeroing
  boolean zero = LOW;
  boolean tzero = HIGH;
  boolean pzero = HIGH;
  while (!zero) {
    if (pzero) {
       md.setM1Speed(-300);
       pzero = digitalRead(plswitch);
    } else {
      md.setM1Speed(0);
    }
    if (tzero) {
       md.setM2Speed(-300);
       tzero = digitalRead(tlswitch);
    } else {
      md.setM2Speed(0);
    }
    if (!tzero && !pzero) {
      zero = HIGH;
    }
  } 
  
  // Reset counters after zeroing
  pCounter=0;
  tCounter=0;
  
  //tell PySerial you're ready to go
  Serial.print("Initialized!");
}

void loop() {
  if (Serial.available() > 0) {
    // check for new data
    incoming = Serial.readString();
    // decode pan and tilt angles from serial
    panString = incoming.substring(0,3);
    tiltString = incoming.substring(3,6);
    pTarget_angle = panString.toInt() / 10.;
    tTarget_angle = tiltString.toInt() / 10.;
    
    // start fire timer
    fireTime = millis();
    toFire = HIGH;
    
    Serial.println(pTarget_angle);
    Serial.println(pAngle);
    Serial.println(pVelocity);
    Serial.println(tTarget_angle);
    Serial.println(tAngle);
    Serial.println(tVelocity);
  }
  
  
  // calculating angle from counters by scaling it
  pAngle = (pCounter % pNum_ticks) / pDivider;
  tAngle = (tCounter % tNum_ticks) / tDivider;

  // calculating and setting error terms
  pError_p = pTarget_angle - pAngle;    
  tError_p = tTarget_angle - tAngle;
  
  // Calculating velocities and setting max and min thresholds
  pVelocity = pError_p*pkp;
  if (pVelocity > maxVelocity) {
    pVelocity = maxVelocity;
  }
  if (pVelocity < -maxVelocity) {
    pVelocity = -maxVelocity;
  }    
  md.setM1Speed(pVelocity); 
  
  tVelocity = tError_p*tkp;
  if (tVelocity > maxVelocity) {
    tVelocity = maxVelocity;
  }
  if (tVelocity < -maxVelocity) {
    tVelocity = -maxVelocity;
  }    
  md.setM2Speed(tVelocity);

  time = millis();
  // If conditions are met, call fire() to fire the marker
  if ( (time-fireTime > fireDelay) && toFire) {
    toFire = LOW;
    fire();
  }
}

// Interrupt sequences to increment counters on CHANGE of encoder pins
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

void fire() 
{
  digitalWrite(marker, HIGH);
  delay(1);
  digitalWrite(marker, LOW);
}
