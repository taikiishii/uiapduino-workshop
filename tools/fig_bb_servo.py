# -*- coding: utf-8 -*-
"""e4 サーボの配線図。力の電気は はじめから電池から取る。

電池は**下の ＋−レール**につなぎ、サーボはそのレールから電気をもらう。
こうすると「電池 → レール → サーボ」の道すじが1本に見える。
合図（黄）だけボードの5番から。GND は ボードもレールにつないで共通にする。
サーボの3本は となりあう列（24・25・26）にいったん落としてから配る。
"""
import sys
sys.path.insert(0, "tools")
sys.stdout.reconfigure(encoding="utf-8")
from bb import *

RED, BROWN, ORANGE, BLACK = (214, 60, 50), (120, 80, 50), (240, 150, 40), (40, 44, 48)
SC = 24                                    # サーボの3本を落とす列（茶・赤・黄）

im, d = new("サーボをつなぐ（力の電気は電池から）", extra_w=460)
board(d)
mcu(d)

sx, sy = 1290, 300
brown, red, orange = servo(d, sx, sy)
bp, bm = battery(d, 1330, 516)

# 電池 → 下の ＋−レール
lead(d, bp[0], bp[1], (27, "+b"), RED)
lead(d, bm[0], bm[1], (30, "-b"), BLACK)

# サーボの3本を となりあう列へ落とす
lead(d, brown[0],  brown[1],  (SC,     "c"), BROWN)
lead(d, red[0],    red[1],    (SC + 1, "c"), RED)
lead(d, orange[0], orange[1], (SC + 2, "c"), ORANGE)

# そこから レールと 5番ピンへ
wire(d, (SC,     "a"), (SC,     "-b"), BLACK)   # 茶 → −レール
wire(d, (SC + 1, "a"), (SC + 1, "+b"), RED)     # 赤 → ＋レール
wire(d, (8, "b"), (SC + 2, "b"), ORANGE)        # 黄 → 5番ピン
wire(d, (3, "a"), (3, "-b"), BLACK)             # ボードのGND → 同じ −レール

center(d, 1435, 648, "電池は下のレールへ。サーボは", reg(17), GRAY)
center(d, 1435, 672, "そのレールから電気をもらう", reg(17), GRAY)
save(im, "e4_servo", "bb_servo.png")
