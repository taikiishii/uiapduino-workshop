# -*- coding: utf-8 -*-
"""b5 ブザーの配線図。

ブザーのリード線を、**マイコンのピンと同じ列に直接**さす（ジャンパーワイヤーなし）。
  赤 → 7番の列（9列目）／ 黒 → GND の列（2列目）
ボードのピンは c〜g 行をふさいでいるが、同じ列の h〜j 行はそのピンにつながっている。

⚠ 上の行のピンは 9列目が「7」、10列目が「9」。まちがえやすいので注意
  （前の図はオレンジの線が 10列目＝9番から出ていた。2026-10-24 に描き直し）。
"""
import sys
sys.path.insert(0, "tools")
sys.stdout.reconfigure(encoding="utf-8")
from bb import *

RED, BLACK = (214, 60, 50), (60, 64, 68)

im, d = new("ブザーをつなぐ", extra_w=380)
board(d)
mcu(d)

bx, by = W + 190, 330
buzzer(d, bx, by)
# 上から出す赤は上の j 行へ、下から出す黒は h 行へ。こうすると2本が交差しない
lead(d, bx - 88, by - 40, (9, "j"), RED)     # 赤 → 7番の列
lead(d, bx - 92, by + 0, (2, "h"), BLACK)    # 黒 → GND の列

center(d, bx, by + 140, "線は ピンと同じ列に", reg(17), GRAY)
center(d, bx, by + 164, "直接さすだけ", reg(17), GRAY)
save(im, "b5_buzzer", "bb_buzzer.png")
