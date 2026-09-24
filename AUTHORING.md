# 資料の作り方 — UIAPduino 教材のきまり

> **共通の書き方（Markdownの記法・画像の渡し方・1枚に入る量・ビルド手順）は
> [workshop-kit の AUTHORING.md](https://github.com/taikiishii/workshop-kit/blob/main/AUTHORING.md)
> にあります。まずそちらを読んでください**（ローカルなら `../workshop-kit/AUTHORING.md`）。
>
> このファイルには、**この教材だけのきまり**を書いています。

---

## 章フォルダ名とセクション

章フォルダ名の頭は **i=導入編 / b=基本編 / c=制御編 / e=発展編 / m=拡張編 / p=作品編 / x=付録**
＋ 連番（例：`b3_pwm`, `e4_servo`, `p4_power`）。

frontmatter の `section` に書く名前と、その中身：

- `導入編` … Arduino IDE の準備と、はじめてのLチカ
- `基本編` … LED・スイッチ・つまみ（デジタル/アナログの入出力）
- `制御編` … 変数・for・if・関数（プログラムの組み立て方）
- `発展編` … センサーとモーター
- `拡張編` … ライブラリを使う部品（LCD など）。**ライブラリを使うのはこの編だけ**（m は module の m）
- `作品編` … 構想から発表まで。自分だけの作品をつくる
- `付録` … 部品リスト・ピン配置・トラブル対応・はんだ付け・ライブラリ・カラーコード・メンター用早見表・配布用1枚紙

カードの枠色 `color` は **`c1`〜`c5`** をぐるぐる使います。
定義は `site.toml` の `[palette]` と `[[sections]]` にあります。

## コードの書き方

言語は **`cpp`** を使います（`ino` / `arduino` と書いてもOK。表示は「Arduino (C++)」）。

- **字下げは半角スペース2つ**（Arduino IDE の自動整形 `Ctrl+T` に合わせる）
- `pinMode` `digitalWrite` `analogRead` などの Arduino の関数は**青く**表示されます
  （語の一覧は `site.toml` の `[code] namespaces`）
- **`analogWrite` を使うときは `analogWriteResolution(8);` を `setup()` に入れる**
  （このボードの既定は 0〜4095。入れないと 0〜255 のつもりで書いたコードがほとんど光りません）

## このボードで気をつけること

教材に書く事実は、必ず [UIAPduino 公式ページ](https://www.uiap.jp/uiapduino/pro-micro/ch32v003/v1dot4)
で裏を取ってください。実機で確かめた値は [CURRICULUM.md](CURRICULUM.md) §6 にまとまっています。

- **書き込み手順が独特** … RESETを押しながらUSBを挿す → 書き込み → 完了後もう一度RESET
- **シリアルモニタが使えない** … `Serial.print` でのデバッグ前提にしない。値はLEDの明るさで見せる
- **PWM が使えるピンは 2・5・6・12 のみ**（実測で確認。公式図は 0 も可とするが、実機では無反応）
- **アナログが読めるピンは A0・A1・A2・A3・A5 のみ**。**A6 は図にあるが読めない**
- **5Vピンは連続 約0.5A まで** … サーボ・モーターは**外部電源**から取る
- **Flash 16KB / RAM 2KB** … 大きなライブラリは載らない。拡張編より前はライブラリなしで組み立てている

## 写真の撮り方

- **配線の写真**は、ジャンパーの色と挿す位置がはっきり分かる角度で撮る。真上から撮るのが基本
- 名前は順番がわかるように `01_wiring.png` `02_ide.png` … と付ける

## このリポジトリだけのフォルダ

- `verify/` … **実機検証用の Arduino スケッチ**（教材に書く値を実機で確かめるためのもの。公開教材ではない）
- `tools/` … 配線図（ブレッドボード図）を SVG で描く Python スクリプト
- `CURRICULUM.md` … カリキュラム案・タイムテーブル・部品リスト・検証プラン
- `content/_index/refs.html` … もくじ下の「📚 参考リンク」
- `content/_index/license.html` … ピン配置図（CC BY 4.0）など第三者素材の注記
