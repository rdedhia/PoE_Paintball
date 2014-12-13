const byte trigger_pin = 11; // Pin number for transistor

void setup()  {
  Serial.begin(9600);
  pinMode(trigger_pin, OUTPUT);
  
  digitalWrite(trigger_pin, HIGH);
  delay(2);
  digitalWrite(trigger_pin, LOW);
}

void loop()  {
}
  
