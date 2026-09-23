const int TRIG = 9;
const int ECHO = 7;
const int LED  = 5;

int kyori() {                    // きょりを cm ではかる
  digitalWrite(TRIG, HIGH); delayMicroseconds(10); digitalWrite(TRIG, LOW);
  int w = 0;
  while (digitalRead(ECHO) == LOW) { if (++w > 600) return 999; }
  int n = 0;
  while (digitalRead(ECHO) == HIGH && n < 600) {
    delayMicroseconds(50);       // 実測で 1回 ＝ 1cm
    n++;
  }
  return n - 2;                  // 数えた回数 ＝ ほぼ cm
}

void setup() {
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(LED, OUTPUT);
}
void loop() {
  digitalWrite(LED, kyori() < 20 ? HIGH : LOW);
  delay(100);
}