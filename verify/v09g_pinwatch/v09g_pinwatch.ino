/*
 * v09g_pinwatch  ---  切り分け（決定版2）：ハートビート付きで、何が起きているか必ず見える
 *
 * v09f は「測定失敗」も「スケッチが動いていない」も同じ消灯になってしまい、
 * 区別がつかなかった。そこで **必ず出るハートビート** を先頭に置く。
 *
 * つなぎかた（センサーは外したまま）
 *   5番ピン ──ジャンパー線1本── 7番ピン
 *
 * 1周の流れ（ぜんぶで約6秒）
 *   ①【長い点灯 0.7秒】ハートビート。これが出れば**スケッチは動いている**
 *   ②【短い点滅】7番ピンを1000回読んだ結果
 *        1回 … ずっと LOW だった（信号が読めていない）
 *        2回 … HIGH と LOW の両方があった（＝信号は読めている ◎）
 *        3回 … ずっと HIGH だった
 *   ③【短い点滅】測定できたか
 *        1回 … pulseIn() だけ成功
 *        2回 … 自前の測り方だけ成功
 *        3回 … 両方成功 ◎
 *        なし … どちらも失敗
 *
 *   ②と③のあいだは1秒、③のあとは2.5秒あく。
 *
 * いちばん知りたいこと
 *   ②が「2回」なのに③が「なし」なら、digitalRead では読めているのに
 *   測定処理だけが失敗していることになる（さらに調べる）。
 *   ②が「1回」なら、そもそも digitalRead が信号を捉えられていない。
 */

const int OUT_PIN = 5;
const int IN_PIN  = 7;
const int LED     = 2;

unsigned long manualPulse(int pin, unsigned long timeout) {
  unsigned long t0 = micros();
  while (digitalRead(pin) == HIGH) { if (micros() - t0 > timeout) return 0; }
  while (digitalRead(pin) == LOW)  { if (micros() - t0 > timeout) return 0; }
  unsigned long s = micros();
  while (digitalRead(pin) == HIGH) { if (micros() - s > timeout) return 0; }
  return micros() - s;
}

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
  analogWrite(OUT_PIN, 128);      // 1000Hz・50% を出しっぱなし
}

void loop() {
  // ① ハートビート（必ず出る）
  digitalWrite(LED, HIGH);
  delay(700);
  digitalWrite(LED, LOW);
  delay(600);

  // ② 7番ピンを1000回読んで、HIGH と LOW の両方があるか見る
  int highs = 0;
  for (int i = 0; i < 1000; i++) {
    if (digitalRead(IN_PIN) == HIGH) highs++;
    delayMicroseconds(10);        // 約10ms かけて読む＝1000Hz なら10周期ぶん
  }
  int state = (highs == 0) ? 1 : ((highs == 1000) ? 3 : 2);
  blinkN(state);
  delay(1000);

  // ③ 2つの方法で測ってみる
  unsigned long a = pulseIn(IN_PIN, HIGH, 30000);
  unsigned long b = manualPulse(IN_PIN, 30000);
  int code = 0;
  if (a > 0) code += 1;
  if (b > 0) code += 2;
  if (code > 0) blinkN(code);

  delay(2500);
}
