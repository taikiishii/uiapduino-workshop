/*
 * v09j_calib  ---  距離の目盛りを校正する（教材に書く定数 K を決める）
 *
 * v09i で「近づくと明るい」ことは確認できた。ただし1回の数えあげにかかる
 * 実時間は delayMicroseconds(10) ＋ digitalRead の時間なので 10us より長い。
 * そこで **決まった距離に物を置いて、そのときの回数を読み取る**。
 *
 * つなぎかた（v09i と同じ）
 *   VCC…5V ／ GND…GND ／ TRIG…5番 ／ ECHO…7番
 *
 * 測りかた
 *   1. センサーの正面 **ちょうど 30cm** のところに、平らなもの（本など）を立てる
 *      ※ センサーの前面から測る。まわりに物が無いところで
 *   2. 基板のオレンジLEDが знач回数を知らせるので、読み取る
 *
 * 読み取りかた（2けたの数字を光で伝える）
 *   【長い点灯】＝ 10のくらい
 *   【短い点滅】＝ 1のくらい
 *   例）長い点灯2回・短い点滅3回 → 23
 *   0 のくらいは点滅なし（例：長い1回・短い0回 → 10）
 *
 *   表示している数は「数えた回数 ÷ 10」。
 *   例）表示 12 なら、数えた回数は 約120回。
 *
 * このあと
 *   30cm のときの反射時間は 30 × 58 ＝ 1740us。
 *   K ＝ 数えた回数 ÷ 30 を計算すれば、教材の kyori() に書く定数が決まる。
 */

const int TRIG = 5;
const int ECHO = 7;
const int LED  = 2;

const int STEP_US   = 10;
const int MAX_COUNT = 3000;

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
  // 10回はかって平均する（ばらつきをならす）
  long sum = 0;
  int ok = 0;
  for (int i = 0; i < 10; i++) {
    int c = measure();
    if (c > 0) { sum += c; ok++; }
    delay(60);
  }

  if (ok == 0) {                 // 反応なし
    digitalWrite(LED, LOW);
    delay(3000);
    return;
  }

  int value = (sum / ok) / 10;   // 数えた回数 ÷ 10 を表示する
  if (value > 99) value = 99;

  delay(1500);                   // 読み取りの準備時間
  blinkLong(value / 10);         // 10のくらい
  delay(600);
  blinkShort(value % 10);        // 1のくらい
  delay(2500);
}
