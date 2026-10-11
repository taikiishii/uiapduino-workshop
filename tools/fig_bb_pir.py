# -*- coding: utf-8 -*-
"""e4 人感センサー（AM312 ミニ PIR モジュール）の配線図（上から見た図）。

2026-10-11 に SB412A から差しかえた（Youmile の AM312／HC-SR312・5個入り）。
ピンを下にして**立てて**さす。上から見ると、直径 約12mm の白いレンズ（約5穴ぶん）が
3本のピンの真上にかぶさって見える。レンズは半透明にして、下のピンが見えるようにしてある。

ピンの名前は基板に **VIN・OUT・GND** と印刷されている（商品写真で確認）。
**文字の面を手前（a行がわ）に向けて立てると、左から GND・OUT・VIN**。
（写真：レンズを左・文字を正しく読める向きで、上から VIN・OUT・GND。
  これを右に 90° 回して立てると、上にあった VIN が右へ行く）
⚠ 実物で確かめてから wip を外すこと。

電源は 5V（VIN は 2.7〜12V。基板のレギュレータで約3V に下げてセンサー IC に送る）。
出力の HIGH は約3V。CH32V003 の VIH は 5V 動作で約 2.06V なので digitalRead で読める。
"""
import sys
sys.path.insert(0, "tools")
sys.stdout.reconfigure(encoding="utf-8")
from bb import *
from PIL import Image, ImageDraw

RED, YELLOW, BLACK = (214, 60, 50), (232, 200, 70), (40, 44, 48)
PC = 21                                        # GND の列（OUT＝22・VIN＝23）

im, d = new("人感センサー（AM312）をつなぐ", extra_w=420)
board(d)
mcu(d)

# ボードの 5V・GND を 上の ＋−レールへ
wire(d, (1, "j"), (1, "+t"), RED)
wire(d, (2, "i"), (2, "-t"), BLACK)

# センサーの3本（f行にさす）
wire(d, (PC, "j"), (PC, "-t"), BLACK)          # GND  → −レール
wire(d, (9, "i"), (PC + 1, "i"), YELLOW)       # OUT  → 7番
wire(d, (PC + 2, "j"), (PC + 2, "+t"), RED)    # VIN  → ＋レール（5V）

# LED（e3 と同じ 16・17列）：5番 → LED → 1kΩ → 下の −レール
wire(d, (8, "b"), (16, "b"), RED)
led(d, 16, "b", note=False)
resistor_v(d, 17, "a", "-b")
wire(d, (3, "a"), (3, "-b"), BLACK)

# センサー本体（上から見た形）
mx, my = cx(PC + 1), cy("f")
bw, bt = 10.0 / 2.54 * PITCH, 1.6 / 2.54 * PITCH    # 基板 10mm × 厚み 1.6mm
d.rectangle([mx - bw / 2, my - bt / 2, mx + bw / 2, my + bt / 2],
            fill=(40, 140, 70), outline=(20, 90, 40), width=2)
for i in range(3):
    x = cx(PC + i)
    d.ellipse([x - 8, my - 8, x + 8, my + 8], fill=GOLD, outline=(180, 150, 40), width=2)

# レンズ（半透明）
r = 12.0 / 2.54 / 2 * PITCH                   # レンズ 直径 約12mm
ov = Image.new("RGBA", im.size, (0, 0, 0, 0))
od = ImageDraw.Draw(ov)
od.ellipse([mx - r, my - r, mx + r, my + r], fill=(255, 255, 255, 175),
           outline=(120, 126, 130, 255), width=4)
im = Image.alpha_composite(im.convert("RGBA"), ov).convert("RGB")
d = ImageDraw.Draw(im)
center(d, mx, my + r + 8, "人感センサー", bold(17), INK)

# 説明（右の余白）
x0 = W + 30
d.text((x0, 150), "上から見た形", font=bold(20), fill=INK)
for k, (t, c) in enumerate((("左 … GND → GND", BLACK), ("まん中 … OUT → 7番", (180, 150, 20)),
                            ("右 … VIN → 5V", RED))):
    d.text((x0, 196 + k * 34), t, font=bold(18), fill=c)
d.text((x0, 312), "VIN・OUT・GND の文字の", font=reg(17), fill=GRAY)
d.text((x0, 336), "面を手前に向けてさす", font=reg(17), fill=GRAY)
d.text((x0, 380), "白い丸はレンズ。", font=reg(17), fill=GRAY)
d.text((x0, 404), "ピンの真上にかぶさる", font=reg(17), fill=GRAY)
save(im, "e4_motion", "bb_pir.png")
