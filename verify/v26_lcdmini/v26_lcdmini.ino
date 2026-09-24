/*
 * v26_lcdmini  ---  検証㉖：付録⑥に載せる自作ライブラリ LcdMini.h が動くか
 *
 * v25（実機で動作ずみ）の中身を、付録⑥のスライドに載せられるように
 * 名前をそろえて LcdMini.h という1つのファイルにまとめたもの。
 * 動きは v25 と同じにしてあるが、書き直したので**実機でもう一度確かめる**。
 *
 * つなぎかた（拡張編① と同じ）
 *   GND → GND ／ VCC → 5V ／ SDA → 3番 ／ SCL → 4番
 *
 * 見かた：1行目に「count」、2行目に 1秒ごとに増える数 → ✅
 *
 * ⚠ 付録⑥のスライドのコードは、このフォルダの LcdMini.h と**まったく同じ**。
 *   ここを直したら、スライド（content/x6_mylib/index.md の①〜④）も同じに直す。
 */
#include "LcdMini.h"

int n = 0;

void setup() {
  lcdInit();
  lcdStr("count");
}

void loop() {
  lcdAt(0, 1);
  lcdNum(n);
  n = n + 1;
  delay(1000);
}
