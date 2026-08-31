/*
 * v04_pwm_scan  ---  検証④：analogWrite（PWM）が本当に効くピンの確認
 *
 * 確認すること
 *   - ソースから読み取った「PWMが使えるピン」が実機と一致するか
 *   - analogWriteResolution(8) を入れると 0〜255 で指定できるか
 *     （このボードの既定は 12bit ＝ 0〜4095。255 のままだとほとんど光らない）
 *
 * ソースから判明していること（PeripheralPins.c の PinMap_TIM）
 *   PWM 可 … D2(TIM2) D5(TIM1) D10(TIM1) D11(TIM1・使わない) D17(TIM2)
 *            A0 A1 A2 A3(TIM1) A4 A7(TIM2)
 *   PWM 不可 … D3 D4 D7 D8 D9 A5 A6
 *
 * つなぎかた
 *   v02 と同じテストLED（LED + 220Ω）を、いま試しているピンに挿す。
 *
 * 見かた
 *     長く1回点灯  →  短い点滅 N 回  →  対象ピンで 2秒かけて明暗を1往復
 *
 *      1:D5   2:D10  3:D17  4:A0   5:A1   6:A2   7:A3   8:A4   9:A7
 *
 *   - テストLEDが【ふわっと】明るくなって暗くなる → PWM が使える ◎
 *   - テストLEDが【パッと点いてパッと消える】     → PWM は使えない ×
 *
 *   ★ 比較用に、PWM 不可のはずの D7 などを PINS[] に足して確かめるとよい。
 */

const int LED = 2;   // 番号を知らせる係（このピン自体も PWM 可・TIM2）

const int PINS[] = {5, 10, 17, A0, A1, A2, A3, A4, A7};
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
