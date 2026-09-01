/*
 * v10d_servo_pulse  ---  サーボをパルス自作で動かす（本命）
 *
 * v10b（ハードウェアPWM）では可動範囲がほとんど出なかった。
 * PWM の分解能・周波数の効き方が読みきれないため、
 * **自分でパルスを作る**方式に切りかえる。
 *
 * 使うのは digitalWrite() と delayMicroseconds() だけ。
 * どちらも実機で動作確認ずみなので、確実性が高い。
 *
 * サーボの決まりごと（これがそのままコードになる）
 *   20ms ごとに、1.0〜2.0ms の長さのパルスを送る
 *     1.0ms（1000us）→ 0度
 *     1.5ms（1500us）→ 90度
 *     2.0ms（2000us）→ 180度
 *   1回送っただけでは動かないので、**送り続ける**のがコツ。
 *
 * つなぎかた（SG90 など）
 *   茶または黒 … GND ／ 赤 … 5V ／ 黄または橙 … 5番ピン
 *
 * ⚠ 動いた瞬間にボードが再起動するときは電源不足。
 *   乾電池などからサーボに給電し、GND だけボードとつなぐ。
 *
 * 動き（基板のオレンジLEDが今の角度を知らせる）
 *   1回 → 0度 ／ 2回 → 90度 ／ 3回 → 180度 ／ 2回 → 90度 … のくりかえし
 *   角度ごとに約1秒間パルスを送り続ける。
 *
 * 見かた
 *   0度と180度で**大きく**向きが変わる → ✅ 成功。この方式を教材に採用する
 *   まだ範囲がせまい → 1000/2000 の数字を 700/2300 くらいに広げて試す
 *   ふるえ続ける     → 電源不足の可能性
 */

const int SERVO = 5;
const int LED   = 2;

void blinkN(int n) {
  for (int i = 0; i < n; i++) {
    digitalWrite(LED, HIGH); delay(120);
    digitalWrite(LED, LOW);  delay(220);
  }
}

// パルスを1回送る（これを何回もくりかえす）
void servoPulse(int us) {
  digitalWrite(SERVO, HIGH);
  delayMicroseconds(us);
  digitalWrite(SERVO, LOW);
  delay(20);                    // 20ms ごと ＝ 1秒間に50回
}

// 指定した角度へ動かす（約1秒ぶん送り続ける）
void moveTo(int kakudo, int n) {
  blinkN(n);
  int us = map(kakudo, 0, 180, 600, 2400);
  for (int i = 0; i < 50; i++) {
    servoPulse(us);
  }
}

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(SERVO, OUTPUT);
  digitalWrite(SERVO, LOW);
}

void loop() {
  moveTo(0,   1);
  moveTo(90,  2);
  moveTo(180, 3);
  moveTo(90,  2);
}
