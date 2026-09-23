# -*- coding: utf-8 -*-
"""e5 モータードライバの端子の拡大図。

配線図（bb_drv8835.png）と**同じ向き・同じ色**。
端子は基板の**内がわ**にあり、名前はそのすぐ外に印刷されている
（実物のモジュール基板と同じ見え方）。
ピン番号は資料 AE-DRV8835-S のとおり。
"""
import sys
sys.path.insert(0, "tools")
sys.stdout.reconfigure(encoding="utf-8")
from fig import *

GREEN, GREEN_E = (22, 168, 96), (18, 118, 70)
GOLD, GOLD_E   = (240, 196, 86), (180, 150, 40)
SIG, PWR       = (0, 118, 110), (214, 92, 40)

TOP = [("VCC", 12, "5V へ"), ("MODE", 11, "GND へ"), ("AIN1", 10, "12番"),
       ("AIN2", 9, "0番"), ("BIN1", 8, "―"), ("BIN2", 7, "―")]
BOT = [("VM", 1, "電池＋"), ("AOUT1", 2, "モーター"), ("AOUT2", 3, "モーター"),
       ("BOUT1", 4, "―"), ("BOUT2", 5, "―"), ("GND", 6, "GND へ")]

P = 150                                     # 端子の間かく
X0 = (1340 - 5 * P) / 2                     # 左はしの端子（中央ぞろえ）
im, d = canvas(1340, 640)
center(d, 670, 8, "DRV8835 の端子（配線図と同じ向き）", bold(28), INK)

x1, x2 = X0 - 74, X0 + 5 * P + 74           # 基板。端子より外まで広げる
yt, yb = 218, 400                           # 端子の行
d.rounded_rectangle([x1, yt - 62, x2, yb + 62], radius=10,
                    fill=GREEN, outline=GREEN_E, width=5)
center(d, 670, 288, "DRV8835", bold(46), WHITE)

for items, yy, col, name_dy, use_y in ((TOP, yt, SIG, -34, 130),
                                       (BOT, yb, PWR, 20, 500)):
    for i, (lab, num, use) in enumerate(items):
        x = X0 + i * P
        d.ellipse([x - 21, yy - 21, x + 21, yy + 21], fill=GOLD,
                  outline=GOLD_E, width=3)
        d.ellipse([x - 8, yy - 8, x + 8, yy + 8], fill=(250, 250, 248))
        middle(d, x, yy, str(num), bold(15), (120, 96, 28))      # ピン番号
        center(d, x, yy + name_dy, lab, bold(19), WHITE)         # 基板の印刷
        center(d, x, use_y, use, bold(18), col if use != "―" else GRAY)

center(d, 670, 88, "上の行 ＝ 合図（ボードとつなぐ）", bold(22), SIG)
center(d, 670, 538, "下の行 ＝ 力（電池とモーター）", bold(22), PWR)
d.rounded_rectangle([180, 580, 1160, 626], radius=10, fill=SOFT, outline=TEAL, width=3)
middle(d, 670, 603, "丸の中の数字はピン番号。B側（BIN・BOUT）は 2つめのモーター用",
       bold(20), DARKTEAL)
save(im, "e5_motor", "drv8835_pins.png")
print("  比 %.2f" % (1340 / 640))
