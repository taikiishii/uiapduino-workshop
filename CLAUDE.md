# UIAPduino 体験ワークショップ

共通ルール（すべての教材で同じ）はこちら:

@../workshop-kit/CLAUDE.md

上の読み込みが効かない環境のために、要点だけ再掲します。

- **編集してよいのは `content/` と `site.toml` だけ。** `docs/` は `python build.py` の生成物。
- `docs/assets/` の共通部品と `.gitignore` `.vscode/settings.json` は **workshop-kit が配る**。
  直すときは `../workshop-kit/wskit/` を直す（＝すべての教材に効く）。
- **表（`| … | … |`）はスライドで使えない。** 箇条書きにする。
- コミットメッセージは日本語。`docs/` もコミットに含める。

---

## この教材のきまり

書き方の詳細は [AUTHORING.md](AUTHORING.md)。とくに：

- 章フォルダ名の頭 … **i=導入編 / b=基本編 / c=制御編 / e=発展編 / p=作品編 / x=付録**
- コードの言語は **`cpp`**、字下げは**半角スペース2つ**（Arduino IDE の `Ctrl+T` に合わせる）
- カードの枠色は `c1`〜`c5`

## 事実の裏取り

ピン番号・電流・仕様を書くときは、**必ず**一次情報で確認する。

- [UIAPduino Pro Micro CH32V003 V1.4 公式ページ](https://www.uiap.jp/uiapduino/pro-micro/ch32v003/v1dot4)
- 実機で確かめた値と注意点 … [CURRICULUM.md](CURRICULUM.md) §6
- 実機検証用スケッチ … `verify/`（教材ではない。公開しない）

とくに間違えやすい点：

- **PWM が使えるピンは 0・2・5・6・12 のみ**
- **アナログが読めるピンは A0・A1・A2・A3・A5 のみ。A6 は図にあるが読めない**
- **`analogWrite` の既定は 0〜4095**。`setup()` に `analogWriteResolution(8);` を入れて 0〜255 にそろえる
- **シリアルモニタが使えない**。`Serial.print` を前提にした説明を書かない
- **5Vピンは連続 約0.5A まで**。サーボ・モーターは外部電源から取る
