/*
 * v02_digital_scan  ---  検証②：digitalWrite が使えるピンを洗い出す
 *
 * 確認すること
 *   - どのピン番号で digitalWrite が効くか
 *   - Arduino のピン番号／A◯ の名前と、基板の刻印の対応
 *
 * つなぎかた
 *   テストLED（LED + 220Ω を直列にしたもの）を用意し、
 *   抵抗側を GND、LED側を「いま試しているピン」に挿す。
 *
 * 見かた
 *   基板のオレンジLEDが、いま何番目のピンを試しているかを知らせる。
 *     長く1回点灯  →  短い点滅 N 回（＝ PINS[] の N 番目）  →  対象ピンが 2.5 秒 ON
 *
 *      1:D3   2:D4   3:D5   4:D7   5:D8   6:D9   7:D10  8:D17
 *      9:A0  10:A1  11:A2  12:A3  13:A4  14:A5  15:A6  16:A7
 *
 *   その 2.5 秒のあいだにテストLEDが光れば、そのピンは digitalWrite で使える。
 *
 * メモ
 *   ボード定義（variant_CH32V003F4.h）では、存在するピン番号は
 *     2,3,4,5,7,8,9,10,11,17 と A0〜A7 だけ。0・1・6 などは割り当てが無い。
 *   D2 は基板のLED（番号を知らせる係）なので除外。D11 は SWIO と共用のため使わない。
 */

const int LED = 2;   // 番号を知らせる係（基板のオレンジLED = PC0）

const int PINS[] = {3, 4, 5, 7, 8, 9, 10, 17,
                    A0, A1, A2, A3, A4, A5, A6, A7};
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
