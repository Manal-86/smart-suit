const int emgPin = 35; // Analog pin connected to EMG sensor

void setup() {
  Serial.begin(115200);
}

void loop() {
  int emgValue = analogRead(emgPin);
  Serial.println(emgValue);
  delay(100); // Adjust delay as needed
}

