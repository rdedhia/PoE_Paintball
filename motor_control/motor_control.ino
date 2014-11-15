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
Adafruit_DCMotor *myMotor = AFMS.getMotor(1);

int motorspeed = 0;
float angle;
volatile int counter;
double time;
double ptime;
double velocity;
double error_p;
double error_i;
float kp = 2;
float ki = .01;
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
  AFMS.begin();
  //tell PySerial you're ready to go
  Serial.print("Initialized!");
}

void loop() {
  angle = (counter % num_ticks) / 4.;
  error_p = target_angle - angle;
  error_i += error_p;
  velocity = error_p*kp + error_i*ki;
  if (velocity > 0) {
    if (velocity > 200) {
      velocity = 200;
    }
    myMotor->run(FORWARD);
    myMotor->setSpeed(velocity);
  } else {
    if (velocity < -200) {
      velocity = -200;
    }    
    myMotor->run(BACKWARD);
    myMotor->setSpeed(abs(velocity)); 
  }
  
  time = millis();
  if (time - ptime > 500) {
    Serial.println(angle);
    Serial.println(error_p);
    Serial.println(error_i);
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

