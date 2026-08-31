/*
 * v02_digital_scan  ---  検証②：digitalWrite が使えるピンを洗い出す
 *
 * 確認すること
 *   - 公式ピン配置図どおりのピン番号で digitalWrite が効くか
 *   - Arduino のピン番号と、基板の刻印の対応
 *
 * つなぎかた
 *   テストLED（LED + 220Ω を直列にしたもの）を用意し、
 *   抵抗側を GND、LED側を「いま試しているピン」に挿す。
 *
 * 見かた
 *   基板のオレンジLEDが、いま何番目のピンを試しているかを知らせる。
 *     長く1回点灯  →  短い点滅 N 回（＝ PINS[] の N 番目）  →  対象ピンが 2.5 秒 ON
 *
 *      1:0   2:1   3:3   4:4   5:5   6:6   7:7
 *      8:8   9:9  10:10 11:12 12:15 13:16
 *
 *   その 2.5 秒のあいだにテストLEDが光れば、そのピンは digitalWrite で使える。
 *
 * メモ（公式ピン配置図より）
 *   使えるピン番号は 0〜12 と 15・16 だけ。13・14・17 は欠番。
 *   2  … 基板のオレンジLED（番号を知らせる係なので除外）
 *   11 … SWIO（デバッグ用）と共用。使うには pinV32_DisconnectDebug(PD_1); が必要なため除外
 *   PD3・PD4 は USB の D+/D− なので、そもそもピンとして出ていない（A4・A7 は存在しない）
 */

const int LED = 2;   // 番号を知らせる係（基板のオレンジLED = PC0）

const int PINS[] = {0, 1, 3, 4, 5, 6, 7, 8, 9, 10, 12, 15, 16};
const int N = sizeof(PINS) / sizeof(PINS[0]);

// 「いま何番目か」をオレンジLEDの点滅で知らせる
void announce(int count) {
  digitalWrite(LED, HIGH);   // 長い点灯＝区切り
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
  for (int i = 0; i < N; i++) {
    pinMode(PINS[i], OUTPUT);
    digitalWrite(PINS[i], LOW);
  }
}

void loop() {
  for (int i = 0; i < N; i++) {
    announce(i + 1);
    digitalWrite(PINS[i], HIGH);   // ここでテストLEDが光るはず
    delay(2500);
    digitalWrite(PINS[i], LOW);
    delay(500);
  }
}
