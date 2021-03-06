#include "DualMC33926MotorShield.h"

#define A_pin 2
#define B_pin 3
#define num_ticks 1440

DualMC33926MotorShield md;

float angle;
volatile int counter;
double time;
double ptime;
double velocity = 0;
double error_p;
double error_i;
float kp = 28;
float ki = 0;
float target_angle = 180;

void setup() {
  Serial.begin(9600);
  pinMode(A_pin, INPUT);
  pinMode(B_pin, INPUT);
  // enable internal pullup resistors
  digitalWrite(A_pin, 1);
  digitalWrite(B_pin, 1);
  // using encoders
  attachInterrupt(0, ISRchanA, CHANGE);
  attachInterrupt(1, ISRchanB, CHANGE);
  // Start motor shield
  md.init();
  Serial.print("Initialized!");
}

void loop() {
  angle = (counter % num_ticks) / 4.;
  error_p = target_angle - angle;
  error_i += error_p;
  if (error_i > 1000) {
    error_i = 1000;
  }
  
  velocity = error_p*kp + error_i*ki;
 
  if (velocity >= 0) {
    if (velocity > 400) {
      velocity = 400;
    }
    md.setM1Speed(velocity);
  } else {
    if (velocity < -400) {
      velocity = -400;
    }    
    md.setM1Speed(velocity);
  }
  
  time = millis();
  if (time - ptime > 500) {
    Serial.println(angle);
    Serial.println(error_p*kp);
    Serial.println(error_i*ki);
    Serial.println(velocity);
    Serial.println("");
    ptime = time;
  }
}

void ISRchanA()
{
  if (digitalRead(A_pin) == digitalRead(B_pin)) {
    counter++;
  } else {
    counter--;
  }
}

void ISRchanB()
{
  if (digitalRead(A_pin) == digitalRead(B_pin)) {
    counter--;
  } else {
    counter++;
  }
}

