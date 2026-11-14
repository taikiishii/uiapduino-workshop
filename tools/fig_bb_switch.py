# -*- coding: utf-8 -*-
"""b3 スイッチと LED の配線図。

タクトスイッチは 25〜28列・d行と g行（溝をまたぐ）。
**25列の上下・28列の上下は いつもつながっていて、押すと 25列と 28列がつながる**
（2026-11-14 オーナーが実物で確認。前は「行どうしがつながる」と書いていた）。

3番は 25列の下（b行）、GND は 28列の上（j行 → −レール）と、ななめの足につないでいる。
押すと 25列と 28列がつながり、3番が GND につながって 0 になる。
LED（5番・16〜17列）は b1・b2 と同じ。図のスクリプトが残っていなかったので作りなおした。
"""
import sys
sys.path.insert(0, "tools")
sys.stdout.reconfigure(encoding="utf-8")
from bb import *

RED, BLUE, BLACK = (214, 60, 50), (60, 110, 210), (40, 44, 48)

im, d = new("スイッチと LED をつなぐ")
board(d)
mcu(d)

wire(d, (2, "j"), (2, "-t"), BLACK)       # ボードの GND → 上の −レール
wire(d, (28, "j"), (28, "-t"), BLACK)     # スイッチの 28列 → 上の −レール（GND）
wire(d, (5, "b"), (25, "b"), BLUE)        # 3番 → スイッチの 25列
tact(d, 25, 28)

# LED（b1・b2 と同じ）：5番 → LED → 1kΩ → 下の −レール
wire(d, (8, "a"), (16, "a"), RED)
led(d, 16, "c", note=False)
resistor_v(d, 17, "a", "-b")
wire(d, (3, "a"), (3, "-b"), BLACK)
save(im, "b3_switch", "bb_switch.png")
