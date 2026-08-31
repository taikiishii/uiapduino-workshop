---
id: b3
slug: pwm
section: 基本編
emoji: 🌗
num: 3
color: c3
nav_title: ③ LEDの明るさを変えよう
card_title: LEDの明るさを変えよう
desc: ついている・消えているだけでなく、とちゅうの明るさにしてみよう。
wip: true
---

{cover}
# ③ LEDの明るさを変えよう

0 と 1 のあいだをつくる（PWM）

*UIAPduino 体験ワークショップ*

---

# なにをするの？

:::

- `analogWrite` で明るさを **0〜255** で指定する
- じつは**ものすごく速く点滅**していて、目には明るさに見えている（PWM）
- ⚠ PWM が使えるピンは決まっている（**D7・D8・D9 は使えない**）

:::

> 📷 ここに写真やスクリーンショットを貼る（VS Code で画像を貼り付けると image/index/ に保存されます）

---

# プログラム

:::

1. Arduino IDE に下のプログラムを入力する
2. **RESETを押しながら**USBを挿して、書き込む
3. 書き込めたら **RESETをもう1回押す**

> 💪 数字を変えると、どう変わるかためしてみよう。

:::

```cpp 明るさを3段階で変える
const int LED = 5;   // PWMが使えるピン（D5 = PC3）

void setup() {
  pinMode(LED, OUTPUT);
  analogWriteResolution(8);   // 明るさを 0〜255 で指定する
}

void loop() {
  analogWrite(LED, 0);    delay(500);   // 消灯
  analogWrite(LED, 64);   delay(500);   // ちょっと明るい
  analogWrite(LED, 255);  delay(500);   // いちばん明るい
}
```
