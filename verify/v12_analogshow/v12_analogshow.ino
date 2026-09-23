/*
 * v12_analogshow  ---  analogRead の値を、そのまま数字で知らせる道具
 *
 * 何のため
 *   「センサーの値がおかしい」ときに、まず **生の値** を知るための道具。
 *   このボードはシリアルモニタが使えないので、基板のオレンジLED（2番）で
 *   1けたずつ知らせる。
 *
 * つなぎかた
 *   調べたいセンサーの出力 … 6番ピン（＝ A2）
 *   （ほかのピンを見たいときは PIN と APIN を書きかえる）
 *
 * 見かた
 *   長く1回光る … これから数字を知らせるよ
 *   そのあと 3けたぶん、短い点滅がつづく（百のくらい → 十のくらい → 一のくらい）
 *   けたの区切りは 1秒の消灯。**0 のけたは「すばやい2回の点滅」**で表す。
 *
 *   例）182 なら … ■  ／ ■（1回） ／ ■■■■■■■■（8回） ／ ■■（2回）
 *
 * 温度センサー（e2）の読みかた … 室温25℃のときの目安
 *   154 前後 … MCP9700 で正常。教材の式でよい
 *   182 前後 … **MCP9701**（0℃=400mV・19.5mV/℃）。式を変える必要がある
 *   233 前後 … ボードが 3.3V 動作になっている（ADC の基準が式と食いちがう）
 *   1023    … 出力が電源に張りついている（配線か向きを疑う）
 *   毎回ばらばら … どこにもつながっていない（浮いている）
 */

const int LED  = 2;      // 基板のオレンジLED
const int APIN = A2;     // 調べたいアナログピン（A2 ＝ 6番）

void keta(int n) {       // 1けたぶんを点滅で知らせる
  if (n == 0) {          // 0 は すばやい2回
    for (int i = 0; i < 2; i++) {
      digitalWrite(LED, HIGH); delay(70);
      digitalWrite(LED, LOW);  delay(120);
    }
  } else {
    for (int i = 0; i < n; i++) {
      digitalWrite(LED, HIGH); delay(200);
      digitalWrite(LED, LOW);  delay(300);
    }
  }
  delay(1000);           // けたの区切り
}

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(APIN, INPUT);
}

void loop() {
  int v = analogRead(APIN);

  digitalWrite(LED, HIGH); delay(1500);   // 「これから知らせるよ」
  digitalWrite(LED, LOW);  delay(1000);

  keta(v / 100);
  keta((v / 10) % 10);
  keta(v % 10);

  delay(2500);                            // ひと呼吸おいて、もう一度
}
