---
id: e2
slug: temp
section: 発展編
emoji: 🌡️
num: 2
color: c2
nav_title: ② 温度をはかろう
card_title: 温度をはかろう
desc: 温度センサーで気温をはかって、暑くなったら知らせるようにしよう。
wip: true
---

{cover}
# ② 温度をはかろう

気温を数字にする

*UIAPduino 体験ワークショップ*

---

# なにをするの？

:::

- 温度センサーは、**温度に応じて電圧が変わる**部品
- `analogRead` の値から、計算で温度（℃）に直す
- しきい値をこえたらLEDやブザーで知らせる

:::

> 📷 ここに写真やスクリーンショットを貼る（VS Code で画像を貼り付けると image/index/ に保存されます）
