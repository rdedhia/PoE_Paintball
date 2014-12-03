#include "DualMC33926MotorShield.h"

#define A_pin 2
#define B_pin 3
#define num_ticks 1440

int motorspeed = 0;
float angle;
volatile int counter;
volatile int counterA;
volatile int counterB;
volatile int counterC; 
double time;
double ptime;
float kp = 2;
float ki = 0.005;
float target_angle = 90;

// initiate the motor shield
DualMC33926MotorShield md;

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
}

void loop() {
  angle = (counter % num_ticks) / 4.; 
  md.setM1Speed(200);
  
  time = millis();
  if (time - ptime > 500) {
    Serial.println(counter);
    Serial.println(counterA);
    Serial.println(counterB);
    Serial.println(counterC);
    Serial.println("");
    ptime = time;
  }
}

void ISRchanA()
{
  if (digitalRead(A_pin) == digitalRead(B_pin)) {
    counter++;
  } else {
    counterA--;
  }
}

void ISRchanB()
{
  if (digitalRead(A_pin) == digitalRead(B_pin)) {
    counterB--;
  } else {
    counterC++;
  }
}

