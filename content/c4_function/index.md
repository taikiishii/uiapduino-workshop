---
id: c4
slug: function
section: 制御編
emoji: 🧩
num: 4
color: c4
nav_title: ④ 配列と関数でまとめよう
card_title: 配列と関数でまとめよう
desc: たくさんのLEDをひとまとめに。同じ処理は関数にまとめてすっきりさせよう。
wip: true
---

{cover}
# ④ 配列と関数でまとめよう

ばらばらの命令をまとめる

*UIAPduino 体験ワークショップ*

---

# なにをするの？

:::

- **配列**を使うと、たくさんのピン番号を1つの名前でまとめられる
- **関数**を作ると、同じ処理を何回も書かなくてよくなる
- `setup()` や `loop()` も、じつは関数のなかま

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

```cpp 3個のLEDを順番に光らせる
const int leds[] = {5, 7, 8};   // 3個のLED
const int N = 3;

void setup() {
  for (int i = 0; i < N; i++) {
    pinMode(leds[i], OUTPUT);
  }
}

void loop() {
  for (int i = 0; i < N; i++) {   // 順番に流れる
    digitalWrite(leds[i], HIGH);
    delay(150);
    digitalWrite(leds[i], LOW);
  }
}
```
