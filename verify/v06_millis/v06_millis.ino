/*
 * v06_millis  ---  検証⑥：millis() / map() と、内蔵発振器の精度
 *
 * 確認すること
 *   - millis() が使えるか（delay を使わない書き方ができるか）
 *   - map() が使えるか（下の INTERVAL の計算で使用。map(50,0,100,0,2000) = 1000）
 *   - 時間が正確か（このボードは水晶を持たず、内蔵発振器で動いている）
 *
 * 見かた
 *   基板のオレンジLEDが 1秒ごとに点灯・消灯を切りかえる（＝点滅1回で2秒）。
 *   「点いて消えて」を1回と数えて、**15回ぶん**をストップウォッチで計る。
 *     ぴったり 30秒に近い     → 精度は十分。tone() やサーボも期待できる
 *     数％ずれる（29秒/31秒） → 音の高さや通信の精度に影響が出るかもしれない
 *
 * メモ
 *   map() でコンパイルエラーになる場合は INTERVAL = 1000 に書きかえて、
 *   「map() は使えない」と記録する（c 制御編の教材から map を外す判断材料になる）。
 */

const int LED = 2;
const unsigned long INTERVAL = map(50, 0, 100, 0, 2000);   // = 1000 ミリ秒

unsigned long previous = 0;
bool isOn = false;

void setup() {
  pinMode(LED, OUTPUT);
  previous = millis();
}

void loop() {
  unsigned long now = millis();
  if (now - previous >= INTERVAL) {
    previous += INTERVAL;
    isOn = !isOn;
    digitalWrite(LED, isOn ? HIGH : LOW);
  }
}
