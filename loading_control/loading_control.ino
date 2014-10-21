#include <Servo.h>

// Creating Servo objects to easily write angles to the servos
Servo myservo;
const byte servo_pin = 9; // Pin number for servo

const int red = 0;
const int blue = 120;
const int green = 240;
int angle; // angle of servo

long previousMillis = 0; // Time of last movement
long currentMillis; // Current time, taken using millis()

// get one row of colors over PySerial as an array
// temporary array for testing
byte colors[8] = {0, 1, 2, 0, 2, 1, 1, 2};

void setup()  { 
  Serial.begin(9600);
  // Attaching Servo object to correct pin
  myservo.attach(servo_pin);
} 

void loop()  {
  // iterate over colors array
    if (color == 1) {
      angle = red;
    } else if (color == 2) {
      angle = blue;
    } else if (color == 3) {
      angle = green;
    }
}
  
