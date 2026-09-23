/*
 * v17_pwmoff  ---  検証⑰：PWM を出したピンを、どうすれば止められるか
 *
 * 分かったこと（v16）
 *   `analogWrite(pin, 200)` を出したあと `digitalWrite(pin, LOW)` を呼んでも
 *   **出力が止まらない**。PWM が出っぱなしになる。
 *
 *   これで e5 の症状が説明できる：
 *     正転 analogWrite(AIN1,200)                  → 回る
 *     逆転 digitalWrite(AIN1,LOW) ← 効かない → AIN1 は 200 のまま
 *          analogWrite(AIN2,200)                  → AIN2 も 200
 *          両方 High 相当 ＝ 真理値表の「1 1」＝ **ブレーキ**
 *     停止 digitalWrite 両方 LOW ← 効かない → ブレーキのまま
 *     → 最初の正転のあと、ずっとブレーキで二度と回らない
 *
 * このスケッチがすること
 *   止め方を3とおり試して、**どれなら本当に消えるか**を見る。
 *
 * つなぎかた
 *   テストLED（LED＋330Ω）を 12番 と GND に1本。モーターは外す。
 *
 * 見かた（基板のオレンジLED が 短く n回 光って、いまどの止め方かを知らせる）
 *   どの段も「2秒 点ける → 2秒 止める」。**止めているあいだ 消えるか**を見る。
 *
 *   1回 … digitalWrite(LOW) で止める      … v16 で効かなかった方法
 *   2回 … analogWrite(0) で止める          ← 有力
 *   3回 … pinMode(OUTPUT) を入れてから digitalWrite(LOW)
 *
 *   消えた止め方が、教材で使うべき書き方。
 */

const int LED = 2;       // 基板のオレンジLED（知らせる係）
const int P   = 12;      // 調べるピン

void shirase(int n) {
  for (int i = 0; i < n; i++) {
    digitalWrite(LED, HIGH); delay(150);
    digitalWrite(LED, LOW);  delay(250);
  }
  delay(400);
}

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(P, OUTPUT);
  analogWriteResolution(8);
}

void loop() {
  shirase(1);                          // ① digitalWrite で止める
  analogWrite(P, 200); delay(2000);
  digitalWrite(P, LOW); delay(2000);   // ここで消えるか

  shirase(2);                          // ② analogWrite(0) で止める
  analogWrite(P, 200); delay(2000);
  analogWrite(P, 0);    delay(2000);   // ここで消えるか

  shirase(3);                          // ③ pinMode を入れ直してから止める
  analogWrite(P, 200); delay(2000);
  pinMode(P, OUTPUT); digitalWrite(P, LOW); delay(2000);
}
