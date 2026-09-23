const int AIN1 = 12;  // どちらも PWM が使えるピン
const int AIN2 = 0;

void setup() {
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  analogWriteResolution(8);
}

void loop() {
  analogWrite(AIN1, 200);  digitalWrite(AIN2, LOW);   // 正転
  delay(2000);
  digitalWrite(AIN1, LOW); analogWrite(AIN2, 200);    // 逆転
  delay(2000);
  digitalWrite(AIN1, LOW); digitalWrite(AIN2, LOW);   // 停止
  delay(1000);
}