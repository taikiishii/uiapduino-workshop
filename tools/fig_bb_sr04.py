# -*- coding: utf-8 -*-
"""e4 超音波センサー（HC-SR04）の配線図（上から見た図）。

**e3（人感センサー）の電源と GND の線をそのまま使う**（2026-10-24）。
センサーは丸い部分を**手前（ボードの下がわ）**に向けて、f行の 25〜28列にさす。
手前から見たピンの並びは、データシートの正面図と同じ **左から Vcc・Trig・Echo・Gnd**。
上から見ても同じ並びになる（丸い部分が図の下を向く）。

e3 からの変えかた：
  そのまま   … 28列の黒い線（Gnd → −レール）、ボードの GND の線、LED
  1列ずらす  … 5V の赤い線の右はし 26列 → 25列（Vcc）
  張りなおす … A2 のオレンジをはずし、Echo（27列）← 7番、Trig（26列）← 9番
プログラムは前と同じ（TRIG = 9・ECHO = 7）。

センサーの丸い部分は手前の e〜b 行あたりにかぶさるが、そこに線の端点はない
（線の途中がかくれるのはよい。端点はかくさない）。
以前は丸い部分を奥に向けて j行・18〜21列にさしていた（そのときの並びは Gnd・Echo・Trig・Vcc）。
"""
import sys
sys.path.insert(0, "tools")
sys.stdout.reconfigure(encoding="utf-8")
from bb import *

RED, ORANGE, YELLOW, BLACK = (214, 60, 50), (240, 150, 40), (232, 200, 70), (40, 44, 48)
PCBC, HEAD, DRUM = (26, 95, 126), (26, 26, 28), (217, 217, 217)
SC = 25                                        # Vcc の列（Trig 26・Echo 27・Gnd 28）

im, d = new("超音波センサー（HC-SR04）をつなぐ", extra_w=130)
board(d)
mcu(d)

# e3 から使いつづける線
wire(d, (2, "j"), (2, "-t"), BLACK)            # ボードの GND → −レール
wire(d, (SC + 3, "j"), (SC + 3, "-t"), BLACK)  # Gnd（28列）→ −レール
wire(d, (1, "i"), (SC, "i"), RED)              # 5V → Vcc（25列。e3 から1列ずらす）
# 張りなおす信号線
wire(d, (9, "j"), (SC + 2, "j"), YELLOW)       # 7番 → Echo（27列）
wire(d, (10, "h"), (SC + 1, "h"), ORANGE)      # 9番 → Trig（26列）

# LED（e3 と同じ 16・17列）
wire(d, (8, "a"), (16, "a"), RED)
led(d, 16, "c", note=False)
resistor_v(d, 17, "a", "-b")
wire(d, (3, "a"), (3, "-b"), BLACK)

# センサー本体（上から見た形。丸い部分が手前＝図の下を向く）
hw = 8.85 * PITCH                              # 45mm ＝ 約17.7列
mx = (cx(SC) + cx(SC + 3)) / 2
yf = cy("f")
ht, hb = yf - 6, yf + 40                       # ピンヘッダ（黒）
pt, pb = hb, hb + 40                           # 基板（横長の帯）
dt, db = pb, pb + 109                          # 丸いところ2つ
for k in (0.245, 0.755):                       # 送信と受信
    ccx = mx - hw + hw * 2 * k
    d.rectangle([ccx - 83, dt, ccx + 83, db], fill=DRUM, outline=(70, 74, 78), width=3)
d.rectangle([mx - hw, pt, mx + hw, pb], fill=PCBC, outline=(14, 56, 78), width=3)
center(d, mx, pt + 10, "HC-SR04", bold(16), WHITE)
d.rectangle([cx(SC) - 26, ht, cx(SC + 3) + 26, hb], fill=HEAD, outline=(10, 10, 12), width=2)
for i, lab in enumerate(("Vcc", "Trig", "Echo", "Gnd")):
    x = cx(SC + i)
    d.ellipse([x - 8, yf - 8, x + 8, yf + 8], fill=GOLD, outline=(180, 150, 40), width=2)
    center(d, x, yf + 14, lab, bold(12), WHITE)
save(im, "e4_distance", "bb_sr04.png")
