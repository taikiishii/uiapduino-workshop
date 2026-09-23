/*
 * v16_pwm2  ---  検証⑯：2本のピンで analogWrite を交互に使えるか
 *
 * ここまでで分かっていること
 *   ・v14 … loop() は回りつづけている（マイコンは正常）
 *   ・v15 … **正転だけ**なら毎回きちんと回る（ドライバ・電池・モーターは正常）
 *   ・e5  … 正転→逆転→停止 をくりかえすと、最初の正転しか回らない。
 *           そのあいだも基板のLEDは点滅している＝ loop は生きている
 *
 * 立てた見当
 *   PWM が使えるのは 0・2・5・6・12 で、**どれも TIM1 の別チャネル**。
 *   同じタイマーを共有しているので、`analogWrite(0番)` を呼んだときに
 *   タイマーの設定が書きかわり、**12番の出力まで止まってしまう**のでは。
 *   v15 で 0番を一度も使わなかったから回りつづけた、と考えると
 *   両方の症状が1つの原因で説明できる。
 *
 * つなぎかた
 *   テストLED（LED＋330Ω）を **2本** 用意して
 *     1本目 … 12番 と GND
 *     2本目 …  0番 と GND
 *   ※ モーターとドライバは外しておく。
 *
 * 見かた（3秒ずつ、3つの段をくりかえす）
 *   基板のオレンジLED（2番）が 短く n回 光って、いまどの段かを知らせる。
 *
 *   1回 … 12番だけ analogWrite(200)
 *          → 12番のLEDが中くらいの明るさで点く
 *   2回 …  0番だけ analogWrite(200)
 *          → **0番のLEDが点くか**（点かなければ 0番は PWM が効かない）
 *   3回 … 12番を analogWrite(200) に **戻す**
 *          → ★ここが焦点。**12番のLEDがまた点くか**
 *             点かない → 0番を使うと 12番が死ぬ。見当どおり
 *             点く     → タイマーの取り合いではない。別の原因をさがす
 */

const int LED = 2;       // 基板のオレンジLED（知らせる係）
const int P1  = 12;      // e5 の AIN1
const int P2  = 0;       // e5 の AIN2

void shirase(int n) {
  for (int i = 0; i < n; i++) {
    digitalWrite(LED, HIGH); delay(150);
    digitalWrite(LED, LOW);  delay(250);
  }
  delay(500);
}

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(P1, OUTPUT);
  pinMode(P2, OUTPUT);
  analogWriteResolution(8);
}

void loop() {
  shirase(1);                                   // ① 12番だけ
  analogWrite(P1, 200); digitalWrite(P2, LOW);
  delay(3000);

  shirase(2);                                   // ② 0番だけ
  digitalWrite(P1, LOW); analogWrite(P2, 200);
  delay(3000);

  shirase(3);                                   // ③ 12番に戻す ★焦点
  analogWrite(P1, 200); digitalWrite(P2, LOW);
  delay(3000);

  digitalWrite(P1, LOW); digitalWrite(P2, LOW);
  delay(1500);
}
