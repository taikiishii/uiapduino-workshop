const int SERVO = 5;

void servoPulse(int us) {        // パルスを1回おくる
  digitalWrite(SERVO, HIGH);
  delayMicroseconds(us);         // 600〜2400 で角度がきまる
  digitalWrite(SERVO, LOW);
  delay(20);                     // 20msごと ＝ 1秒に50回
}

void muku(int kakudo) {          // その角度をむく
  int us = map(kakudo, 0, 180, 600, 2400);
  for (int i = 0; i < 50; i++) servoPulse(us);
}

void setup() {
  pinMode(SERVO, OUTPUT);
}

void loop() {
  muku(0);      // 0度をむく
  muku(90);     // まんなか
  muku(180);    // 180度
}