#include "Servo.h"

Servo loadServo;

// Servo positions, which depend on color input
int pos;
int pos_min = 90;
int pos_max;

// Getting incoming color from serial
String incoming = "";
int color; // 0 is blue, 1 is orange, 2 is yellow

void setup() {
  Serial.begin(9600);
  loadServo.attach(9);
  loadServo.write(90);
  Serial.println("Servo Begin");
}

void loop() {
  if (Serial.available() > 0) {
    //check for new data
    incoming = Serial.readString();
    color = incoming.toInt();
    load(color);
  }
}

void load(int color)
{ 
  Serial.println("Colors Yay!");
  if (color == 1) {
    // Sweep from 90 to 0
    pos_max = 0;
    for (pos = pos_min; pos >= pos_max; pos -= 1)
    {                                
      loadServo.write(pos);
      delay(10);           
    }
  } else if (color == 2) {
    // Sweep from 90 to 180
    pos_max = 180;
    for (pos = pos_min; pos <= pos_max; pos += 1)
    {                                  
      loadServo.write(pos);            
      delay(10);                       
    }
  }
  // Return to position of 90
  loadServo.write(90);
}

