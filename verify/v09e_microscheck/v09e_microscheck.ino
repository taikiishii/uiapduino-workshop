/*
 * v09e_microscheck  ---  切り分け③：micros() の目盛りを millis() を基準に測る
 *
 * v09c で「フェードして見える」＝ 切りかえが速すぎる ＝
 * micros() が本来より速く進んでいる疑いがある。
 * millis() は v07 で正確（誤差1.7%）と確認ずみなので、それを物差しにする。
 *
 * しくみ
 *   millis() で**ちょうど1秒**待つあいだに、micros() がいくつ進むかを数える。
 *   正常なら 1,000,000（100万）進むはず。
 *
 * 見かた（基板のオレンジLEDの点滅回数 ＝ 進んだ量 ÷ 10万）
 *   **10回**  → ✅ 正常（1秒で 100万マイクロ秒）
 *   20回      → 2倍速い
 *   1回       → 10分の1しか進んでいない
 *   16回で頭打ち → 16倍以上速い（数えきれない）
 *   0回（点かない） → micros() が進んでいない
 *
 * 数え方
 *   短い点滅が何回か続いたあと、**2秒の休み**が入る。その1組を数える。
 *   自信がなければ2〜3周見て、同じ数になることを確かめる。
 */

const int LED = 2;

void setup() {
  pinMode(LED, OUTPUT);
}

void loop() {
  // --- millis() で 1秒待つあいだに micros() がいくつ進むかを見る ---
  unsigned long m0 = micros();
  unsigned long t0 = millis();
  while (millis() - t0 < 1000UL) {
    // なにもしない（1秒待つだけ）
  }
  unsigned long delta = micros() - m0;

  // --- 10万マイクロ秒を1回として点滅で知らせる ---
  int n = delta / 100000UL;      // 正常なら 10
  if (n < 1) n = 1;
  if (n > 16) n = 16;            // 16回で頭打ち

  if (delta == 0) {              // まったく進んでいない場合
    digitalWrite(LED, LOW);
    delay(3000);
    return;
  }

  for (int i = 0; i < n; i++) {
    digitalWrite(LED, HIGH);
    delay(120);
    digitalWrite(LED, LOW);
    delay(200);
  }
  delay(2000);                   // 区切りの休み
}
