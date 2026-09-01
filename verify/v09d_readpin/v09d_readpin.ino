/*
 * v09d_readpin  ---  切り分け②：7番ピンが「入力」として読めるか
 *
 * なぜ調べるか
 *   v00 で 7番の出力は確認したが、**入力として読めるか**は未確認。
 *   v03（INPUT_PULLUP）で実際に触ったのは 3番・4番だけだった。
 *   pulseIn() はピンを INPUT にして読むので、ここが動かないと必ず 0 になる。
 *
 * つなぎかた（ジャンパー線1本だけ）
 *   7番ピン ── 3V3（または 5V）  … LEDが点くはず
 *   7番ピン ── GND               … LEDが消えるはず
 *   ※ 抜き差しはUSBを抜いた状態で行うと安全
 *
 * 見かた
 *   3V3 で点灯・GND で消灯   → ✅ 7番は入力として正常（原因は別のところ）
 *   どちらでも変わらない     → ❌ 7番が入力として読めていない
 *   何もつながずチカチカする → 入力が宙に浮いている（正常な反応。判定には使わない）
 *
 * ⚠ 7番を 5V につなぐのは、公式図で 5V トレラント表示があるため。
 *   ほかのピンで同じことをするときは figure を確認すること。
 */

const int IN_PIN = 7;
const int LED    = 2;

void setup() {
  pinMode(IN_PIN, INPUT);      // pulseIn と同じ「ふつうの入力」
  pinMode(LED, OUTPUT);
}

void loop() {
  digitalWrite(LED, digitalRead(IN_PIN) == HIGH ? HIGH : LOW);
  delay(20);
}
