/*
 * v15_motorstep  ---  検証⑮：モーターが「一度きり」になる原因をしぼる
 *
 * ここまでで分かっていること
 *   ・v14 で loop() は回りつづけていると確認（マイコンは正常）
 *   ・e5 では 正転2秒 → 逆転2秒 → 停止1秒 のうち、最初の正転しか回らない
 *
 * このスケッチがすること
 *   **正転だけを、3秒おきに 1秒ずつ** くりかえす（逆転は使わない）。
 *   力の出し方も 3周ごとに変えて、電流のちがいを見る。
 *     1周目〜 … analogWrite 200（e5 と同じ）
 *     つぎ    … analogWrite 120（弱め）
 *     つぎ    … digitalWrite HIGH（全開）
 *   基板のオレンジLED（2番）が、回している1秒のあいだ点灯する。
 *
 * つなぎかた
 *   e5 の配線のまま（モーターもつないだまま）。
 *
 * 見かた
 *   ① 毎回きちんと回る
 *        → 正転だけなら問題なし。**逆転のがわ**（0番の analogWrite）が怪しい
 *          → v13_pwm0 で 0番の PWM を確かめる
 *   ② 1回目だけ回って、あとは回らない
 *        → **ドライバか電池**の問題。出力が止まったまま戻っていない
 *          → USBと電池を両方抜いて10秒待ち、挿し直して1回だけ回るなら
 *            **DRV8835 が保護でラッチしている**（過電流・過熱）
 *          → 電池を新品にすると直るなら **電池の消耗**
 *   ③ 弱め(120)なら回りつづけ、200 や全開だと止まる
 *        → **電流が足りていない**（電池の消耗・接触不良）
 *
 * ⚠ モーターが熱いときは さわらず、電源を抜いてしばらく待つこと。
 */

const int LED  = 2;      // 基板のオレンジLED
const int AIN1 = 12;
const int AIN2 = 0;

int kai = 0;             // 何周目か

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  analogWriteResolution(8);
  digitalWrite(AIN2, LOW);   // 逆転がわは ずっと LOW（正転だけを見る）
}

void loop() {
  digitalWrite(LED, HIGH);              // 回しているあいだ 点灯
  if (kai < 3)      analogWrite(AIN1, 200);
  else if (kai < 6) analogWrite(AIN1, 120);
  else              digitalWrite(AIN1, HIGH);
  delay(1000);

  digitalWrite(AIN1, LOW);              // 出力を切りはなして休む
  digitalWrite(LED, LOW);
  delay(3000);

  kai++;
  if (kai >= 9) kai = 0;
}
