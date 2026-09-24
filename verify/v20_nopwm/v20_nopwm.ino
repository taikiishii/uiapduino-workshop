// v20_nopwm … PWM を使わずに、モーターを正転・逆転させる
//
// 【これまで】
//   ・v19：12番の PWM は安定していた（LED が 2秒点灯・0.5秒消灯を
//     きちんとくりかえした）。→ マイコンの出力は指示どおり
//   ・向き（APHASE）・GND・配線は白
//
// 【この検証】AENBL を PWM ではなく、ただの HIGH／LOW で動かす。
//   ＝ いつも全力。PWM による電流のオン・オフが無くなる。
//
//   ・正転も逆転も力強い → PWM のオン・オフと電源（別USB電源）の相性が原因。
//     USB 電源やモバイルバッテリーは、急な電流の変化で保護が働き、
//     電圧を落としたり出力を止めたりすることがある。
//   ・まだ片方が弱い      → PWM は無関係。モーター自身か電源の容量。
//
// ★ このスケッチでは 12番に analogWrite を一度も使わない。
//   （このコアは analogWrite と digitalWrite を同じピンに混ぜると壊れる）
// ★ 書きこんだあと、念のため USB を抜き差ししてから見る。
// 配線は e5 のまま（APHASE=7・AENBL=12）。

const int APHASE = 7;
const int AENBL  = 12;

void setup() {
  pinMode(APHASE, OUTPUT);
  pinMode(AENBL, OUTPUT);
}

void loop() {
  digitalWrite(APHASE, HIGH); digitalWrite(AENBL, HIGH);  // 片方向・全力
  delay(2000);
  digitalWrite(AENBL, LOW);                               // 止める
  delay(500);
  digitalWrite(APHASE, LOW);  digitalWrite(AENBL, HIGH);  // もう片方・全力
  delay(2000);
  digitalWrite(AENBL, LOW);
  delay(500);
}
