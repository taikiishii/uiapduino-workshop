/*
 * v07_clock  ---  検証⑦：内蔵発振器の精度を「数えずに」測る
 *
 * v06 では点滅の回数を数えたが、1回の数えちがいが 2 秒の誤差になってしまう。
 * このスケッチは **点いて消えるまで1回だけ** なので、数えまちがいが起きない。
 *
 * 使いかた
 *   1. 書きこむ
 *   2. RESET を押す → その瞬間にオレンジLEDが点灯する
 *   3. 点灯した瞬間にストップウォッチをスタート
 *   4. LEDが消えた瞬間にストップ
 *
 * 見かた（理論値 60.0 秒）
 *   59〜61 秒   → 精度は十分（±2%以内）。tone() もサーボも問題なし
 *   56 秒くらい → millis が約 7% 速い。音程は半音ほどずれる
 *   64 秒くらい → millis が約 7% 遅い
 *
 * ずれていた場合の直し方
 *   ずれの割合がわかれば、tone() の周波数やサーボのパルス幅を
 *   その割合でかけ算して補正できる（教材には出さず、内部の調整に使う）。
 */

const int LED = 2;                    // 基板のオレンジLED
const unsigned long DURATION = 60000UL;   // 60秒（millis の数え方で）

bool finished = false;

void setup() {
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);            // ここから計測スタート
}

void loop() {
  if (!finished && millis() >= DURATION) {
    digitalWrite(LED, LOW);           // ここでストップ
    finished = true;
  }
}
