/*
 * v09b_pulsecheck  ---  切り分け：pulseIn() 単体が動くかをセンサー無しで確かめる
 *
 * v09 で LED がまったく点かないとき、原因が
 *   (A) pulseIn() がこのコアで動かない
 *   (B) HC-SR04 が反応していない（配線・電圧レベル）
 * のどちらかを切り分ける。
 *
 * しくみ
 *   5番ピンから **PWM（ハードウェアが自動で出す方形波）** を出し、
 *   それをジャンパー線で 7番ピンに入れて pulseIn() で測る。
 *   PWM はハードウェアが出すので、CPU が pulseIn で待っていても出続ける。
 *
 * つなぎかた（センサーは外す）
 *   5番ピン ──ジャンパー線1本── 7番ピン
 *
 * 見かた（基板のオレンジLED）
 *   点滅する      → ✅ pulseIn() は動いている（＝原因は (B) センサー側）
 *                    点滅の回数 ≒ 測れた時間 ÷ 100マイクロ秒
 *                    既定の PWM は 1000Hz なので、50% なら 500us ＝ 5回前後のはず
 *   ずっと消灯    → ❌ pulseIn() が 0 を返している（＝原因は (A)）
 *                    ジャンパー線がちゃんと 5番と 7番に入っているかも確認する
 *
 * 点滅の数え方
 *   短い点滅が何回か続いたあと、2秒の休みが入る。その1組を数える。
 */

const int OUT_PIN = 5;   // PWM を出す側（PWM 可を実測ずみ）
const int IN_PIN  = 7;   // pulseIn で測る側
const int LED     = 2;

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(IN_PIN, INPUT);
  pinMode(OUT_PIN, OUTPUT);
  analogWriteResolution(8);
  analogWrite(OUT_PIN, 128);      // デューティ 50% の方形波を出しっぱなし
}

void loop() {
  long t = pulseIn(IN_PIN, HIGH, 30000);   // HIGH の長さ（マイクロ秒）

  if (t == 0) {
    digitalWrite(LED, LOW);       // 測れなかった → 消灯
    delay(500);
    return;
  }

  int n = t / 100;                // 100us を1回として点滅で知らせる
  if (n < 1) n = 1;
  if (n > 9) n = 9;

  for (int i = 0; i < n; i++) {
    digitalWrite(LED, HIGH);
    delay(120);
    digitalWrite(LED, LOW);
    delay(200);
  }
  delay(2000);                    // 区切りの休み
}
