# -*- coding: utf-8 -*-
"""b2 タクトスイッチの中のつながり。
   ここはまだ一般論の説明なので、ブレッドボードの行の名前は出さない。"""
import sys
sys.path.insert(0, "tools")
sys.stdout.reconfigure(encoding="utf-8")
from fig import *

BODY   = (46, 50, 54)
BAR    = (232, 244, 242)
BTN_OFF, BTN_ON = (128, 132, 136), (205, 50, 44)

def switch(d, cx, top, pressed):
    for y in (top, top + 166):                       # 上と下の わたり
        d.rounded_rectangle([cx - 152, y, cx + 152, y + 30], radius=8,
                            fill=BAR, outline=TEAL, width=3)
    if pressed:                                      # 押すと 上と下がつながる
        for sx in (cx - 120, cx + 120):
            d.rounded_rectangle([sx - 9, top + 15, sx + 9, top + 181], radius=5,
                                fill=DEEPORANGE)
    d.rounded_rectangle([cx - 122, top + 36, cx + 122, top + 160], radius=8,
                        fill=BODY, outline=(28, 32, 36), width=2)
    r = 36
    d.ellipse([cx - r, top + 98 - r, cx + r, top + 98 + r],
              fill=BTN_ON if pressed else BTN_OFF,
              outline=(160, 40, 34) if pressed else (96, 100, 104), width=3)
    for y in (top + 15, top + 181):                  # 4本の足
        for sx in (cx - 120, cx + 120):
            d.ellipse([sx - 11, y - 11, sx + 11, y + 11], fill=GOLD,
                      outline=(180, 150, 40), width=2)

im, d = canvas(1240, 520)
center(d, 620, 10, "タクトスイッチの中のつながり", bold(28), INK)
for x0, head, col, pressed, note in (
        (40, "押していないとき", DARKTEAL, False, "上と下は はなれている"),
        (660, "押したとき", DEEPORANGE, True, "押すと 上と下が つながる")):
    cx = x0 + 270
    center(d, cx, 54, head, bold(24), col)
    center(d, cx, 92, "上の2本は いつもつながっている", reg(17), GRAY)
    switch(d, cx, 120, pressed)
    center(d, cx, 322, "下の2本も いつもつながっている", reg(17), GRAY)
    center(d, cx, 356, note, bold(22), col)

d.rounded_rectangle([160, 412, 1080, 470], radius=12, fill=SOFT, outline=TEAL, width=3)
middle(d, 620, 441, "つながっているのは 上の2本どうし・下の2本どうし", bold(25), DARKTEAL)
save(im, "b2_switch", "switch_legs.png")
