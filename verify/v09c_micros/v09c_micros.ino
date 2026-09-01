/*
 * v09c_micros  ---  切り分け①：micros() が動いているか（配線不要）
 *
 * なぜ調べるか
 *   pulseIn() は内部で **micros() だけ**を使って時間を測っている。
 *   micros() が止まっていたり値がおかしいと、pulseIn() は
 *   タイムアウト判定もできず 0 を返す（または抜けられなくなる）。
 *   millis() は v06/v07 で動作確認ずみだが、micros() は未確認。
 *
 * 動き
 *   micros() だけを使って、0.5秒ごとに基板のオレンジLEDを切りかえる
 *   （＝ v01 と同じ 1秒周期の点滅になるはず）。
 *
 * 見かた
 *   v01 と同じテンポで点滅  → ✅ micros() は正常（pulseIn の土台は問題なし）
 *   まったく点滅しない      → ❌ micros() が進んでいない
 *   極端に速い／遅い        → ⚠ micros() の目盛りがずれている
 *                              （その比率ぶん pulseIn の値もずれる）
 */

const int LED = 2;

unsigned long previous = 0;
bool isOn = false;

void setup() {
  pinMode(LED, OUTPUT);
  previous = micros();
}

void loop() {
  unsigned long now = micros();
  if (now - previous >= 500000UL) {   // 0.5秒 ＝ 500000 マイクロ秒
    previous += 500000UL;
    isOn = !isOn;
    digitalWrite(LED, isOn ? HIGH : LOW);
  }
}
