/*
 * v09i_sonic  ---  micros() を使わずに超音波センサーで距離を測る
 *
 * わかったこと
 *   micros() は 1ms 刻みでしか進まない（v09h で実測）。
 *   そのため pulseIn() は 1ms 未満のパルスを測れず 0 を返す。
 *   HC-SR04 の反射時間は 20cm でも約 1.16ms なので、pulseIn では実用にならない。
 *
 * 迂回のしかた
 *   delayMicroseconds() は SysTick レジスタを直接読む別実装で、
 *   micros() の不具合の影響を受けない。
 *   そこで「ECHO が HIGH のあいだ、少しずつ待ちながら回数を数える」方式にする。
 *   数えた回数 × 1回あたりの時間 ＝ パルスの長さ。
 *
 * つなぎかた
 *   HC-SR04 の VCC  … 5V
 *   HC-SR04 の GND  … GND
 *   HC-SR04 の TRIG … 5番ピン
 *   HC-SR04 の ECHO … 7番ピン（5V トレラント）
 *
 * 見かた（基板のオレンジLEDの明るさ ＝ 近さ）
 *   手を近づける → 明るくなる
 *   手を遠ざける → 暗くなる
 *   ずっと消灯   → 反応が返ってこない（配線／センサーの電源電圧を確認）
 *
 * ⚠ 目盛りはまだ合っていない可能性がある。
 *   1回あたりの実際の時間は delayMicroseconds(10) ＋ digitalRead の時間なので
 *   10us より長い。まずは「手の動きに反応するか」だけを見る。
 *   反応したら、定規を使って STEP_US の値を調整する。
 */

const int TRIG = 5;
const int ECHO = 7;
const int LED  = 2;

const int STEP_US   = 10;     // 1回あたりの待ち時間（あとで校正する）
const int MAX_COUNT = 3000;   // 数える上限（約30ms ぶん）

void setup() {
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(LED, OUTPUT);
  analogWriteResolution(8);
  digitalWrite(TRIG, LOW);
}

void loop() {
  // 超音波を発射
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  // ECHO が HIGH になるのを待つ
  int guard = 0;
  while (digitalRead(ECHO) == LOW && guard < MAX_COUNT) {
    delayMicroseconds(STEP_US);
    guard++;
  }
  if (guard >= MAX_COUNT) {          // 反応なし
    analogWrite(LED, 0);
    delay(100);
    return;
  }

  // HIGH のあいだ数える ＝ パルスの長さ
  int count = 0;
  while (digitalRead(ECHO) == HIGH && count < MAX_COUNT) {
    delayMicroseconds(STEP_US);
    count++;
  }

  long us = (long)count * STEP_US;   // パルスの長さ（マイクロ秒・目安）
  long cm = us / 58;                 // 距離（cm・目安）

  long b = map(cm, 2, 40, 255, 0);
  analogWrite(LED, constrain(b, 0, 255));   // 近いほど明るい

  delay(100);
}
