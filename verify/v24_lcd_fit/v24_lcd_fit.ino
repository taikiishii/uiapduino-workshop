/*
 * v24_lcd_fit  ---  検証㉔：m1 の 💪 の課題が Flash に入るか
 *
 * なぜ調べるか
 *   m1 の「つまみのプログラム」で Flash が 96%（15780 / 16384）になった。
 *   残りは約 600 バイト。💪 の課題がこの枠に入るかを確かめる。
 *   とくに温度は小数（float）を使う。このマイコンには小数を計算する回路が
 *   ないので、小数の計算と表示のプログラムが丸ごと足され、大きくなるはず。
 *
 * やりかた
 *   **配線は要らない。コンパイル（✓ボタン）だけでよい**。
 *   TEST を 1〜4 に変えて、それぞれ下に出る「スケッチが ○○ バイト（○○%）」を記録する。
 *   入らないときは「スケッチが大きすぎます」のようなエラーになる。
 *
 *   TEST 1 … 💪2 つまみを map で 0〜100% に直して出す
 *   TEST 2 … 💪3 温度を e2 と同じ式（float）で出す
 *   TEST 3 … 💪3 温度を整数だけで出す（0.1℃ 単位で計算して、小数点を自分で書く）
 *   TEST 4 … 💪4 距離を e3 と同じ kyori() で出す
 */
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define TEST 1

LiquidCrystal_I2C lcd(0x3F, 16, 2);

#if TEST == 4
const int TRIG = 9;
const int ECHO = 7;

int kyori() {
  digitalWrite(TRIG, HIGH); delayMicroseconds(10); digitalWrite(TRIG, LOW);
  int w = 0;
  while (digitalRead(ECHO) == LOW) { if (++w > 600) return 999; }
  int n = 0;
  while (digitalRead(ECHO) == HIGH && n < 600) {
    delayMicroseconds(50);
    n++;
  }
  return n - 2;
}
#endif

void setup() {
  lcd.init();
  lcd.backlight();
#if TEST == 4
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
#endif
}

void loop() {
  lcd.setCursor(0, 1);
#if TEST == 1
  lcd.print(map(analogRead(A2), 0, 1023, 0, 100));
  lcd.print("%   ");
#elif TEST == 2
  int v = analogRead(A2);
  float mv = v * 5000.0 / 1024;
  float t = (mv - 400) / 19.5;
  lcd.print(t);
  lcd.print(" C   ");
#elif TEST == 3
  long mv = analogRead(A2) * 5000L / 1024;
  int t10 = (mv - 400) * 100 / 195;     // 0.1℃ 単位。÷19.5 ×10 を、整数で ×100 ÷195 にした
  lcd.print(t10 / 10);
  lcd.print(".");
  lcd.print(t10 % 10);
  lcd.print(" C   ");
#elif TEST == 4
  lcd.print(kyori());
  lcd.print(" cm   ");
#endif
  delay(200);
}
