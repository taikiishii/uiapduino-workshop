/*
 * v09k_calib2  ---  校正の仕上げ：「数えた回数 ＝ そのまま cm」にする
 *
 * v09j の結果（30cm で約75回・STEP_US = 10）から、
 * 1回の数えあげにかかる実時間は約 23us と分かった。
 *   → 内訳：delayMicroseconds(10) の 10us ＋ digitalRead などの約 13us
 *
 * 音が 1cm 往復するのにかかる時間は 58us。
 * そこで待ち時間を **45us** にすれば、1回 ≒ 45 + 13 = 58us となり、
 * **数えた回数がそのままセンチメートル**になる。
 *   → 教材のコードから割り算が消える（子どもに説明しやすい）
 *
 * つなぎかた（v09j と同じ）
 *   VCC…5V ／ GND…GND ／ TRIG…5番 ／ ECHO…7番
 *
 * 測りかた
 *   センサーの正面 **ちょうど 30cm** に平らなものを立てる
 *
 * 読み取りかた（今回は「数えた回数」をそのまま2けたで表示）
 *   【長い点灯】＝ 10のくらい　【短い点滅】＝ 1のくらい
 *   例）長い3回・短い0回 → 30
 *
 * 判定
 *   28〜32 → ✅ ちょうどよい。教材は「回数 ＝ cm」で書ける
 *   それ以外 → ずれた量に応じて STEP_US を増減して再測定
 *              （表示が大きすぎる＝1回が短すぎる → STEP_US を増やす）
 */

const int TRIG = 5;
const int ECHO = 7;
const int LED  = 2;

const int STEP_US   = 50;     // ★ ここを調整する
const int MAX_COUNT = 600;    // 600回 ≒ 600cm ぶん（十分）

void blinkLong(int n) {
  for (int i = 0; i < n; i++) {
    digitalWrite(LED, HIGH); delay(600);
    digitalWrite(LED, LOW);  delay(400);
  }
}

void blinkShort(int n) {
  for (int i = 0; i < n; i++) {
    digitalWrite(LED, HIGH); delay(120);
    digitalWrite(LED, LOW);  delay(280);
  }
}

int measure() {
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  int guard = 0;
  while (digitalRead(ECHO) == LOW && guard < MAX_COUNT) {
    delayMicroseconds(STEP_US); guard++;
  }
  if (guard >= MAX_COUNT) return 0;

  int count = 0;
  while (digitalRead(ECHO) == HIGH && count < MAX_COUNT) {
    delayMicroseconds(STEP_US); count++;
  }
  return count;
}

void setup() {
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(LED, OUTPUT);
  digitalWrite(TRIG, LOW);
}

void loop() {
  long sum = 0;
  int ok = 0;
  for (int i = 0; i < 10; i++) {
    int c = measure();
    if (c > 0) { sum += c; ok++; }
    delay(60);
  }

  if (ok == 0) {
    digitalWrite(LED, LOW);
    delay(3000);
    return;
  }

  int value = sum / ok;          // 今回は「数えた回数」をそのまま表示
  if (value > 99) value = 99;

  delay(1500);
  blinkLong(value / 10);         // 10のくらい
  delay(600);
  blinkShort(value % 10);        // 1のくらい
  delay(2500);
}
