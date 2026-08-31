/*
 * v03_pullup_scan  ---  検証③：INPUT_PULLUP + digitalRead が使えるか
 *
 * 確認すること
 *   - pinMode(pin, INPUT_PULLUP) が効く（何もつながないと HIGH）
 *   - GND に落とすと LOW を読める（＝タクトスイッチが使える）
 *
 * つなぎかた
 *   ジャンパー1本の片側を GND に挿し、もう片側で調べたいピンに順番に触れる。
 *
 * 見かた
 *   - 何もつながない状態：基板のオレンジLEDは【消灯】が正しい
 *   - ピンを GND に触れたとき：オレンジLEDが【点灯】すれば、そのピンは使える
 *
 *   ★ 何もつないでいないのに点きっぱなしになるときは、どれか1本が常に LOW を
 *     読んでいる。PINS[] を1本だけに減らして（例：{3}）1本ずつ調べ、
 *     犯人のピンを特定して「スイッチには使えないピン」として記録する。
 */

const int LED = 2;   // 結果を見せる係（基板のオレンジLED）

const int PINS[] = {3, 4, 5, 7, 8, 9, 10, 17,
                    A0, A1, A2, A3, A4, A5, A6, A7};
const int N = sizeof(PINS) / sizeof(PINS[0]);

void setup() {
  pinMode(LED, OUTPUT);
  for (int i = 0; i < N; i++) {
    pinMode(PINS[i], INPUT_PULLUP);
  }
}

void loop() {
  bool anyLow = false;
  for (int i = 0; i < N; i++) {
    if (digitalRead(PINS[i]) == LOW) {
      anyLow = true;
    }
  }
  digitalWrite(LED, anyLow ? HIGH : LOW);
  delay(20);
}
