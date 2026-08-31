---
id: b2
slug: switch
section: 基本編
emoji: 🔘
num: 2
color: c2
nav_title: ② スイッチでLEDをつけよう
card_title: スイッチでLEDをつけよう
desc: ボタンを押したときだけLEDが光るようにしよう。デジタル入力の回。
wip: true
---

{cover}
# ② スイッチでLEDをつけよう

押したときだけ光らせる

*UIAPduino 体験ワークショップ*

---

# なにをするの？

:::

- `digitalRead` で**スイッチの状態を読む**
- `INPUT_PULLUP` を使うと、**押していないとき HIGH・押すと LOW**
- デジタルは **0 か 1 の2つだけ**

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

```cpp スイッチでLEDをつける
const int SW  = 3;   // タクトスイッチ（D3 = PC1）
const int LED = 5;   // LED（D5 = PC3）

void setup() {
  pinMode(SW, INPUT_PULLUP);  // 押していないとき HIGH
  pinMode(LED, OUTPUT);
}

void loop() {
  if (digitalRead(SW) == LOW) {   // 押された！
    digitalWrite(LED, HIGH);
  } else {
    digitalWrite(LED, LOW);
  }
}
```
