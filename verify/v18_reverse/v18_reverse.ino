// v18_reverse … 逆転が弱い原因をさがす（3とおりを順に見せる）
//
// 【症状】1ループで **3つの動き**が見える：
//     ① 正方向に強く一瞬   ② 逆方向に強く2秒   ③ 正方向に弱く2秒
//   プログラムは2回しか駆動を指示していないのに、動きが3つある。
//
// 【わかっていること】
//   ・7番の電圧は 正転中 0V／逆転中 5V。ボードは既定5V動作なので
//     どちらも正常な LOW／HIGH に見える。
//     ⚠ ただしテスターは**平均値**を読むので、PWM が出ていても
//       デューティ次第で 0V／5V に近い値に見えることがある。
//   ・配線は正しい（AIN1=APHASE・AIN2=AENBL。TI・秋月とも一致）。
//   ・v04_pwm_scan は **7番にも analogWrite している**。
//   ・CURRICULUM の教訓：このボードでは
//     **analogWrite と digitalWrite を同じピンに混ぜない**。
//     このコアには turnOffPWM が無く、一度 PWM を出したピンは
//     digitalWrite では戻らない（＝ PWM が残る）。
//
// 【仮説】7番に PWM が残っていて、APHASE が「途中の電圧」になっている。
//   向きが定まらないので、弱くなったり一瞬だけ強く回ったりする。
//
// 【使い方】TEST を 1 → 2 → 3 と替えて、それぞれ書きこむ。
//   ★ 書きこんだあと、かならず **USBを抜き差し（電源を入れ直す）**。
//      リセットボタンでは PWM が残ったままのことがある。

#define TEST 1

const int APHASE = 7;
const int AENBL  = 12;

void setup() {
#if TEST == 3
  // ③ 7番を使わず、9番（BPHASE 用の予備）に APHASE を移す
  //    配線も 7番 → 9番 に挿し替えること。
  //    これで直れば、原因は「7番に残った PWM」で確定。
  pinMode(9, OUTPUT);
#else
  pinMode(APHASE, OUTPUT);
#endif
  pinMode(AENBL, OUTPUT);
  analogWriteResolution(8);

#if TEST == 2
  // ② 7番の PWM を明示的に止めてから digital に戻す
  analogWrite(APHASE, 0);
  pinMode(APHASE, OUTPUT);
#endif
}

#if TEST == 3
  #define DIR 9
#else
  #define DIR APHASE
#endif

void loop() {
  digitalWrite(DIR, HIGH); analogWrite(AENBL, 200);   // 片方向
  delay(2000);
  analogWrite(AENBL, 0);                              // 止める
  delay(500);
  digitalWrite(DIR, LOW);  analogWrite(AENBL, 200);   // もう片方
  delay(2000);
  analogWrite(AENBL, 0);
  delay(500);
}
