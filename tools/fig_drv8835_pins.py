# -*- coding: utf-8 -*-
"""e5 モータードライバの端子の拡大図。

配線図（bb_drv8835.png）と**同じ向き・同じ色**にしてある。
上の行が合図、下の行が力。ピン番号は資料 AE-DRV8835-S のとおり。
"""
import sys
sys.path.insert(0, "tools")
sys.stdout.reconfigure(encoding="utf-8")
from fig import *

GREEN, GREEN_E = (28, 110, 70), (16, 80, 50)
GOLD, GOLD_E   = (240, 196, 86), (180, 150, 40)
SIG, PWR       = (0, 118, 110), (214, 92, 40)

TOP = [("VCC", 12, "5V へ"), ("MODE", 11, "GND へ"), ("AIN1", 10, "12番"),
       ("AIN2", 9, "0番"), ("BIN1", 8, "―"), ("BIN2", 7, "―")]
BOT = [("VM", 1, "電池＋"), ("AOUT1", 2, "モーター"), ("AOUT2", 3, "モーター"),
       ("BOUT1", 4, "―"), ("BOUT2", 5, "―"), ("GND", 6, "GND へ")]

P, X0 = 150, 200                            # 端子の間かく ／ 左はし
im, d = canvas(1300, 660)
center(d, 650, 10, "DRV8835 の端子（配線図と同じ向き）", bold(28), INK)

x1, x2 = X0 - 54, X0 + 5 * P + 54
y1, y2 = 212, 448
d.rounded_rectangle([x1, y1, x2, y2], radius=14, fill=GREEN, outline=GREEN_E, width=4)
center(d, (x1 + x2) / 2, (y1 + y2) / 2 - 16, "DRV8835", bold(30), (150, 205, 175))

for row, items, yy, col, note_y in ((0, TOP, y1, SIG, 100), (1, BOT, y2, PWR, 536)):
    for i, (lab, num, use) in enumerate(items):
        x = X0 + i * P
        d.ellipse([x - 20, yy - 20, x + 20, yy + 20], fill=GOLD, outline=GOLD_E, width=3)
        middle(d, x, yy, str(num), bold(19), (90, 72, 20))     # ピン番号
        dy = -52 if row == 0 else 32
        center(d, x, yy + dy, lab, bold(22), WHITE if False else INK)
        center(d, x, note_y, use, bold(17),
               col if use != "―" else GRAY)

center(d, 650, 58, "上の行 ＝ 合図（ボードとつなぐ）", bold(22), SIG)
center(d, 650, 576, "下の行 ＝ 力（電池とモーター）", bold(22), PWR)
d.rounded_rectangle([170, 610, 1130, 654], radius=10, fill=SOFT, outline=TEAL, width=3)
middle(d, 650, 632, "丸の中の数字はピン番号。B側（BIN・BOUT）は 2つめのモーター用",
       bold(21), DARKTEAL)
save(im, "e5_motor", "drv8835_pins.png")
print("  比 %.2f" % (1300 / 660))
