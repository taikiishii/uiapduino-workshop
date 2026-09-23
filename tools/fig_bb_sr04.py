# -*- coding: utf-8 -*-
"""e3 超音波センサーの配線図（上から見た図）。

HC-SR04 は 45×20mm。**いちばん外の j行**にさすと、本体が盤面の外へ
はみ出すので、f〜i行があいたまま残る。

上から見ると、横長の基板の上に丸いところが2つ、手前にピンヘッダが
出ている形。**ピンは左から Gnd・Echo・Trig・Vcc**（部品の裏に書いて
ある並びを、上から見ると逆に見える）。

センサーの電源は上のレールが本体にかくれるので、**下のレール**へ回す。
"""
import sys
sys.path.insert(0, "tools")
sys.stdout.reconfigure(encoding="utf-8")
from bb import *

RED, ORANGE, YELLOW, BLACK = (214, 60, 50), (240, 150, 40), (232, 200, 70), (40, 44, 48)
PCB, HEAD, DRUM = (26, 95, 126), (26, 26, 28), (217, 217, 217)
SCOL, OFF_Y, EXTRA_W = 18, 150, 420         # Gnd の列 ／ 上のすきま ／ 右の余白

im0, d0 = canvas(W, H)
board(d0)
mcu(d0)

# ── 合図。行を分けて平行に走らせる ───────────────────
wire(d0, (9, "i"),  (SCOL + 1, "i"), YELLOW)   # 7番 → Echo
wire(d0, (10, "h"), (SCOL + 2, "h"), ORANGE)   # 9番 → Trig

# ── 電源。センサーがわは 下のレールへ下ろす ─────────────
wire(d0, (1, "j"), (1, "+t"), RED)             # ボードの5V → 上の ＋レール
wire(d0, (30, "+t"), (30, "+b"), RED)          # 上下の ＋レールをつなぐ
wire(d0, (SCOL + 3, "f"), (SCOL + 3, "+b"), RED)    # Vcc → 下の ＋レール
wire(d0, (SCOL, "f"), (SCOL, "-b"), BLACK)          # Gnd → 下の −レール
wire(d0, (3, "a"), (3, "-b"), BLACK)           # ボードのGND → 同じレール

# ── LED（基本編と同じ 16・17列）──────────────────────
wire(d0, (8, "b"), (16, "b"), RED)             # 5番 → LED
led(d0, 16, "b", note=False)
resistor_v(d0, 17, "a", "-b")

# ── 上にすきまを作って貼り、本体を j行から外へはみ出させる ──
im, d = canvas(W + EXTRA_W, H + OFF_Y)
im.paste(im0, (0, OFF_Y))
center(d, (W + EXTRA_W) / 2, 16, "超音波センサー（HC-SR04）をつなぐ", bold(30), INK)

hw = 8.85 * PITCH                              # 45mm ＝ 約17.7列
mx = (cx(SCOL) + cx(SCOL + 3)) / 2
yj = cy("j") + OFF_Y
hb, ht = yj + 6, yj - 40                       # ピンヘッダ（黒）
pb, pt = ht, ht - 40                           # 基板（横長の帯）
db, dt = pt, pt - 109                          # 丸いところ2つ

for k in (0.245, 0.755):                       # 送信と受信
    ccx = mx - hw + hw * 2 * k
    d.rectangle([ccx - 83, dt, ccx + 83, db], fill=DRUM, outline=(70, 74, 78), width=3)
d.rectangle([mx - hw, pt, mx + hw, pb], fill=PCB, outline=(14, 56, 78), width=3)
d.rectangle([cx(SCOL) - 26, ht, cx(SCOL + 3) + 26, hb], fill=HEAD,
            outline=(10, 10, 12), width=2)
for i, lab in enumerate(("Gnd", "Echo", "Trig", "Vcc")):
    x = cx(SCOL + i)
    d.ellipse([x - 11, yj - 11, x + 11, yj + 11], fill=GOLD,
              outline=(180, 150, 40), width=3)
    center(d, x, yj + 16, lab, bold(14), INK)

box(d, W + 20, 330, EXTRA_W - 46, 190, fill=SOFT, outline=TEAL)
for i, ln in enumerate(["本体は 45×20mm。", "いちばん外の j行 にさして、",
                        "盤面の外へ はみ出させる。", "ピンは左から",
                        "Gnd・Echo・Trig・Vcc。"]):
    center(d, W + 20 + (EXTRA_W - 46) / 2, 352 + i * 34, ln, reg(18), INK)
save(im, "e3_distance", "bb_sr04.png")
print("  比 %.2f" % ((W + EXTRA_W) / (H + OFF_Y)))
