---
id: x10
slug: ubuntu
section: 付録
emoji: 🐧
num: 10
color: c5
nav_title: ⑩ Ubuntu で使う
card_title: Ubuntu で使う
desc: Ubuntu 26.04 に Arduino IDE 2 を入れて、ランチャーから起動し、UIAPduino に書きこめるようにする。セットアップする大人むけ。
wip: 実機確認まち
---

{cover}
# ⑩ Ubuntu で使う

Arduino IDE 2 を入れて、UIAPduino に書きこむまで

*セットアップする大人むけ*

---

# この章について
**Ubuntu 26.04** のパソコンで使うための準備です。端末（ターミナル）でコマンドを順に実行します。

1. AppImage を動かす準備（FUSE）
2. Arduino IDE をダウンロード
3. `--no-sandbox` で起動する
4. ランチャーに登録する
5. UIAPduino のボードを追加する（導入編②と同じ）
6. 書きこみの許可（udev）を設定する

> Ubuntu 24.04 以降は、ほぼ同じ手順です。管理者のパスワード（`sudo`）が要ります。

---

# 1. AppImage を動かす準備

:::

Arduino IDE は **AppImage** という1つのファイルで配られています。動かすには **FUSE 2** が要ります。

- Ubuntu 24.04 以降の名前は `libfuse2t64`
- 22.04 では `libfuse2`

> FUSE 2 は、最初から入っている FUSE 3 と**いっしょに使えます**。

:::

```bash 1. FUSE 2 を入れる
sudo apt update
sudo apt install libfuse2t64
```

---

# 2. Arduino IDE をダウンロード

:::

ホームの `Applications` フォルダに置きます。

- 2.3.10 は 2026年9月時点の最新版
- 新しい版が出ていたら、番号を読みかえる
- `chmod +x` で「実行してよい」印をつける

:::

```bash 2. ダウンロードして実行できるようにする
mkdir -p ~/Applications
cd ~/Applications
wget https://downloads.arduino.cc/arduino-ide/arduino-ide_2.3.10_Linux_64bit.AppImage
chmod +x arduino-ide_2.3.10_Linux_64bit.AppImage
```

---

# 3. --no-sandbox で起動する

:::

Ubuntu 24.04 以降は **AppArmor** の制限で、そのままでは起動の途中で止まります。

- 起動するときに `--no-sandbox` をつける
- 外れるのは Arduino IDE の中の保護だけ

> ⚠ ネットで見かける `kernel.unprivileged_userns_clone=1` は、**この制限には効きません**。

:::

```bash 3. 起動してみる
~/Applications/arduino-ide_2.3.10_Linux_64bit.AppImage --no-sandbox
```

---

# 4. ランチャーに登録する（1）

:::

毎回コマンドを打たなくていいように、**起動用のスクリプト**を作ります。

- `--no-sandbox` をつけて IDE を起動するだけの中身
- `'EOF'` と引用符でかこむと、`$HOME` がそのまま書きこまれる

:::

```bash 4-1. 起動用のスクリプトを作る
cat << 'EOF' > ~/Applications/launch-arduino.sh
#!/bin/bash
exec "$HOME/Applications/arduino-ide_2.3.10_Linux_64bit.AppImage" --no-sandbox "$@"
EOF
chmod +x ~/Applications/launch-arduino.sh
```

---

# 4. ランチャーに登録する（2）

:::

アプリの一覧に出す**項目**（`.desktop`）とアイコンを作ります。

- `.desktop` では `$HOME` が使えないので、`EOF` を引用符でかこまず**作るときに置きかえる**
- アイコンは AppImage の**中から取り出す**

> 一覧に出ないときは、ログインしなおしてください。

:::

```bash 4-2. ランチャーの項目とアイコンを作る
mkdir -p ~/.local/share/icons ~/.local/share/applications
(cd /tmp && ~/Applications/arduino-ide_2.3.10_Linux_64bit.AppImage --appimage-extract 'usr/share/icons/*' > /dev/null)
cp /tmp/squashfs-root/usr/share/icons/hicolor/512x512/apps/arduino-ide.png ~/.local/share/icons/
rm -rf /tmp/squashfs-root
cat << EOF > ~/.local/share/applications/arduino-ide.desktop
[Desktop Entry]
Type=Application
Name=Arduino IDE
Exec=$HOME/Applications/launch-arduino.sh
Icon=$HOME/.local/share/icons/arduino-ide.png
Terminal=false
Categories=Development;Engineering;
StartupWMClass=arduino ide
EOF
update-desktop-database ~/.local/share/applications
```

---

# 5. UIAPduino のボードを追加する

:::

ここからは**導入編②**の手順②〜⑤と同じです。

1. 日本語にする
2. 追加のボードマネージャに、右の URL を貼る
3. ボードマネージャで **UIAPduino** を入れる
4. ボードに **Pro Micro CH32V003** をえらぶ

:::

```
https://github.com/YuukiUmeta-UIAP/board_manager_files/raw/main/package_uiap.jp_index.json
```

---

# 6. 書きこみの許可（udev）

:::

Linux では、USB 機器を使うのに**許可**が要ります。UIAPduino の**公式ページ**にある設定を入れます。

- 書きこみ口は **1209:b803**（HID）
- `plugdev` グループの人が使えるようになる

> ⚠ `1a86`・`4348` は WCH の書き込み器の番号です。UIAPduino の書きこみには**使われません**。

:::

```bash 6. 公式の udev ルールを入れる
sudo wget -O /etc/udev/rules.d/99-minichlink-uiap.rules https://raw.githubusercontent.com/YuukiUmeta-UIAP/ch32fun/3bfa603f11d493710f2a811b5a2dfad905d9425c/minichlink/99-minichlink-uiap.rules
sudo udevadm control --reload-rules
sudo udevadm trigger
groups    # 表示に plugdev があるか確かめる
```

---

# 書きこんでみよう

1. **RESET を押しながら** USB をさす（書きこみ待ちになる）
2. IDE の「書き込み」ボタンを押す
3. 最後に **`Image written.`** と出れば成功
4. もう一度 **RESET** を押すと、プログラムが動きはじめる

> ⚠ 途中で `ioctl (GFEATURE): Protocol error` と出ることがあります。
> 最後に `Image written.` があれば、書きこめています。

---

# うまくいかないとき

- **起動しない** → 端末から起動して、エラーを読む（FUSE → 1 ／ サンドボックス → 3）
- **アプリの一覧に出ない** → ログインしなおす
- **書きこみで止まる** → RESET を押しながら USB をさしなおす
- **Permission denied** → `groups` に `plugdev` がなければ、`sudo usermod -aG plugdev $USER` のあとログインしなおす

> このボードはシリアルモニタを使わないので、`dialout` グループは要りません。
