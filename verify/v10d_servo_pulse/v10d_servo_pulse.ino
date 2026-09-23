const int CDS = A2;
const int LED = 5;    // 基板のオレンジLED

void setup() {
  pinMode(LED, OUTPUT);
}

void loop() {
  int n = analogRead(CDS) / 100;
  for (int i = 0; i < n; i++) {
    digitalWrite(LED, HIGH); delay(200);
    digitalWrite(LED, LOW);  delay(300);
  }
  delay(2000);        // ひと休み
}