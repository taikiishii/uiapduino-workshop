// v21_pwmfreq_motor … PWM の周波数を上げると、モーターが安定するか
//
// 【これまで】
//   ・v19：マイコンの PWM 出力は正常
//   ・v20：PWM を使わず全力（HIGH）なら、**両方向とも力強い**
//   → 問題は「PWM」と「モーター用の別USB電源」の組みあわせ。
//
// 【仮説】既定の PWM は 1000Hz。モーターのコイルは 1000Hz では電流を
//   ならしきれず、電源から見ると「大きな電流が1秒に1000回オン・オフ」する。
//   USB 電源はこれで保護が働いたり電圧が揺れたりして、強さが勝手に変わる。
//   「ピー」という音は、この 1000Hz そのもの。
//   20000Hz（20kHz）にすると、コイルが電流をならしてくれるので、電源から
//   見るとほぼ一定の電流になる。耳にも聞こえなくなる。
//   DRV8835 は 0〜250kHz の PWM を受けつける（TI データシート §6.3）。
//
// 【使い方】TEST を 1 → 2 と替えて書きこむ。配線は e5 のまま。
//   TEST 1 … 1000Hz（既定）。いまの症状が出るはず
//   TEST 2 … 20000Hz。両方向とも力強く、ピー音も消えれば確定
// ★ 書きこんだあと、念のため USB を抜き差ししてから見る。

#define TEST 2

const int APHASE = 7;
const int AENBL  = 12;

void setup() {
  pinMode(APHASE, OUTPUT);
  pinMode(AENBL, OUTPUT);
  analogWriteResolution(8);
#if TEST == 2
  analogWriteFrequency(20000);   // ★ 20kHz にする
#endif
}

void loop() {
  digitalWrite(APHASE, HIGH); analogWrite(AENBL, 200);  // 片方向
  delay(2000);
  analogWrite(AENBL, 0);
  delay(500);
  digitalWrite(APHASE, LOW);  analogWrite(AENBL, 200);  // もう片方
  delay(2000);
  analogWrite(AENBL, 0);
  delay(500);
}
