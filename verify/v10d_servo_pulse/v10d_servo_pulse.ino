const int CDS = A2;
const int LED = 5;

void setup() {
  pinMode(LED, OUTPUT);
}

void loop() {
  if (analogRead(CDS) < 500) {
    digitalWrite(LED, HIGH);   // 暗い → 光る
  } else {
    digitalWrite(LED, LOW);
  }
  delay(50);
}