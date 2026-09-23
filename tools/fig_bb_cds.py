# -*- coding: utf-8 -*-
"""e1 光センサーの配線図。

10kΩ の固定抵抗のかわりに **10kΩ の可変抵抗**を入れてある。
こうすると、章の途中で配線を組みかえずに

  ・いっぱいに回す（＝10kΩ）… 明るさで点滅の速さが変わるのを見る
  ・少し もどす            … 室内でぎりぎり消えるさかいめを作る

の両方ができる。つまみは **まん中の足と、片はしの足**の2本だけ使う。
長い線は i行、レールへの立ち上げは j行から。こうすると交差しない。
"""
import sys
sys.path.insert(0, "tools")
sys.stdout.reconfigure(encoding="utf-8")
from bb import *

RED, ORANGE, BLACK = (214, 60, 50), (240, 150, 40), (40, 44, 48)

im, d = new("光センサー（CdS）と つまみ をつなぐ")
board(d)
mcu(d)

wire(d, (1, "j"), (1, "+t"), RED)          # ボードの5V  → ＋レール
wire(d, (2, "j"), (2, "-t"), BLACK)        # ボードのGND → −レール

# ＋レール ── CdS ── A2 ── つまみ ── −レール
cds(d, 20, "f")                            # 足は 20列・21列
wire(d, (20, "j"), (20, "+t"), RED)        # CdS の上がわ → ＋レール
wire(d, (6, "i"), (21, "i"), ORANGE)       # まん中（21列）→ A2
wire(d, (21, "g"), (24, "g"), ORANGE)      # まん中 → つまみの片はし
vr(d, 24, "f")                             # 足は 24・25・26列（まん中が25）
wire(d, (25, "j"), (25, "-t"), BLACK)      # まん中の足 → −レール

# LED（基本編と同じ 16・17列）
wire(d, (8, "a"), (16, "a"), RED)          # 5番 → LED
led(d, 16, "c", note=False)
resistor_v(d, 17, "a", "-b")
wire(d, (3, "a"), (3, "-b"), BLACK)        # ボードのGND → 下の −レール
save(im, "e1_light", "bb_cds.png")
