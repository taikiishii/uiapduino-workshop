---
id: e3
slug: distance
section: 発展編
emoji: 📏
num: 3
color: c3
nav_title: ③ ものとの距離をはかろう
card_title: ものとの距離をはかろう
desc: 超音波センサーで距離をはかろう。音がはね返る時間から距離がわかる。
wip: true
---

{cover}
# ③ ものとの距離をはかろう

音のはね返りで距離をはかる

*UIAPduino 体験ワークショップ*

---

# なにをするの？

:::

- 超音波を出して、**はね返ってくるまでの時間**をはかる
- `pulseIn()` でその時間（マイクロ秒）が読める
- 時間 ÷ 58 で、だいたいの距離（cm）になる

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

```cpp 近づいたらLEDが光る
const int TRIG = 5;
const int ECHO = 7;
const int LED  = 8;

void setup() {
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(LED, OUTPUT);
}

void loop() {
  digitalWrite(TRIG, HIGH); delayMicroseconds(10); digitalWrite(TRIG, LOW);
  long t = pulseIn(ECHO, HIGH);      // はね返るまでの時間
  long cm = t / 58;                  // 距離（cm）
  digitalWrite(LED, cm < 20 ? HIGH : LOW);
  delay(100);
}
```
