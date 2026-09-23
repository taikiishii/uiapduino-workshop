# -*- coding: utf-8 -*-
"""e5 モータードライバの配線図。

端子の並びは資料 AE-DRV8835-S のピン番号から起こしてある。
  1 VM   2 AOUT1  3 AOUT2  4 BOUT1  5 BOUT2  6 GND
  7 BIN2 8 BIN1   9 AIN2  10 AIN1  11 MODE  12 VCC
12ピンDIP なので 1番の向かいが 12番。これを横向きにさすと
  上の行  VCC MODE AIN1 AIN2 BIN1 BIN2
  下の行  VM  AOUT1 AOUT2 BOUT1 BOUT2 GND
となり、合図が上・力が下にそろう。

VCC はロジックの基準電圧。資料に「入力は 0〜Vcc」とあるので、
5V で動くこのボードにつなぐなら VCC も 5V。
"""
import sys
sys.path.insert(0, "tools")
sys.stdout.reconfigure(encoding="utf-8")
from bb import *

RED, ORANGE, YELLOW, BLACK = (214, 60, 50), (240, 150, 40), (232, 200, 70), (40, 44, 48)
DC = 20                                    # DIP の左はし（VCC / VM の列）

im, d = new("モータードライバ（DRV8835）をつなぐ", extra_w=420)
board(d)
mcu(d)
dip(d, DC, ["VCC", "MODE", "AIN1", "AIN2", "BIN1", "BIN2"],
            ["VM", "AOUT1", "AOUT2", "BOUT1", "BOUT2", "GND"])

# ── 合図と電源（上がわ）。行を分けて平行に走らせる ──────────
wire(d, (1, "h"), (DC,     "h"), RED)      # 5V      → VCC
wire(d, (5, "i"), (DC + 2, "i"), ORANGE)   # 12番(A3) → AIN1
wire(d, (7, "j"), (DC + 3, "j"), YELLOW)   # 0番(A1)  → AIN2
wire(d, (DC + 1, "g"), (28, "g"), BLACK)   # MODE → GND（IN/INモード）
wire(d, (28, "g"), (28, "-t"), BLACK)
wire(d, (2, "j"), (2, "-t"), BLACK)        # ボードのGND → 上の −レール

# ── 力の線（下がわ）────────────────────────────────
wire(d, (DC,     "a"), (DC,     "+b"), RED)     # VM  → 下の ＋レール
wire(d, (DC + 5, "a"), (DC + 5, "-b"), BLACK)   # GND → 下の −レール
wire(d, (3, "a"), (3, "-b"), BLACK)             # ボードのGND → 同じレール

MX, MY = 1450, 300
motor(d, MX, MY)
lead(d, MX - 66, MY + 40, (DC + 1, "b"), (176, 180, 182))   # AOUT1
lead(d, MX - 40, MY + 66, (DC + 2, "b"), (176, 180, 182))   # AOUT2
bp, bm = battery(d, 1330, 500)
lead(d, bp[0], bp[1], (29, "+b"), RED)
lead(d, bm[0], bm[1], (30, "-b"), BLACK)
save(im, "e5_motor", "bb_drv8835.png")
