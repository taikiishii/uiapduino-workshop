# -*- coding: utf-8 -*-
"""e5 モータードライバの配線図。
   VCC はロジックの基準電圧。モジュール資料に「入力は 0〜Vcc、
   絶対最大 Vcc+0.5V」とあるので、5V で動くこのボードにつなぐなら
   VCC は 3V3 ではなく 5V。"""
import sys
sys.path.insert(0, "tools")
sys.stdout.reconfigure(encoding="utf-8")
from bb import *

RED, ORANGE, YELLOW, BLACK = (214, 60, 50), (240, 150, 40), (232, 200, 70), (40, 44, 48)
DIPCOL = 20

im, d = new("モータードライバ（DRV8835）をつなぐ", extra_w=420)
board(d)
mcu(d)
dip(d, DIPCOL, ["BIN2", "BIN1", "AIN2", "AIN1", "MODE", "VCC"],
                ["GND", "BOUT2", "BOUT1", "AOUT2", "AOUT1", "VM"])

# 合図の線。5V を j行にすると col2 の GND ジャンパーと交差して見えるので h行に置く
wire(d, (5, "j"), (23, "j"), ORANGE)     # A3（12番）→ AIN1
wire(d, (7, "i"), (22, "i"), YELLOW)     # A1（0番） → AIN2
wire(d, (1, "h"), (25, "h"), RED)        # 5V        → VCC
wire(d, (24, "g"), (28, "g"), BLACK)     # MODE      → GND（IN/INモード）
wire(d, (28, "g"), (28, "-t"), BLACK)
wire(d, (2, "j"), (2, "-t"), BLACK)      # ボードの GND → 上の − レール
wire(d, (3, "a"), (3, "-b"), BLACK)      # ボードの GND → 下の − レール
wire(d, (DIPCOL, "b"), (DIPCOL, "-b"), BLACK)   # ドライバの GND → 下の − レール

# 力の線
MX, MY = 1450, 320
motor(d, MX, MY)
lead(d, MX - 66, MY + 40, (23, "b"), (176, 180, 182))
lead(d, MX - 40, MY + 66, (24, "b"), (176, 180, 182))
bp, bm = battery(d, 1330, 486)
lead(d, bp[0], bp[1], (25, "a"), RED)
lead(d, bm[0], bm[1], (28, "-b"), BLACK)

save(im, "e5_motor", "bb_drv8835.png")
