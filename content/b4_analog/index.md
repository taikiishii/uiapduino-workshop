---
id: b4
slug: analog
section: 基本編
emoji: 🎛️
num: 4
color: c4
nav_title: ④ つまみで明るさを変えよう
card_title: つまみで明るさを変えよう
desc: 可変抵抗（つまみ）を回すと、LEDの明るさがぬるっと変わるようにしよう。
wip: true
---

{cover}
# ④ つまみで明るさを変えよう

アナログの値を読む

*UIAPduino 体験ワークショップ*

---

# なにをするの？

:::

- `analogRead` で **0〜1023** の値を読む
- デジタル（0か1）とちがって、アナログは**とちゅうの値がある**
- 読んだ値をそのまま明るさにすると、つまみでLEDを調節できる

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

```cpp つまみで明るさを変える
const int VR  = A2;   // 可変抵抗（つまみ）… A2 = PC4
const int LED = 5;

void setup() {
  pinMode(LED, OUTPUT);
  analogWriteResolution(8);
}

void loop() {
  int value = analogRead(VR);        // 0〜1023（10bit）
  analogWrite(LED, value / 4);       // 0〜255 に変える
  delay(10);
}
```
