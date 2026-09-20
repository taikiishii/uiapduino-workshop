# UIAPduino 体験ワークショップ

**UIAPduino Pro Micro（CH32V003）** を使った、子ども向けの Arduino プログラミング・ワークショップ教材です。
LEDをチカチカさせるところから、センサーとモーターを組み合わせた作品づくりまで進みます。

対象は **小学校高学年〜中学生**。ブロックではなく、**文字（C++）でプログラムを書く**教材です。

## 🌐 公開ページ（GitHub Pages）

**https://taikiishii.github.io/uiapduino-workshop/**

もくじページから各章を開けます。

- 1つのソース（Markdown）から、**Web ページ / 発表スライド / 印刷用 PDF** の3通りに使えます。
  - 各ページ上部の「▶ 発表モード」で全画面プレゼン、「印刷 / PDF」でスライド形式の配布資料になります。
- コードブロックは、パソコンなら右上の「📋 コピー」でワンクリックコピーできます。

## コース構成

教材は共通で、**進むセクションを増やす**ことで開催期間を変えられます。

| コース | 進む範囲 | ゴール |
|---|---|---|
| 半日（3時間） | 導入編 ＋ 基本編 b1〜b4 | スイッチ・つまみでLEDを操作できた |
| 1日（6時間） | ＋ 基本編 b5 ＋ 制御編 | くりかえしと条件を自分で書けた |
| 複数日（3時間×4回） | ＋ 発展編 | センサーで感じてモーターで動く作品を作った |

くわしい章立て・タイムテーブル・部品リストは **[CURRICULUM.md](CURRICULUM.md)** を参照してください。

## 新しい資料を作るには

`content/` に Markdown を書いて `python build.py` を実行するだけで、HTML・QRコード・もくじが自動生成されます。
くわしい手順は **[AUTHORING.md](AUTHORING.md)** を参照してください（ひな形は `content/_template/index.md`）。

## リポジトリ構成（概要）

**1つの章 = 1つのフォルダ**（`content/` と `docs/` が対称）。

- `content/<章>/` … 各章の **`index.md`（編集するのはここ）** と `image/index/`（写真・画面キャプチャ）
- `templates/`, `build.py` … 変換のしくみ
- `docs/<章>/` … `build.py` が生成する `index.html` / `qr.svg` / `image/index/`（GitHub Pages 配信元。直接編集しない）
- `docs/assets/` … 共通部品（`deck.css` / `deck.js` / `hakase.png` / `hakase-face.png` / `qr.svg`）
- `verify/` … **実機検証用の Arduino スケッチ**（教材に書く値を実機で確かめるためのもの。公開教材ではない）
- `CURRICULUM.md` … カリキュラム案・ピン配置・注意点・検証プラン

章フォルダ名の頭は **i=導入編 / b=基本編 / c=制御編 / e=発展編 / p=作品編 / x=付録** ＋ 連番です。

## このボードで気をつけること

教材を書く人向けの要点です（くわしくは [CURRICULUM.md](CURRICULUM.md) §6）。

- **書き込み手順が独特** … RESETを押しながらUSBを挿す → 書き込み → 完了後もう一度RESET
- **シリアルモニタが使えない** … `Serial.print` でのデバッグ前提にしない。値はLEDの明るさで見せる
- **`analogWrite` の既定は 0〜4095** … `setup()` に `analogWriteResolution(8);` を入れて 0〜255 にそろえる
- **PWM が使えるピンは限られる** … **0・2・5・6・12** のみ（実測で確認）
- **アナログが読めるピンも限られる** … **A0・A1・A2・A3・A5** のみ。**A6 は図にあるが読めない**
- **5Vピンは連続 約0.5A まで** … USB の VBUS と直結だが、間のヒューズの溶断電流が 1A
  （公式が「一般的にはその半分が連続定格」と明記）。サーボ・モーターは**外部電源**から取る
- **Flash 16KB / RAM 2KB** … 大きなライブラリは載らない。教材はライブラリなしで組み立てている

## 参考リンク

- **[UIAPduino Pro Micro CH32V003 V1.4 — 公式ページ](https://www.uiap.jp/uiapduino/pro-micro/ch32v003/v1dot4)**
  … 仕様・ピン配置・書き込み手順の**一次情報**。教材に書く事実は必ずここで裏を取ること
- [同ページ：ch32fun 開発環境](https://www.uiap.jp/uiapduino/pro-micro/ch32v003/v1dot4#with-ch32fun-development-environment)
  … Arduino IDE を使わないコマンドライン開発（発展）
- [Arduino IDE のダウンロード](https://www.arduino.cc/en/software)
- [スイッチサイエンス — ボードの購入](https://www.switch-science.com/products/9914)
- ボードマネージャURL：
  `https://github.com/YuukiUmeta-UIAP/board_manager_files/raw/main/package_uiap.jp_index.json`

## ライセンス

文章・レイアウト・プログラムは **MIT ライセンス**（[docs/LICENSE.txt](docs/LICENSE.txt)）で公開しています。
自由に利用・改変・再配布できますが、作成者は一切の責任を負いません（無保証）。

※ 画面キャプチャ・イラスト・写真などの第三者素材は MIT の対象外で、各提供元の利用規約に従います。

※ **ピン配置図**（`content/x3_pinout/image/index/pinout.png` および `content/i1_about/image/index/pinout.png`）は
**Yuuki U.** 氏の著作物で、**CC BY 4.0** で公開されているものです。
再配布・改変の際は、作者名とライセンス（https://creativecommons.org/licenses/by/4.0/ ）の表示を残してください。

Copyright © 2026 Taiki Ishii
