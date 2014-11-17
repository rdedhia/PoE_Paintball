// Include necessary modules for Adafruit motor shield
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"

#define A_pin 2
#define B_pin 3
#define num_ticks 1440

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

// Select which 'port' M1, M2, M3 or M4. In this case, M1
Adafruit_DCMotor *panMotor = AFMS.getMotor(1);

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
  AFMS.begin();
  //tell PySerial you're ready to go
  Serial.print("Initialized!");
}

void loop() {
  angle = (counter % num_ticks) / 4.; 
  panMotor->run(BACKWARD);  
  panMotor->setSpeed(100); 
  
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

