/*
 * v11_pir  ---  検証⑪：人感センサー SB412A を 5V のこのボードで読めるか
 *
 * なぜ確かめるのか
 *   SB412A のデータシート（SENBA SB412A-01-013）にはこう書いてある。
 *       Supply Voltage : DC 3.5V〜12V
 *       Voltage Output : High level signal **3V**, Standby 0V or Open-Collector
 *       「Voutput can be as same as DC-Input by adding some components」
 *   つまり **出力の HIGH は電源電圧ではなく 3V 固定**。12V で動かしても 3V しか出ない。
 *   このボードは 5V で動いているので、3V を digitalRead が HIGH と判定するかが
 *   データシートだけでは決まらない（記録にあるのは VIH の「最大」だけ）。
 *   ここを実測して、e2「人感センサー」の配線とコードを決める。
 *
 * つなぎかた
 *   SB412A  DC-INPUT（電源）… 5V
 *   SB412A  GND           … GND
 *   SB412A  VOUT（まん中）  … 6番ピン（＝ A2）
 *   ※ 3本の並びは実物のシルク印刷で確かめること。
 *   ※ 電源を入れてから **1分ほど**は出力が安定しない。落ち着いてから試す。
 *
 * 見かた
 *   【ふだん】基板のオレンジLED（2番）＝ digitalRead(6) の結果そのもの
 *       センサーの前で手を動かして **点けば** → そのまま digitalRead で読める ◎
 *       動かしても **点かなければ** → 3V では HIGH と判定されていない
 *   【10秒に1回】analogRead(A2) の値を知らせる
 *       長く1回光る → そのあと短い点滅が N回
 *       読んだ値 ≒ N × 100 ／ 電圧 ＝ 値 × 5 ÷ 1024
 *       （点滅なし ＝ ほぼ 0V。知らせている約5秒間は上の表示が止まる）
 *
 *   判定のしかた
 *     ・手を動かしたとき N が **6前後（≒3.0V）** なのに LED が点かない
 *         → センサーは正常。3V が VIH に届いていない、が確定。
 *           → USE_PULLUP を true にして、出力がオープンコレクタかを見る。
 *             プルアップで N が **10前後（≒5V）** に上がれば、
 *             外付けプルアップで 5V の合図にできる ＝ 教材はその形で書ける。
 *             変わらなければ押し出し出力なので、analogRead + しきい値で読む。
 *     ・手を動かしていないとき N が 0 に戻るか（戻らなければ配線か個体の問題）
 */

const int LED = 2;                 // 基板のオレンジLED
const int PIR = 6;                 // SB412A の VOUT（A2 と同じピン）

// ふだんは false。LED が点かなかったときだけ true にして、もう一度書きこむ。
const bool USE_PULLUP = false;

unsigned long last = 0;

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(PIR, USE_PULLUP ? INPUT_PULLUP : INPUT);
}

// 長く1回 → 短く n 回。n が読んだ値の 100 のくらい。
void shirase(int n) {
  digitalWrite(LED, HIGH); delay(1200);
  digitalWrite(LED, LOW);  delay(600);
  for (int i = 0; i < n; i++) {
    digitalWrite(LED, HIGH); delay(180);
    digitalWrite(LED, LOW);  delay(320);
  }
  delay(1000);
}

void loop() {
  digitalWrite(LED, digitalRead(PIR));      // ふだんは digitalRead をそのまま映す

  if (millis() - last > 10000) {            // 10秒に1回、電圧を知らせる
    last = millis();
    shirase((analogRead(A2) + 50) / 100);
    last = millis();                        // 知らせに使った時間は数えない
  }
}
