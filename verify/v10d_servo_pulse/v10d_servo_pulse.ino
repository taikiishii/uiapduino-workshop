const int AIN1 = 12;  // どちらも PWM が使えるピン
const int AIN2 = 6;

void setup() {
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  analogWriteResolution(8);
}

void loop() {
  analogWrite(AIN1, 200);  analogWrite(AIN2, 0);   // 正転
  delay(2000);
  analogWrite(AIN1, 0);    analogWrite(AIN2, 200); // 逆転
  delay(2000);
  analogWrite(AIN1, 0);    analogWrite(AIN2, 0);   // 停止
  delay(1000);
}