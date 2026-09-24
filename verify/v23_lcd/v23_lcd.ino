/*
 * v23_lcd  ---  検証㉓：LCD 1602 のライブラリがこのボードで使えるか
 *
 * 先に v22_i2cscan でアドレスを確かめておくこと（0x27 なら下を書きかえる）。
 *
 * 確かめること
 *   1. ライブラリ「LiquidCrystal I2C」（作者 Frank de Brabander）が
 *      このボードで**コンパイルできるか**
 *      ⚠ 「このライブラリは AVR 用」という警告が出ることがある。
 *        警告だけで書きこめるなら、そのことを記録する
 *   2. **16KB に入るか**。書きこみのあと、画面の下に
 *      「最大16384バイトのフラッシュメモリのうち、スケッチが ○○ バイト（○○%）」
 *      と出る。**この数字を記録する**（教材で「あとどれだけ書けるか」の目安になる）
 *   3. 画面に文字が出るか。1行目に「Hello!」、2行目に数が1秒ごとに増える
 *
 * つなぎかた（v22 と同じ）
 *   GND → GND ／ VCC → 5V ／ SDA → 3番 ／ SCL → 4番
 *
 * 文字が見えないとき
 *   ・光っているのに文字が出ない → 変換基板の裏の**青いつまみ**（コントラスト）を回す
 *   ・光りもしない             → 配線と、アドレス（0x27／0x3F）を確かめる
 *
 * うまくいかないときの 次の候補
 *   ライブラリ「LiquidCrystal_PCF8574」（作者 Matthias Hertel）。
 *   こちらは「どのボードでも使える」と宣言している（architectures=*）。
 */
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 16, 2);   // アドレス・16文字・2行（手元の LCD は 0x3F だった）

int n = 0;

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Hello!");
}

void loop() {
  lcd.setCursor(0, 1);
  lcd.print(n);
  n = n + 1;
  delay(1000);
}
