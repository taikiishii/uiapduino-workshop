/*
 * v00_allhigh  ---  切り分け用：使えるピンを全部 HIGH にしっぱなしにする
 *
 * テストLEDがどのピンでも光らないとき、原因が
 *   「配線・LEDの向き・LEDそのもの」なのか
 *   「スケッチのタイミング（v04 の合図待ち）」なのか
 * を切り分けるためのスケッチ。
 *
 * 動き
 *   - 0,1,3,4,5,6,7,8,9,10,12,15,16 を **ずっと HIGH** にする
 *   - 基板のオレンジLED（2）は1秒ごとに点滅 ＝ プログラムが動いているしるし
 *
 * 使いかた
 *   テストLEDを各ピンに挿していく。HIGH なので **挿せばすぐ光る**はず。
 *
 * それでも光らないときの確認（上から順に）
 *   1. テストLEDを **5V と GND のあいだ** に直接つないでみる
 *      → ここで光らなければ、LEDの向き・断線・抵抗値のどれか
 *   2. LEDの **長い足が＋（ピン側）** になっているか
 *   3. 抵抗のもう一方が、本当に **GND** のピンに入っているか
 *   4. LEDの色は？ 青・白・緑は Vf が 3V 以上あるので、
 *      ボードが 3.3V 動作だと 330Ω ではほとんど光らない（赤・黄がおすすめ）
 *
 * ⚠ 11（PD1）は SWIO と共用なので含めていない。2 は点滅の係。
 */

const int LED = 2;   // 基板のオレンジLED（動いているしるし）

const int PINS[] = {0, 1, 3, 4, 5, 6, 7, 8, 9, 10, 12, 15, 16};
const int N = sizeof(PINS) / sizeof(PINS[0]);

void setup() {
  pinMode(LED, OUTPUT);
  for (int i = 0; i < N; i++) {
    pinMode(PINS[i], OUTPUT);
    digitalWrite(PINS[i], HIGH);   // ずっと HIGH
  }
}

void loop() {
  digitalWrite(LED, HIGH);
  delay(1000);
  digitalWrite(LED, LOW);
  delay(1000);
}
