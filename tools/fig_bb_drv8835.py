# -*- coding: utf-8 -*-
"""e6 モータードライバの配線図。2枚つくる。

  bb_drv8835.png … モーターA だけ（本編）
  bb_2motor.png  … モーターB を足す（ロボットカー）

**はじめから PHASE/ENABLE モード**（MODE を ＋レールへ）で組む。
  PHASE  … 向き（digitalWrite）
  ENABLE … 速さ（PWM）
実機で PWM が使えるのは 2・5・6・12 の4本だけ。IN/IN だと1モーターで
2本使うので、2つめを足すときに足りなくなる。PHASE/ENABLE なら
1モーター1本で済むので、**あとからBを足しても配線を組み直さなくてよい**。

ピン： AENBL=12番(A3) / APHASE=7番 / BENBL=6番(A2) / BPHASE=9番
ジャンパーは「どこからどこへ」が分かればよいので、経路は交差してよい。

⚠ ただし**どの行から出すか**は電気的に意味がある。ブレッドボードの列は
上半分（f〜j）と下半分（a〜e）で別のまとまりなので、**上の行のピンに
つなぐ線は、かならず f〜j 行から出す**こと。下半分から出すと、同じ列でも
下の行のピン（まったく別の番号）につながってしまう。
"""
import sys
sys.path.insert(0, "tools")
sys.stdout.reconfigure(encoding="utf-8")
from bb import *

RED, YELLOW, ORANGE, GREEN, BLUE, BLACK = ((214, 60, 50), (232, 200, 70),
    (240, 150, 40), (90, 180, 110), (90, 150, 220), (40, 44, 48))
DC = 20                                    # DIP の左はし（VCC / VM の列）

def draw(two):
    title = "ロボットカー（モーターを2つに）" if two else "モータードライバ（DRV8835）をつなぐ"
    im, d = new(title, extra_w=420)
    board(d)
    mcu(d)
    dip(d, DC, ["VCC", "MODE", "APHASE", "AENBL", "BPHASE", "BENBL"],
                ["VM", "AOUT1", "AOUT2", "BOUT1", "BOUT2", "GND"],
                title_dx=52)     # VCC・MODE の赤線の端点をよける

    # 電源と MODE
    wire(d, (1, "j"), (1, "+t"), RED)            # ボードの5V → 上の ＋レール
    wire(d, (2, "i"), (2, "-t"), BLACK)          # ボードのGND → 上の −レール
    # 端点は g 行まで下ろす。h・i 行は合図の線が通っていて、そこで
    # 止めると赤い丸（端点）が線の下に隠れてしまう。
    wire(d, (DC, "g"), (DC, "+t"), RED)          # VCC  → ＋レール
    wire(d, (DC + 1, "g"), (DC + 1, "+t"), RED)  # MODE → ＋レール（HIGHに固定）

    # モーターA の合図
    wire(d, (9, "h"), (DC + 2, "h"), YELLOW)     # 7番  → APHASE（向き）
    wire(d, (5, "i"), (DC + 3, "i"), ORANGE)     # 12番 → AENBL （速さ・PWM）
    if two:                                      # モーターB の合図
        wire(d, (10, "j"), (DC + 4, "j"), GREEN) # 9番  → BPHASE（向き）
        # 上に使える行（h・i・j）は3本ぶんしかないので、青だけ h 行を黄と
        # 共有する。重ならないよう、上へふくらませてよける。黄を h にするのは
        # 緑（24列まで）より短く（22列まで）、よける区間が短くてすむから。
        wire(d, (6, "h"), (DC + 5, "h"), BLUE, arc=-46)  # 6番 → BENBL（速さ・PWM）

    # 力の線
    wire(d, (DC, "a"), (DC, "+b"), RED)                # VM  → 下の ＋レール
    wire(d, (DC + 5, "a"), (DC + 5, "-b"), BLACK)      # GND → 下の −レール
    wire(d, (3, "a"), (3, "-b"), BLACK)                # ボードのGND

    if two:
        motor(d, 1420, 190, r=56); center(d, 1560, 190, "A（左）", bold(20), INK)
        motor(d, 1420, 400, r=56); center(d, 1560, 400, "B（右）", bold(20), INK)
        lead(d, 1372, 220, (DC + 1, "a"), (176, 180, 182))
        lead(d, 1384, 234, (DC + 2, "a"), (176, 180, 182))
        lead(d, 1372, 430, (DC + 3, "a"), (150, 154, 158))
        lead(d, 1384, 444, (DC + 4, "a"), (150, 154, 158))
    else:
        motor(d, 1420, 270); center(d, 1560, 270, "A", bold(22), INK)
        lead(d, 1354, 310, (DC + 1, "a"), (176, 180, 182))
        lead(d, 1380, 336, (DC + 2, "a"), (176, 180, 182))
    bp, bm = battery(d, 1310, 540)
    lead(d, bp[0], bp[1], (27, "+b"), RED)
    lead(d, bm[0], bm[1], (30, "-b"), BLACK)
    save(im, "e6_motor", "bb_2motor.png" if two else "bb_drv8835.png")

draw(False)
draw(True)
