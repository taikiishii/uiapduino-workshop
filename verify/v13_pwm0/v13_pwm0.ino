/*
 * v13_pwm0  ---  検証⑬：0番ピンで analogWrite（PWM）が効くか
 *
 * なぜ確かめるのか
 *   e5 のモーターが「リセット後2秒だけ回って、そのあと回らない」。
 *   loop() は 正転(AIN1=12 に analogWrite) → 逆転(AIN2=0 に analogWrite)
 *   → 停止 のくりかえしなので、**0番の analogWrite が効いていない**と
 *   ちょうどこの見え方になる（正転だけ回り、逆転は LOW のまま）。
 *
 *   v04 で **1番(PA2)・10番(PD0) は analogWrite が無反応**と実測ずみ。
 *   どちらも TIM1 の相補出力（CH2N/CH1N）で、MOE ビットを立てないと
 *   端子に出ないため。**0番(PA1) は未検証**のまま残っていた。
 *   0番も同じ TIM1 まわりなので、同じ不具合の可能性がある。
 *
 * つなぎかた
 *   テストLED（LED＋330Ω を直列にしたもの）を用意して、
 *     抵抗がわ … GND
 *     LEDがわ … 調べたいピン
 *   ※ モーターは外しておくこと。
 *
 * 見かた（1本ずつ 約6秒。順番に3本くりかえす）
 *   基板のオレンジLED（2番）が **短く n回** 光って、いまどのピンかを知らせる。
 *     1回 … 12番（A3）  … 効くはず（v04 で実測ずみ）
 *     2回 …  0番（A1）  … ★ここが焦点
 *     3回 …  6番（A2）  … 効くはず（v04 で実測ずみ）
 *   そのあと、そのピンが **2秒かけて ふわっと明るくなる**。
 *     ふわっと明暗する → PWM が効いている ◎
 *     パッと点くだけ   → PWM が効いていない（digitalWrite と同じ）
 *     まったく光らない → analogWrite が無反応（1番・10番と同じ不具合）
 */

const int LED  = 2;                 // 基板のオレンジLED（知らせる係）
const int PINS[] = {12, 0, 6};      // 調べるピン
const int N = 3;

void shirase(int n) {               // いまどのピンかを知らせる
  for (int i = 0; i < n; i++) {
    digitalWrite(LED, HIGH); delay(150);
    digitalWrite(LED, LOW);  delay(250);
  }
  delay(600);
}

void setup() {
  pinMode(LED, OUTPUT);
  analogWriteResolution(8);
  for (int i = 0; i < N; i++) pinMode(PINS[i], OUTPUT);
}

void loop() {
  for (int i = 0; i < N; i++) {
    shirase(i + 1);
    for (int v = 0; v <= 255; v += 5) {   // 2秒かけて ふわっと
      analogWrite(PINS[i], v);
      delay(40);
    }
    digitalWrite(PINS[i], LOW);
    delay(800);
  }
}
