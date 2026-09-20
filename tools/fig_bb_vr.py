# -*- coding: utf-8 -*-
"""b4 つまみ（可変抵抗）の配線図。
   ADC の基準は VDD＝5V なので、つまみの上は 3V3 ではなく 5V から取る。"""
import sys
sys.path.insert(0, "tools")
sys.stdout.reconfigure(encoding="utf-8")
from bb import *

RED   = (214, 60, 50)
ORANGE= (240, 150, 40)
BLACK = (40, 44, 48)

im, d = new("つまみ（可変抵抗）をつなぐ")
board(d)
mcu(d)

# つまみ：端子は h行の 21・22・23列
vr(d, 21, "h")
# 5V は i行、A2 は j行。逆にすると 5V の線が GND のジャンパーと
# 交差して、つながっているように見えてしまう
wire(d, (1, "i"), (21, "i"), RED)        # 5V     → つまみの左はし
wire(d, (6, "j"), (22, "j"), ORANGE)     # A2     → まん中（ワイパー）
wire(d, (23, "j"), (23, "-t"), BLACK)    # 右はし → 上の − レール
wire(d, (2, "j"), (2, "-t"), BLACK)      # GND    → 上の − レール

# LED（基本編①のまま）：5番ピン → LED → 1kΩ → 下の − レール
wire(d, (8, "a"), (16, "a"), RED)
led(d, 16, "c", note=False)
resistor_v(d, 17, "a", "-b")
wire(d, (3, "a"), (3, "-b"), BLACK)

save(im, "b4_analog", "bb_vr.png")
