/*
 * v01_blink  ---  検証①：そもそも書き込めるか
 *
 * 確認すること
 *   - Arduino IDE + UIAPduino ボードパッケージでコンパイルできる
 *   - RESET を押しながら USB を挿す書き込み手順が通る
 *   - 基板のオレンジLED（D2）が制御できる
 *
 * 見かた
 *   オレンジLEDが 1秒周期（0.5秒点灯・0.5秒消灯）で点滅すれば成功。
 *
 * メモ
 *   コンパイル後に IDE 下部へ出る Flash / RAM の使用量を README に記録すること。
 */

const int LED = 2;   // 基板のオレンジLED

void setup() {
  pinMode(LED, OUTPUT);
}

void loop() {
  digitalWrite(LED, HIGH);
  delay(500);
  digitalWrite(LED, LOW);
  delay(500);
}
