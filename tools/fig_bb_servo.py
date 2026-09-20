# -*- coding: utf-8 -*-
"""e4 サーボの配線図。力の電気は はじめから電池から取る。
   合図だけボードから出し、GND はボード側の − レールでつなぐ。"""
import sys
sys.path.insert(0, "tools")
sys.stdout.reconfigure(encoding="utf-8")
from bb import *

RED, BROWN, ORANGE, BLACK = (214, 60, 50), (120, 80, 50), (240, 150, 40), (40, 44, 48)

im, d = new("サーボをつなぐ（力の電気は電池から）", extra_w=460)
board(d)
mcu(d)

# ボードの GND を 上下どちらの − レールにも出しておく
wire(d, (2, "j"), (2, "-t"), BLACK)
wire(d, (3, "a"), (3, "-b"), BLACK)

sx, sy = 1290, 300
brown, red, orange = servo(d, sx, sy)
bp, bm = battery(d, 1330, 516)

lead(d, brown[0], brown[1], (20, "-t"), BROWN)     # 茶 … GND（− レールへ）
d.line([red[0], red[1], bp[0] + 6, bp[1]], fill=WHITE, width=16)
d.line([red[0], red[1], bp[0] + 6, bp[1]], fill=RED, width=8)
lead(d, bm[0], bm[1], (26, "-b"), BLACK)           # 電池の − … 下の − レールへ
lead(d, orange[0], orange[1], (8, "a"), ORANGE)    # 黄 … 5番ピン

center(d, 1435, 644, "ジャンパーワイヤー（オス−メス）で", reg(17), GRAY)
center(d, 1435, 668, "さす場所まで そのままのばす", reg(17), GRAY)
save(im, "e4_servo", "bb_servo.png")
