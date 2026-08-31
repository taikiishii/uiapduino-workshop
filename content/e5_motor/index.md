---
id: e5
slug: motor
section: 発展編
emoji: ⚙️
num: 5
color: c5
nav_title: ⑤ モータードライバでDCモーター
card_title: モータードライバでDCモーター
desc: モーターをぐるぐる回そう。正転・逆転・スピード調節までできるよ。
wip: true
---

{cover}
# ⑤ モータードライバでDCモーター

回す・逆に回す・速さを変える

*UIAPduino 体験ワークショップ*

---

# なにをするの？

:::

- モーターは電気をたくさん使うので、**マイコンに直接つないではいけない**
- **モータードライバ**という部品にお願いして回してもらう
- 2本のピンの組み合わせで、**正転・逆転・停止**が決まる

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

```cpp 正転と逆転をくりかえす
const int AIN1 = 5;   // どちらも PWM が使えるピン
const int AIN2 = 6;   // （PWM 可は 0・2・5・6・12 の5本だけ）

void setup() {
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  analogWriteResolution(8);
}

void loop() {
  analogWrite(AIN1, 200);  digitalWrite(AIN2, LOW);   // 正転
  delay(2000);
  digitalWrite(AIN1, LOW); analogWrite(AIN2, 200);    // 逆転
  delay(2000);
  digitalWrite(AIN1, LOW); digitalWrite(AIN2, LOW);   // 停止
  delay(1000);
}
```
