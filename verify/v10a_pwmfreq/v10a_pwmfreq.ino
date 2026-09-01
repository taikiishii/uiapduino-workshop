/*
 * v10a_pwmfreq  ---  analogWriteFrequency() が効くかを「目で見て」確かめる
 *
 * なぜ調べるか
 *   e4 サーボは **50Hz** のパルスを必要とする。
 *   Servo ライブラリはこのボードに同梱されていないので、
 *   analogWriteFrequency(50) ＋ analogWrite で作る案を考えていた。
 *   その前提が成り立つかを確認する。
 *
 * しくみ
 *   PWM の周波数を **1Hz** に設定し、デューティ 50% を出す。
 *   1Hz なら「0.5秒点灯・0.5秒消灯」で、**目で見て点滅が分かる**。
 *   既定の 1000Hz のままなら、速すぎて「うすく光りっぱなし」に見える。
 *   時間を測る関数を一切使わないので、micros() の不具合の影響を受けない。
 *
 * つなぎかた
 *   テストLED（330Ω付き）を **5番ピン と GND** のあいだに
 *   ※ 超音波センサーは外してよい
 *
 * 見かた
 *   テストLEDが **ゆっくり点滅**（1秒周期）
 *       → ✅ analogWriteFrequency() は効く。サーボは PWM で作れる
 *   テストLEDが **うすく光りっぱなし**
 *       → ❌ 効いていない（1000Hz のまま）。サーボは自前でパルスを作る
 *   テストLEDが消えたまま
 *       → 配線を確認
 *
 *   基板のオレンジLEDは3秒ごとに短く光る（動いているしるし）。
 *   テストLEDの点滅とは明らかに速さが違うので、取りちがえない。
 */

const int PWM_PIN = 5;
const int LED     = 2;

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(PWM_PIN, OUTPUT);
  analogWriteResolution(8);
  analogWriteFrequency(1);      // ★ 1Hz にする
  analogWrite(PWM_PIN, 128);    // デューティ 50%
}

void loop() {
  // ハートビート（3秒ごとに短く1回）
  digitalWrite(LED, HIGH);
  delay(100);
  digitalWrite(LED, LOW);
  delay(2900);
}
