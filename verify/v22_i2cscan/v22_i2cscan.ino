/*
 * v22_i2cscan  ---  検証㉒：I2C がこのボードで使えるか、LCD のアドレスはいくつか
 *
 * なぜ調べるか
 *   拡張編の LCD 1602（I2C 変換基板つき）は、I2C で合図をやりとりする。
 *   このコアの Wire（I2C）が 3番（SDA）・4番（SCL）で本当に動くかは
 *   まだ確かめていない。LCD のアドレス（0x27 か 0x3F が多い）も
 *   基板によってちがう。**ライブラリを入れる前に**、ここを確かめる。
 *
 *   ライブラリを使わず、ボードに入っている Wire だけで調べる。
 *
 * つなぎかた（LCD の変換基板の4本）
 *   GND → GND ／ VCC → 5V ／ SDA → 3番 ／ SCL → 4番
 *
 * 見かた（基板のオレンジLED＝2番）
 *   はじめに1秒点灯 … 調べている
 *   ゆっくり1回ずつ点滅 … ✅ 0x27 に見つかった
 *   ゆっくり2回ずつ点滅 … ✅ 0x3F に見つかった
 *   長く○回・短く○回 … ✅ ほかのアドレスに見つかった
 *                         （長い回数＝上の桁、短い回数＝下の桁。16進）
 *   速い点滅がつづく    … ❌ 何も見つからない（配線・電源を確認）
 *   ついたまま消えない  … ❌ Wire が止まった（I2C がうまく動いていない）
 */
#include <Wire.h>

const int LED = 2;

void blink(int n, int on, int off) {
  for (int i = 0; i < n; i++) {
    digitalWrite(LED, HIGH); delay(on);
    digitalWrite(LED, LOW);  delay(off);
  }
}

int found = 0;

void setup() {
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);          // 調べている
  delay(1000);
  Wire.begin();
  for (int a = 1; a < 127; a++) {
    Wire.beginTransmission(a);
    if (Wire.endTransmission() == 0) { found = a; break; }
  }
  digitalWrite(LED, LOW);           // 調べおわった
  delay(1000);
}

void loop() {
  if (found == 0x27) {
    blink(1, 500, 500);
  } else if (found == 0x3F) {
    blink(2, 500, 500);
  } else if (found != 0) {
    blink(found >> 4, 700, 300);    // 上の桁
    delay(600);
    blink(found & 0x0F, 150, 250);  // 下の桁
  } else {
    blink(1, 80, 80);               // 見つからない
    return;
  }
  delay(2000);
}
