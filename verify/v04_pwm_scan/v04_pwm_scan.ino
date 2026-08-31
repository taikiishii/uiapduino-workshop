/*
 * v04_pwm_scan  ---  検証④：analogWrite（PWM）が本当に効くピンの確認
 *
 * 確認すること
 *   - 公式ピン配置図で PWM 表示があるピン（0・2・5・6・12）で本当にフェードするか
 *   - 公式図に PWM 表示はないが、コアの PinMap_TIM には載っている 1・10 はどうか
 *     （TIM1 の相補出力 CH2N / CH1N。効くかどうかが今回の焦点）
 *   - analogWriteResolution(8) を入れると 0〜255 で指定できるか
 *     （このボードの既定は 12bit ＝ 0〜4095。255 のままだとほとんど光らない）
 *
 * つなぎかた
 *   v02 と同じテストLED（LED + 220Ω）を、いま試しているピンに挿す。
 *
 * 見かた
 *     長く1回点灯  →  短い点滅 N 回  →  対象ピンで 2秒かけて明暗を1往復
 *
 *      1:5   2:6   3:12        ← 公式図で PWM 表示あり（◎のはず）
 *      4:1   5:10              ← 相補出力。効くかどうかが焦点
 *      6:7                     ← PWM 不可のはず（比較用）
 *
 *   - テストLEDが【ふわっと】明るくなって暗くなる → PWM が使える ◎
 *   - テストLEDが【パッと点いてパッと消える】     → PWM は使えない ×
 *
 * メモ
 *   0 も PWM 可（公式図）だが、ここでは 1 との取りちがえを避けるため
 *   4番目に 1 を置いている。0 を確かめたいときは PINS[] に足すこと。
 *   2 は番号を知らせる係に使っているため、この一覧からは外してある。
 */

const int LED = 2;   // 番号を知らせる係（このピン自体も PWM 可）

const int PINS[] = {5, 6, 12, 1, 10, 7};
const int N = sizeof(PINS) / sizeof(PINS[0]);

void announce(int count) {
  digitalWrite(LED, HIGH);
  delay(600);
  digitalWrite(LED, LOW);
  delay(400);
  for (int i = 0; i < count; i++) {
    digitalWrite(LED, HIGH);
    delay(100);
    digitalWrite(LED, LOW);
    delay(200);
  }
  delay(400);
}

void setup() {
  pinMode(LED, OUTPUT);
  analogWriteResolution(8);        // 0〜255 で指定できるようにする（既定は0〜4095）
  for (int i = 0; i < N; i++) {
    pinMode(PINS[i], OUTPUT);
    digitalWrite(PINS[i], LOW);
  }
}

void loop() {
  for (int i = 0; i < N; i++) {
    announce(i + 1);
    for (int v = 0; v <= 255; v += 5) {     // だんだん明るく（約1秒）
      analogWrite(PINS[i], v);
      delay(20);
    }
    for (int v = 255; v >= 0; v -= 5) {     // だんだん暗く（約1秒）
      analogWrite(PINS[i], v);
      delay(20);
    }
    analogWrite(PINS[i], 0);
    digitalWrite(PINS[i], LOW);
    delay(500);
  }
}
