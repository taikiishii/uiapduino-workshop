# -*- coding: utf-8 -*-
"""e2 温度センサー（MCP9701）の配線図。

**e1 の A2 と GND の線をそのまま使う**（2026-10-24）。
温度センサーの足は、平らな面を手前にして左から VDD・OUT・GND。
これを 26〜28列（e1 でつまみがあった場所）にさすと、
  OUT → 27列（e1 の A2 のオレンジ ＝ 6列 → 27列）
  GND → 28列（e1 の黒い線 ＝ 28列 → −レール）
にぴったり合う。足は f 行にさす（本体が上に描かれるので、j 行の線とぶつからない）。

e1 からの変えかた：つまみ・CdS・21→27列の短いオレンジを抜き、
5V の赤い線を 1列 → 26列（i 行）に引きなおす。LED（5番）はそのまま。
"""
import sys
sys.path.insert(0, "tools")
sys.stdout.reconfigure(encoding="utf-8")
from bb import *

RED, ORANGE, BLACK = (214, 60, 50), (240, 150, 40), (40, 44, 48)

im, d = new("温度センサーをつなぐ")
board(d)
mcu(d)

wire(d, (2, "j"), (2, "-t"), BLACK)        # ボードの GND → −レール（e1 と同じ）
wire(d, (6, "j"), (27, "j"), ORANGE)       # A2 → 27列（e1 と同じ線）
wire(d, (28, "j"), (28, "-t"), BLACK)      # 28列 → −レール（e1 と同じ線）
wire(d, (1, "i"), (26, "i"), RED)          # 5V → 26列（引きなおす）
to92(d, 26, "f")                           # VDD 26・OUT 27・GND 28

# LED（e1 と同じ 16・17列）
wire(d, (8, "a"), (16, "a"), RED)          # 5番 → LED
led(d, 16, "c", note=False)
resistor_v(d, 17, "a", "-b")
wire(d, (3, "a"), (3, "-b"), BLACK)        # ボードのGND → 下の −レール
save(im, "e2_temp", "bb_temp.png")
