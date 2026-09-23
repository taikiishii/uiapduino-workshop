const int CDS = A2;
const int LED = 5;

void setup() {
  pinMode(LED, OUTPUT);
  analogWriteResolution(8);
}

void loop() {
  analogWrite(LED, analogRead(CDS) / 4);
  delay(10);
}