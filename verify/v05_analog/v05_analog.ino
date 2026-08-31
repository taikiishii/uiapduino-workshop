/*
 * v05_analog  ---  検証⑤：analogRead が使えるピンの確認
 *
 * 確認すること
 *   - A0〜A7 のうち、実際にアナログ値を読めるのはどれか
 *   - 値の範囲が 0〜1023（10bit）であること
 *
 * ソースから判明していること
 *   variant_CH32V003F4.h … ADC_RESOLUTION 10（＝ 0〜1023）
 *   PeripheralPins.c の PinMap_ADC と、変換表（PIN_A6 / PIN_A7）が食い違っている。
 *   → **A0〜A5 は使えるはず。A6・A7 は要確認**（このスケッチの主目的）
 *
 * つなぎかた（可変抵抗 10kΩ）
 *   両端 … 片方を 5V（または3V3）、もう片方を GND
 *   まん中（ワイパー） … TEST_PIN
 *
 * 使いかた
 *   下の TEST_PIN を A0 → A1 → … → A7 と書きかえて、1本ずつ試す。
 *
 * 見かた
 *   つまみを回すと、基板のオレンジLEDの明るさが変わる。
 *   - 端から端まで回して【なめらかに1往復ぶん】暗→明 と変わる → 正常（0〜1023）
 *   - 途中で【何度も明るさが往復する】                       → 値が1023を超えている
 *   - まったく変わらない                                     → そのピンでは analogRead が使えない
 */

#define TEST_PIN A2   // ← A0 / A1 / A2 / A3 / A4 / A5 / A6 / A7 と書きかえて試す

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
