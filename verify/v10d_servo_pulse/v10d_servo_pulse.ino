const int SENSOR = A2;
const int LED    = 5;

void setup() {
  pinMode(LED, OUTPUT);
}

void loop() {
  int v = analogRead(SENSOR);
  float mv = v * 5000.0 / 1024;
  float t = (mv - 400) / 19.5;
  digitalWrite(LED, t > 28 ? HIGH : LOW);
  delay(200);
}