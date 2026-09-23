# -*- coding: utf-8 -*-
"""e3 超音波センサーの配線図（上から見た図）。

HC-SR04 は 45×20mm。20mm ＝ 約8行ぶんあるので、どこにさしても本体が
まわりの行をおおってしまう。そこで **いちばん外の j行** にさす。
こうすると本体は盤面の外（上）へはみ出し、**f〜i行があいたまま**になるので、
4本の線がどの穴に入るかがそのまま見える。
LED は基本編と同じ 16・17列の外づけ。下半分はぜんぶあいている。
"""
import sys
sys.path.insert(0, "tools")
sys.stdout.reconfigure(encoding="utf-8")
from bb import *

RED, ORANGE, YELLOW, BLACK = (214, 60, 50), (240, 150, 40), (232, 200, 70), (40, 44, 48)
SCOL, OFF_Y, EXTRA_W = 21, 165, 390        # センサーの左はしの列 ／ 上のすきま ／ 右の余白

im0, d0 = canvas(W, H)
board(d0)
mcu(d0)

# ── 合図と電源。上下の順（5V → Trig → Echo）をそろえて交差させない ──
wire(d0, (1, "j"),  (SCOL,     "i"), RED)      # 5V    → Vcc
wire(d0, (10, "i"), (SCOL + 1, "h"), ORANGE)   # 9番   → Trig
wire(d0, (9, "h"),  (SCOL + 2, "g"), YELLOW)   # 7番   → Echo
wire(d0, (SCOL + 3, "f"), (SCOL + 3, "-b"), BLACK)   # Gnd → 下の −レール
wire(d0, (3, "a"),  (3, "-b"), BLACK)          # ボードのGND → 同じレール

# ── LED（基本編と同じ 16・17列）。下半分はあいている ──────────
wire(d0, (11, "b"), (16, "b"), RED)            # 8番 → LED
led(d0, 16, "b", note=False)
resistor_v(d0, 17, "a", "-b")

# ── 上にすきまを作って貼り、本体を j行から外へはみ出させて描く ──
im, d = canvas(W + EXTRA_W, H + OFF_Y)
im.paste(im0, (0, OFF_Y))
center(d, (W + EXTRA_W) / 2, 16, "超音波センサー（HC-SR04）をつなぐ", bold(30), INK)

hw = 8.85 * PITCH                              # 45mm ＝ 約17.7列
h  = hw * 2 / 2.25                             # 20mm ＝ 約8行
mx = (cx(SCOL) + cx(SCOL + 3)) / 2
yj = cy("j") + OFF_Y                           # ずらしたあとの j行
bot, top = yj - 26, yj - 26 - h                # 端子の すぐ上までにして、盤面を隠さない
d.rounded_rectangle([mx - hw, top, mx + hw, bot], radius=10,
                    fill=(38, 64, 140), outline=(24, 44, 100), width=3)
for k in (0.245, 0.755):                       # 送信と受信の丸いところ
    ccx, ccy, r = mx - hw + hw * 2 * k, top + h * 0.45, h * 0.30
    d.ellipse([ccx - r, ccy - r, ccx + r, ccy + r], fill=(178, 182, 186),
              outline=(118, 122, 126), width=3)
    for q in (0.72, 0.5, 0.28):
        d.ellipse([ccx - r * q, ccy - r * q, ccx + r * q, ccy + r * q],
                  outline=(128, 132, 136), width=2)
center(d, mx, top + 10, "HC-SR04", bold(21), WHITE)
for i, lab in enumerate(("Vcc", "Trig", "Echo", "Gnd")):
    x = cx(SCOL + i)
    d.ellipse([x - 11, yj - 11, x + 11, yj + 11], fill=GOLD,
              outline=(180, 150, 40), width=3)
    center(d, x, yj + 16, lab, bold(14), INK)

box(d, W + 20, 300, EXTRA_W - 46, 230, fill=SOFT, outline=TEAL)
for i, ln in enumerate(["本体は 45×20mm。", "いちばん外の j行 にさして、",
                        "盤面の外へ はみ出させる。", "こうすると f〜i行が",
                        "あいたままになる。"]):
    center(d, W + 20 + (EXTRA_W - 46) / 2, 326 + i * 40, ln, reg(19), INK)
save(im, "e3_distance", "bb_sr04.png")
print("  比 %.2f" % ((W + EXTRA_W) / (H + OFF_Y)))
