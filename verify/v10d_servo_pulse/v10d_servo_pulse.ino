const int CDS = A2;
const int LED = 5;

void setup() {
  pinMode(LED, OUTPUT);
}

void loop() {
  int t = analogRead(CDS) / 4;   // 明るいほど長い
  digitalWrite(LED, HIGH); delay(t);
  digitalWrite(LED, LOW);  delay(t);
}