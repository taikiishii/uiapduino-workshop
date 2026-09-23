const int SENSOR = A2;
const int LED    = 5;

void setup() {
  pinMode(LED, OUTPUT);
}

void loop() {
  int v = analogRead(SENSOR);
  float volt = v * 5.0 / 1024;
  float t = (volt - 0.5) * 100;
  digitalWrite(LED, t > 38 ? HIGH : LOW);
  delay(200);
}