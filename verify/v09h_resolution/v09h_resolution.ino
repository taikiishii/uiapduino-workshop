/*
 * v09h_resolution  ---  micros() の「刻み幅」と、長いパルスなら測れるかを調べる
 *
 * v09g の結果：信号は digitalRead で読めている（②が2回）のに、
 * pulseIn も自前の測り方も 0 を返した（③が出ない）。
 *
 * 仮説：micros() が 1000us 刻みでしか進まない。
 *   測っていたのは 500us のパルスなので、差が 0 になり「失敗」と判定されていた。
 *   → 刻み幅を直接測り、さらに **50ms の長いパルス** なら測れるかを確かめる。
 *
 * つなぎかた（センサーは外したまま）
 *   5番ピン ──ジャンパー線1本── 7番ピン
 *
 * 1周の流れ（約8秒）
 *   ①【長い点灯 0.7秒】ハートビート
 *   ②【短い点滅】micros() の刻み幅
 *        1回 … 1us 刻み（理想）
 *        2回 … 2〜9us
 *        3回 … 10〜99us
 *        4回 … 100〜999us
 *        5回 … 1000us 以上 ← 仮説どおり（短いパルスは測れない）
 *   ③【短い点滅】10Hz の PWM（HIGH は 50ms）を測った結果 ÷ 10ms
 *        5回前後 … ✅ 測れている（長いパルスなら問題なし）
 *        なし    … 長いパルスでも測れない（別の原因）
 *
 * この結果でわかること
 *   ②が5回・③が5回 → micros() の分解能が 1ms。HC-SR04 の距離は
 *     1ms ≒ 17cm きざみでしか測れないことになり、e3 の作り方を変える必要がある。
 *   ②が1〜3回      → 分解能は十分。原因は別のところにある。
 */

const int OUT_PIN = 5;
const int IN_PIN  = 7;
const int LED     = 2;

void blinkN(int n) {
  for (int i = 0; i < n; i++) {
    digitalWrite(LED, HIGH);
    delay(120);
    digitalWrite(LED, LOW);
    delay(220);
  }
}

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(IN_PIN, INPUT);
  pinMode(OUT_PIN, OUTPUT);
  analogWriteResolution(8);
  analogWriteFrequency(10);       // 10Hz ＝ 周期100ms、HIGH は 50ms
  analogWrite(OUT_PIN, 128);
}

void loop() {
  // ① ハートビート
  digitalWrite(LED, HIGH);
  delay(700);
  digitalWrite(LED, LOW);
  delay(600);

  // ② micros() の刻み幅を測る（値が変わるまで読み続ける）
  unsigned long m0 = micros();
  unsigned long m1 = m0;
  while (m1 == m0) m1 = micros();
  unsigned long step = m1 - m0;

  int s;
  if (step <= 1)        s = 1;
  else if (step <= 9)   s = 2;
  else if (step <= 99)  s = 3;
  else if (step <= 999) s = 4;
  else                  s = 5;
  blinkN(s);
  delay(1000);

  // ③ 50ms の長いパルスを pulseIn で測る
  unsigned long t = pulseIn(IN_PIN, HIGH, 300000);   // 待ち時間は 300ms に延長
  int n = t / 10000;               // 10ms を1回として知らせる（期待は 5）
  if (n > 9) n = 9;
  if (n > 0) blinkN(n);

  delay(2500);
}
