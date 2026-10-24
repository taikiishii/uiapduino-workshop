# -*- coding: utf-8 -*-
"""e5 「⚠ 電気をたくさん使う」の図：サーボに別の電源をつなぐときの考え方。

力の電気は **ボードとは別の電源**（USB 電源でも電池でもよい）から、
合図（信号）だけボードから。GND どうしはつなぐ。
（前は「電池」だけで描いていた。2026-10-24 に「USB 電源 または 電池」にした）
"""
import sys
sys.path.insert(0, "tools")
sys.stdout.reconfigure(encoding="utf-8")
from fig import *

RED, BLACK, YEL = (214, 60, 50), (60, 66, 70), (235, 190, 60)

im, d = canvas(1040, 440)
center(d, 520, 16, "サーボに別の電源をつなぐとき", bold(26), INK)

# 電源（上）・ボード（左）・サーボ（右）
box(d, 380, 60, 300, 86, fill=(255, 240, 228), outline=(232, 120, 40))
middle(d, 530, 92, "別の電源", bold(24), (214, 100, 30))
middle(d, 530, 126, "USB 電源 または 電池", reg(16), (150, 100, 60))
box(d, 60, 198, 240, 156, fill=(228, 246, 244), outline=(0, 160, 150))
middle(d, 180, 276, "UIAPduino", bold(24), (0, 110, 104))
box(d, 740, 198, 240, 156, fill=(240, 242, 242), outline=(136, 144, 148))
middle(d, 860, 276, "サーボ", bold(26), INK)

# ＋（赤）：電源 → サーボ
d.line([680, 84, 860, 84, 860, 198], fill=RED, width=7)
d.text((690, 52), "＋（赤）", font=bold(18), fill=RED)
# −（黒）：電源 → GND の線
d.line([680, 122, 700, 122, 700, 240], fill=BLACK, width=7)
d.text((712, 154), "−（黒）", font=bold(18), fill=BLACK)
# GND どうし
d.line([300, 240, 740, 240], fill=BLACK, width=7)
d.ellipse([692, 232, 708, 248], fill=BLACK)
center(d, 520, 254, "GND どうしを つなぐ（忘れると動かない）", bold(18), INK)
# 信号
d.line([300, 306, 740, 306], fill=YEL, width=7)
center(d, 520, 318, "信号（5番ピン）", bold(18), (190, 140, 20))

center(d, 520, 396, "力の電気は別の電源から、合図だけボードから", reg(20), GRAY)
save(im, "e5_servo", "servo_power.png")
