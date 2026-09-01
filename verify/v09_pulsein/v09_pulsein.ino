/*
 * v09_pulsein  ---  検証⑨：pulseIn() と超音波センサー（HC-SR04）
 *
 * 確認すること
 *   1. pulseIn() が使えるか（e3「ものとの距離をはかろう」の成否）
 *   2. 距離の値が妥当か（定規と見くらべる）
 *   3. ECHO の 5V 出力をボードが受けられるか
 *
 * つなぎかた
 *   HC-SR04 の VCC  … 5V
 *   HC-SR04 の GND  … GND
 *   HC-SR04 の TRIG … 5番ピン
 *   HC-SR04 の ECHO … 7番ピン（公式図で 5V トレラント表示のあるピン）
 *
 * ⚠ ECHO は 5V を出す。ボードが 3.3V 動作の場合、5V トレラントでないピンに
 *   つなぐと傷める可能性がある。**必ず 7番**を使うこと。
 *
 * 見かた（基板のオレンジLEDの明るさ＝近さ）
 *   手をセンサーに近づける → **明るくなる**（5cm 以下でいちばん明るい）
 *   手を遠ざける           → **暗くなる**（40cm 以上で消える）
 *   ずっと消えたまま       → 反応が返ってきていない
 *                            （配線／TRIG・ECHO の取りちがえ／pulseIn 不可）
 *   ずっと明るいまま       → 何かが近くにある、または誤検出
 *
 * 距離が合っているかの確かめ方
 *   センサーから 10cm のところに本などを置き、そこを境に
 *   明るさがちょうど中くらいになるか見る（2cm→255、40cm→0 の割りあて）。
 */

const int TRIG = 5;
const int ECHO = 7;
const int LED  = 2;      // 基板のオレンジLED（明るさで距離を見せる）

void setup() {
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(LED, OUTPUT);
  analogWriteResolution(8);
  digitalWrite(TRIG, LOW);
}

void loop() {
  // 10マイクロ秒のパルスを出して超音波を発射
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  // はね返ってくるまでの時間（マイクロ秒）。30ms で見切りをつける
  long t = pulseIn(ECHO, HIGH, 30000);

  if (t == 0) {
    analogWrite(LED, 0);          // 返事なし → 消灯
  } else {
    long cm = t / 58;             // 時間から距離（cm）へ
    long b = map(cm, 2, 40, 255, 0);
    analogWrite(LED, constrain(b, 0, 255));   // 近いほど明るい
  }

  delay(100);
}
