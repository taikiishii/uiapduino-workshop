# -*- coding: utf-8 -*-
"""p1「組み合わせて考える」の図。きっかけ（左）と 動くもの（右）を線でつなぐ。

もとの図（2026-09-20）は生成スクリプトが残っていなかったので、寸法を測って描きなおした。
2026-11-14 に「人感センサー」を足した（発展編 e3）。
"""
import sys
sys.path.insert(0, "tools")
sys.stdout.reconfigure(encoding="utf-8")
from fig import *

LINE      = (232, 238, 238)       # うすい線（つなぎ方はほかにもある）
PALEGOLD  = (255, 250, 235)
PALEORANGE = (255, 240, 232)
DARKGOLD  = (176, 130, 20)

# （名前, ひとこと, 黄色か）。黄色は「センサーではないきっかけ」
TRIG = [("スイッチ", "おす", 0), ("つまみ", "まわす", 0), ("光センサー", "明るさ", 0),
        ("温度センサー", "あつさ", 0), ("距離センサー", "近さ", 0),
        ("人感センサー", "人の気配", 0),
        ("タイマー", "時間・ランダム", 1), ("?", "ほかにもある", 1)]
ACT  = [("LED", "光る"), ("ブザー", "鳴る"), ("サーボ", "角度"), ("モーター", "回る")]
# 太線で例を示す（きっかけの番号, 動くものの番号）
PICK = [(2, 0), (3, 3), (4, 2), (5, 0), (6, 1)]

LX0, LX1 = 60, 410
RX0, RX1 = 790, 1140
H, STEP, TOP = 52, 64, 90
H_ALL = TOP + STEP * (len(TRIG) - 1) + H                     # 左の列の下はし
RTOP = TOP + STEP                                             # 右の列は左の2つめにそろえる（線がななめになって「つなぐ」感じが出る）

def ly(i): return TOP + STEP * i + H // 2
def ry(j): return RTOP + STEP * j + H // 2

im, d = canvas(1200, H_ALL + 22)
center(d, 600, 12, "「きっかけ」と「動くもの」を 線でつなぐ", bold(26))
center(d, 235, 52, "きっかけ", bold(22), DARKTEAL)
center(d, 965, 52, "動くもの", bold(22), DEEPORANGE)

for i in range(len(TRIG)):
    for j in range(len(ACT)):
        d.line([LX1 + 3, ly(i), RX0 - 3, ry(j)], fill=LINE, width=1)
for i, j in PICK:
    d.line([LX1, ly(i), RX0, ry(j)], fill=TEAL, width=3)

for i, (name, sub, gold) in enumerate(TRIG):
    y = TOP + STEP * i
    edge, fill, ink = (GOLD, PALEGOLD, DARKGOLD) if gold else (TEAL, SOFT, DARKTEAL)
    d.rounded_rectangle([LX0, y, LX1, y + H], radius=H // 2, fill=fill, outline=edge, width=3)
    middle(d, 172, y + H / 2, name, bold(20), ink)
    b = d.textbbox((0, 0), sub, font=reg(15))
    d.text((284, y + H / 2 - (b[3] - b[1]) / 2 - b[1]), sub, font=reg(15), fill=GRAY)

for j, (name, sub) in enumerate(ACT):
    y = RTOP + STEP * j
    d.rounded_rectangle([RX0, y, RX1, y + H], radius=H // 2, fill=PALEORANGE,
                        outline=DEEPORANGE, width=3)
    middle(d, 902, y + H / 2, name, bold(20), DEEPORANGE)
    b = d.textbbox((0, 0), sub, font=reg(15))
    d.text((1014, y + H / 2 - (b[3] - b[1]) / 2 - b[1]), sub, font=reg(15), fill=GRAY)

save(im, "p1_idea", "sense_act.png")
