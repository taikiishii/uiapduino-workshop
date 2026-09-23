const int CDS = A2;
const int LED = 5;

void setup() {
  pinMode(LED, OUTPUT);
  analogWriteResolution(8);
}

void loop() {
  int v = analogRead(CDS);
  digitalWrite(LED, HIGH); delay(v / 4);
  digitalWrite(LED, LOW);  delay(v / 4);
}
