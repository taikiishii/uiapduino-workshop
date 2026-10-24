# -*- coding: utf-8 -*-
"""e3 人感センサー（AM312 ミニ PIR モジュール）の配線図（上から見た図）。

2026-10-11 に SB412A から差しかえた（Youmile の AM312／HC-SR312・5個入り）。
ピンを下にして**立てて**さす。上から見ると、直径 約12mm の白いレンズ（約5穴ぶん）が
3本のピンの真上にかぶさって見える。レンズは半透明にして、下のピンが見えるようにしてある。

**e2（温度センサー）の配線をそのまま使う**（2026-10-24）。
e2 は 26列＝5V（赤）・27列＝A2（オレンジ）・28列＝GND（黒）。
AM312 は基板に VIN・OUT・GND と印刷されていて、文字の面を手前に向けると左から GND・OUT・VIN、
**文字の面を奥（j行がわ）に向けると左から VIN・OUT・GND**。こうして 26〜28列にさせば、
温度センサーを抜いて差しかえるだけでよい（線は1本も変えない）。
→ OUT は A2（6番）につながる。プログラムは `PIR = A2` で digitalRead する。
✅ 向き・並び・digitalRead(A2) とも実機で確認ずみ（2026-10-24）。

電源は 5V（VIN は 2.7〜12V。基板のレギュレータで約3V に下げてセンサー IC に送る）。
出力の HIGH は約3V。CH32V003 の VIH は 5V 動作で約 2.06V なので digitalRead で読める。
"""
import sys
sys.path.insert(0, "tools")
sys.stdout.reconfigure(encoding="utf-8")
from bb import *
from PIL import Image, ImageDraw

RED, ORANGE, BLACK = (214, 60, 50), (240, 150, 40), (40, 44, 48)
PC = 26                                        # VIN の列（OUT＝27・GND＝28）。e2 の温度センサーと同じ場所

im, d = new("人感センサー（AM312）をつなぐ", extra_w=420)
board(d)
mcu(d)

# e2 と同じ線（1本も変えない）
wire(d, (2, "j"), (2, "-t"), BLACK)            # ボードの GND → −レール
wire(d, (6, "j"), (PC + 1, "j"), ORANGE)       # A2 → 27列（OUT）
wire(d, (PC + 2, "j"), (PC + 2, "-t"), BLACK)  # 28列（GND）→ −レール
wire(d, (1, "i"), (PC, "i"), RED)              # 5V → 26列（VIN）

# LED（e2 と同じ 16・17列）：5番 → LED → 1kΩ → 下の −レール
wire(d, (8, "a"), (16, "a"), RED)
led(d, 16, "c", note=False)
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
for k, (t, c) in enumerate((("左 … VIN → 5V", RED), ("まん中 … OUT → A2", (200, 120, 20)),
                            ("右 … GND → GND", BLACK))):
    d.text((x0, 196 + k * 34), t, font=bold(18), fill=c)
d.text((x0, 312), "VIN・OUT・GND の文字の", font=reg(17), fill=GRAY)
d.text((x0, 336), "面を奥に向けてさす", font=reg(17), fill=GRAY)
d.text((x0, 380), "白い丸はレンズ。", font=reg(17), fill=GRAY)
d.text((x0, 404), "ピンの真上にかぶさる", font=reg(17), fill=GRAY)
save(im, "e3_motion", "bb_pir.png")
