/*
 * v10b_servo  ---  検証⑩：サーボモーターをライブラリ無しで動かす
 *
 * わかっていること
 *   ・Servo ライブラリはこのボードに**同梱されていない**
 *   ・analogWriteFrequency() は効く（v10a で実測）
 *   → 50Hz のハードウェアPWMを出せば、ライブラリ無しでサーボを動かせるはず。
 *
 * サーボの決まりごと
 *   20ms ごとに、1.0〜2.0ms の長さのパルスを送ると、その長さで角度が決まる。
 *     1.0ms → 0度 ／ 1.5ms → 90度 ／ 2.0ms → 180度
 *
 * PWM の値への直しかた（分解能は既定の 12bit ＝ 0〜4095）
 *   50Hz なら 1周期は 20ms。1.0ms は 20ms の 5%     → 4095 × 0.05  ≒ 205
 *                          1.5ms は 7.5%           → 4095 × 0.075 ≒ 307
 *                          2.0ms は 10%            → 4095 × 0.10  ≒ 410
 *   ※ analogWriteResolution() は呼ばない（既定の 12bit のまま使う）
 *
 * つなぎかた（SG90 など）
 *   茶または黒 … GND
 *   赤         … 5V
 *   黄または橙 … 5番ピン
 *
 * ⚠ サーボは電気をたくさん使う。動かした瞬間にボードが再起動する（LEDが消えて
 *   やり直しになる）ときは、電源が足りていない。その場合は乾電池など**別の電源**から
 *   サーボに給電し、GND だけをボードとつなぐ。
 *
 * 動き（基板のオレンジLEDが今の角度を知らせる）
 *   短く1回 → 0度   ／ 2回 → 90度 ／ 3回 → 180度 ／ 2回 → 90度 … のくりかえし
 *
 * 見かた
 *   3つの位置にきちんと止まり、90度がまんなかに見える → ✅ 成功
 *   ふるえる・うなる → パルスが不安定か、電源が足りない
 *   まったく動かない → 配線（とくに信号線とGND）を確認
 */

const int SERVO = 5;     // PWM が使えるピン（実測ずみ）
const int LED   = 2;

// 角度（0〜180）を PWM の値に直す
int kakudoToPwm(int kakudo) {
  return map(kakudo, 0, 180, 205, 410);
}

void blinkN(int n) {
  for (int i = 0; i < n; i++) {
    digitalWrite(LED, HIGH); delay(120);
    digitalWrite(LED, LOW);  delay(220);
  }
}

void moveTo(int kakudo, int n) {
  blinkN(n);
  analogWrite(SERVO, kakudoToPwm(kakudo));
  delay(1500);
}

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(SERVO, OUTPUT);
  analogWriteFrequency(50);      // ★ サーボは 50Hz
}

void loop() {
  moveTo(0,   1);     // 0度
  moveTo(90,  2);     // 90度
  moveTo(180, 3);     // 180度
  moveTo(90,  2);     // 90度にもどる
}
