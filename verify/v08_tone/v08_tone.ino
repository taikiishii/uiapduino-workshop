/*
 * v08_tone  ---  検証⑧：tone() が使えるか／LEDのPWMと干渉しないか
 *
 * 確認すること
 *   1. tone() で音が鳴るか、音の高さが変わるか（b5 ブザーの成否）
 *   2. tone() を鳴らしながら analogWrite が使えるか
 *      → tone() は TIM2 を使う。基板のLED（2番＝PC0）の PWM も TIM2 なので、
 *         取り合いが起きる可能性がある。ここを実際に確かめる。
 *
 * わかっていること（コアのソースより）
 *   Tone.cpp はタイマー割り込みで GPIO をトグルする実装なので、
 *   **どのピンでも鳴らせる**（タイマー出力チャネルである必要がない）。
 *
 * つなぎかた
 *   受動ブザー（圧電スピーカー）の片方 … 7番ピン
 *   もう片方                          … GND
 *   ※「能動ブザー（電圧をかけるだけで鳴るタイプ）」だと音の高さは変わりません。
 *     音階が変わらない場合は、まずブザーの種類を疑ってください。
 *
 * 見かた
 *   【第1段階・約4秒】ドレミファソラシド と音が上がっていく
 *       → 音が上がる     : tone() は使える ◎
 *       → 同じ音が続く   : 能動ブザーの可能性（受動ブザーに交換して再確認）
 *       → 鳴らない       : 配線か、tone() が動いていない
 *   【第2段階・約4秒】ラ(440Hz)が鳴りっぱなしの間、基板のオレンジLEDがフェード
 *       → LEDがなめらかに明暗 : 干渉なし ◎（音とLEDを同時に使える）
 *       → LEDがちらつく/消える: TIM2 の取り合いあり（教材で注意が必要）
 *   【第3段階・2秒】無音・消灯 → 最初にもどる
 */

const int BUZZER = 7;   // ブザー（PWM不要なピンを選ぶ）
const int LED    = 2;   // 基板のオレンジLED（PWM は TIM2 ＝ tone と同じタイマー）

// ドレミファソラシド（Hz）
const int SCALE[] = {262, 294, 330, 349, 392, 440, 494, 523};
const int SCALE_N = sizeof(SCALE) / sizeof(SCALE[0]);

void setup() {
  pinMode(LED, OUTPUT);
  analogWriteResolution(8);
}

void loop() {
  // ---- 第1段階：音階を鳴らす ----
  for (int i = 0; i < SCALE_N; i++) {
    tone(BUZZER, SCALE[i]);
    digitalWrite(LED, HIGH);
    delay(400);
    digitalWrite(LED, LOW);
    delay(100);
  }
  noTone(BUZZER);
  delay(500);

  // ---- 第2段階：鳴らしながらLEDをフェード ----
  tone(BUZZER, 440);                       // ラ を鳴らしっぱなし
  for (int v = 0; v <= 255; v += 5) { analogWrite(LED, v); delay(8); }
  for (int v = 255; v >= 0; v -= 5) { analogWrite(LED, v); delay(8); }
  analogWrite(LED, 0);
  noTone(BUZZER);

  // ---- 第3段階：休み ----
  digitalWrite(LED, LOW);
  delay(2000);
}
