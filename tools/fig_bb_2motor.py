# -*- coding: utf-8 -*-
"""e5 ロボットカー（モーター2つ）の配線図。

**PHASE/ENABLE モード**（MODE を HIGH）にすると、1モーターあたり
  PHASE … 向き（digitalWrite でよい）
  ENABLE … 速さ（PWM）
に分かれるので、**PWM は1モーターに1本**で済む。
実機で PWM が使えるのは 2・5・6・12 の4本しかないため、
IN/IN のまま2モーターにすると4本ぜんぶ取られてしまう。

ボード側（5・6・9・10列）とドライバ側（DC+2〜DC+5）の並び順が
そろっているので、4本を平行に引ける。
"""
import sys
sys.path.insert(0, "tools")
sys.stdout.reconfigure(encoding="utf-8")
from bb import *

RED, ORANGE, YELLOW, GREEN, BLUE, BLACK = ((214, 60, 50), (240, 150, 40),
    (232, 200, 70), (90, 180, 110), (90, 150, 220), (40, 44, 48))
DC = 20

im, d = new("ロボットカー（モーター2つ）", extra_w=420)
board(d)
mcu(d)
dip(d, DC, ["VCC", "MODE", "APHASE", "AENBL", "BPHASE", "BENBL"],
            ["VM", "AOUT1", "AOUT2", "BOUT1", "BOUT2", "GND"])

# ── 電源と MODE（上がわ）────────────────────────────
wire(d, (1, "h"), (DC, "f"), RED)             # 5V → VCC
wire(d, (DC + 1, "g"), (28, "g"), RED)        # MODE → ＋（HIGH に固定）
wire(d, (28, "g"), (28, "+t"), RED)
wire(d, (1, "j"), (1, "+t"), RED)             # ボードの5V → 上の ＋レール
wire(d, (2, "i"), (2, "-t"), BLACK)           # ボードのGND → 上の −レール

# ── 合図4本。**行を分けて平行に**引く ──────────────────
# ドライバの並びは APHASE→AENBL→BPHASE→BENBL の順だが、ボード側は
# PWM が使える 12番(5列)・6番(6列) が左に固まっている。そのため
# どう置いても「列の順」はそろわない（総当たりで確認）。
# ブレッドボードの行は独立しているので、**1本ずつ別の行**に置けば
# 線どうしは重ならず、たどるのもやさしい。
# ボード側で使えるのは h・i・j の3行だけ（MCU が c〜g を占有）。
# 4本目だけ、ドライバがわの到達行を g に下ろして重なりを避ける。
wire(d, (9,  "j"), (DC + 2, "j"), YELLOW)     # 7番  → APHASE（左の向き）
wire(d, (5,  "i"), (DC + 3, "i"), ORANGE)     # 12番 → AENBL （左の速さ・PWM）
wire(d, (10, "h"), (DC + 4, "h"), GREEN)      # 9番  → BPHASE（右の向き）
wire(d, (6,  "j"), (DC + 5, "g"), BLUE)       # 6番  → BENBL （右の速さ・PWM）

# ── 力の線（下がわ）────────────────────────────────
wire(d, (DC, "a"), (DC, "+b"), RED)                 # VM  → 下の ＋レール
wire(d, (DC + 5, "a"), (DC + 5, "-b"), BLACK)       # GND → 下の −レール
wire(d, (3, "a"), (3, "-b"), BLACK)                 # ボードのGND

MX, MY = 1430, 190
motor(d, MX, MY, r=60)
motor(d, MX, MY + 250, r=60)
center(d, MX, MY + 76, "左", bold(20), INK)
center(d, MX, MY + 326, "右", bold(20), INK)
lead(d, MX - 52, MY + 32, (DC + 1, "b"), (176, 180, 182))
lead(d, MX - 30, MY + 52, (DC + 2, "b"), (176, 180, 182))
lead(d, MX - 52, MY + 282, (DC + 3, "b"), (150, 154, 158))
lead(d, MX - 30, MY + 302, (DC + 4, "b"), (150, 154, 158))
bp, bm = battery(d, 1300, 540)
lead(d, bp[0], bp[1], (27, "+b"), RED)
lead(d, bm[0], bm[1], (30, "-b"), BLACK)
save(im, "e5_motor", "bb_2motor.png")
