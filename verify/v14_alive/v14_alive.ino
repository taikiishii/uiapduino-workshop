/*
 * v14_alive  ---  検証⑭：e5 の loop() が回りつづけているか
 *
 * なぜ確かめるのか
 *   e5 のモーターが「一度回って止まると、5秒以上たっても二度と動かない」。
 *   loop() は 2+2+1 = **5秒周期**なので、生きていれば必ずまた回るはず。
 *   → **マイコンが止まっている**のか、**モーターがわの問題**なのかを分ける。
 *
 * つなぎかた
 *   **モーターは外す**（ドライバはさしたままでよい）。
 *   基板のオレンジLED（2番）だけを見る。配線の追加は要らない。
 *
 * 見かた
 *   基板のオレンジLED が
 *     **2秒点灯 → 1秒消灯 → 短く2回 → 1秒消灯**  を くりかえす
 *
 *   ① くりかえす        → loop() は回っている。**原因はモーターがわ**
 *                          （電源不足／ドライバの保護／配線）
 *   ② 1回で止まる        → **マイコンが止まっている**
 *                          （電源が落ちた／リセットがかかった）
 *   ③ 最初から光らない   → 書きこみのあと RESET を押していない
 *
 * ①だった場合につぎに見るところ
 *   - 電池の電圧（モーターを回すと落ちていないか）
 *   - ボードの GND と 電池の − が **同じ −レール**につながっているか
 *   - DRV8835 の VCC が **5V**（3V3 ではない）か
 *   - モーターの2本が AOUT1・AOUT2 に入っているか（VM と取りちがえていないか）
 */

const int LED  = 2;      // 基板のオレンジLED
const int AIN1 = 12;
const int AIN2 = 0;

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  analogWriteResolution(8);
}

void loop() {
  // e5 と同じ手順をなぞる（モーターは外してあるので、見るのはLEDだけ）
  digitalWrite(LED, HIGH);                              // 正転のあいだ 点灯
  analogWrite(AIN1, 200);  digitalWrite(AIN2, LOW);
  delay(2000);

  digitalWrite(LED, LOW);                               // 逆転のあいだ 消灯
  digitalWrite(AIN1, LOW); analogWrite(AIN2, 200);
  delay(1000);

  for (int i = 0; i < 2; i++) {                         // 停止のしるしに 2回
    digitalWrite(LED, HIGH); delay(150);
    digitalWrite(LED, LOW);  delay(250);
  }
  digitalWrite(AIN1, LOW); digitalWrite(AIN2, LOW);
  delay(1000);
}
