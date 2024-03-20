
int dist;
int pin = A0;

void setup() {
  pinMode(pin, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  dist = analogRead(pin);
  Serial.println(dist);
  if (dist<512){
    digitalWrite(LED_BUILTIN, LOW);
  }
  else{
    digitalWrite(LED_BUILTIN, HIGH);
  }
  delay(100);
}
