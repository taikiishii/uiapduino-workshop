---
id: e1
slug: light
section: 発展編
emoji: 🔦
num: 1
color: c1
nav_title: ① 光センサーで暗くなったら光る
card_title: 光センサーで暗くなったら光る
desc: 明るさをはかるセンサーをつないで、自動で点灯するライトを作ろう。
wip: true
---

{cover}
# ① 光センサーで暗くなったら光る

くらさを感じて自動で光る

*UIAPduino 体験ワークショップ*

---

# なにをするの？

:::

- **CdS（光センサー）** は、明るさによって抵抗が変わる部品
- 抵抗と組み合わせて `analogRead` で明るさを数字にする
- しきい値より暗くなったらLEDをつける ＝ **自動ライト**

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

```cpp 暗くなったら光る
const int CDS = A2;   // 光センサー（CdS + 10kΩ の分圧）
const int LED = 5;

void setup() {
  pinMode(LED, OUTPUT);
}

void loop() {
  int light = analogRead(CDS);
  if (light < 300) {              // 暗くなったら
    digitalWrite(LED, HIGH);
  } else {
    digitalWrite(LED, LOW);
  }
  delay(50);
}
```
