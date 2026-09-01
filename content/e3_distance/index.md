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

```cpp きょりをはかる
int kyori() {                    // きょりを cm ではかる
  digitalWrite(TRIG, HIGH); delayMicroseconds(10); digitalWrite(TRIG, LOW);
  int w = 0;
  while (digitalRead(ECHO) == LOW) { if (++w > 600) return 999; }
  int n = 0;
  while (digitalRead(ECHO) == HIGH && n < 600) {
    delayMicroseconds(50);       // 音が 1cm 往復する時間
    n++;
  }
  return n - 2;                  // 数えた回数 ＝ ほぼ cm
}
```
