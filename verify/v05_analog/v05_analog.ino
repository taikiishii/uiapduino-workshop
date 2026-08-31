/*
 * v05_analog  ---  検証⑤：analogRead が使えるピンの確認
 *
 * 確認すること
 *   - 公式ピン配置図にある 6本（A0・A1・A2・A3・A5・A6）で実際に値が読めるか
 *   - 値の範囲が 0〜1023（10bit）であること
 *
 * わかっていること
 *   公式図 … アナログ入力は A0(=1) A1(=0) A2(=6) A3(=12) A5(=15) A6(=16) の6本。
 *            **A4 と A7 は存在しない**（PD3・PD4 は USB の D+/D− に使われている）
 *   コア   … variant_CH32V003F4.h の ADC_RESOLUTION は 10（＝ 0〜1023）。
 *            ただし PinMap_ADC と変換表が一部食い違っているので、
 *            **A5・A6 が本当に読めるかは実機で確かめる**（このスケッチの主目的）
 *
 * つなぎかた（可変抵抗 10kΩ）
 *   両端 … 片方を 5V（または3V3）、もう片方を GND
 *   まん中（ワイパー） … TEST_PIN
 *
 * 使いかた
 *   下の TEST_PIN を書きかえて、A0 → A1 → A2 → A3 → A5 → A6 と1本ずつ試す。
 *
 * 見かた
 *   つまみを回すと、基板のオレンジLEDの明るさが変わる。
 *   - 端から端まで回して【なめらかに1往復ぶん】暗→明 と変わる → 正常（0〜1023）
 *   - 途中で【何度も明暗をくり返す】                         → 値が1023を超えている
 *   - まったく変わらない                                     → そのピンでは analogRead が使えない
 */

#define TEST_PIN A2   // ← A0 / A1 / A2 / A3 / A5 / A6 と書きかえて試す（A4・A7 は無い）

const int LED = 2;    // 基板のオレンジLED（PWM 可）

void setup() {
  pinMode(LED, OUTPUT);
  analogWriteResolution(8);          // 明るさを 0〜255 で指定する
}

void loop() {
  int value = analogRead(TEST_PIN);  // 期待値：0〜1023（10bit）
  analogWrite(LED, value / 4);       // 0〜255 に変換して明るさで見せる
  delay(10);
}
