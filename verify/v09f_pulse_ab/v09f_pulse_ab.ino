/*
 * v09f_pulse_ab  ---  切り分け（決定版）：pulseIn と「自前の測り方」を同じ信号で比べる
 *
 * これ1本で次のすべてが分かる。
 *   ・7番ピンが入力として読めるか（自前の測り方が digitalRead を使うため）
 *   ・pulseIn() が使えるか
 *   ・使えないのはどちらか／両方か
 *   ・測れた値が妥当か（500us 前後のはず）
 *
 * つなぎかた（センサーは外したまま）
 *   5番ピン ──ジャンパー線1本── 7番ピン
 *
 * 見かた（2組の点滅がセットで出る）
 *   【1組目】どの方法が動いたか
 *       1回 … pulseIn() だけ動いた
 *       2回 … 自前の測り方だけ動いた（＝ pulseIn が使えない。入力は正常）
 *       3回 … 両方動いた ◎
 *       点かない（3秒消灯）… どちらも動かない（＝7番が入力として読めていない）
 *   【2組目】測れた長さ ÷ 100マイクロ秒
 *       5回前後 … 妥当（1000Hz・デューティ50% ＝ HIGH 500us）
 *
 *   1組目 → 1秒あき → 2組目 → 2.5秒あき、のくりかえし。
 */

const int OUT_PIN = 5;
const int IN_PIN  = 7;
const int LED     = 2;

// pulseIn と同じことを digitalRead と micros() だけで行う
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
    delay(200);
  }
}

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(IN_PIN, INPUT);
  pinMode(OUT_PIN, OUTPUT);
  analogWriteResolution(8);
  analogWrite(OUT_PIN, 128);      // 1000Hz・50% の方形波を出しっぱなし
}

void loop() {
  unsigned long a = pulseIn(IN_PIN, HIGH, 30000);   // 標準の方法
  unsigned long b = manualPulse(IN_PIN, 30000);     // 自前の方法

  int code = 0;
  if (a > 0) code += 1;
  if (b > 0) code += 2;

  if (code == 0) {                 // どちらも失敗
    digitalWrite(LED, LOW);
    delay(3000);
    return;
  }

  blinkN(code);                    // 1組目：どれが動いたか
  delay(1000);

  unsigned long v = (b > 0) ? b : a;
  int n = v / 100;                 // 2組目：測れた長さ
  if (n < 1) n = 1;
  if (n > 9) n = 9;
  blinkN(n);

  delay(2500);
}
