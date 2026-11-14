# -*- coding: utf-8 -*-
"""PWM（明るさを変えられる）ピンの地図。b2・x2 で使う。

実機で analogWrite が効くのは **2・5・6・12 の4本だけ**。
公式図は 0 も PWM 可とするが、実機では 0 は無反応（1・10 と同じ。2026-09-23 実測）。
もとの図（2026-09-03 生成）は 0 もみどりにしていたので、描きなおした。
"""
import sys
sys.path.insert(0, "tools")
sys.stdout.reconfigure(encoding="utf-8")
from fig import *

PIN      = (255, 171, 25)     # ふつうのピン（オレンジ）
PIN_EDGE = (200, 160, 40)
BOARD    = (252, 252, 248)
EDGE     = (226, 232, 232)
USBGRAY  = (170, 176, 178)

# 上から順に（小さな注記, 番号）。左右それぞれ 12 本。
LEFT  = [("TX", "15/A5"), ("RX", "16/A6"), ("", "GND"), ("", "GND"),
         ("SDA", "3"), ("SCL", "4"), ("LED", "2"), ("", "5"),
         ("×", "11"), ("SCK", "7"), ("MOSI", "8"), ("MISO", "9")]
RIGHT = ["5V", "GND", "RESET", "3V3", "12/A3", "6/A2", "0/A1", "1/A0",
         "7", "9", "8", "10"]

def pinmap(title, on, note):
    """on … みどりにする番号（ラベルの文字列）の集合"""
    im, d = canvas(1000, 620)
    center(d, 500, 16, title, bold(26))
    d.rounded_rectangle([391, 67, 609, 546], radius=12, fill=BOARD, outline=EDGE, width=3)
    d.rounded_rectangle([456, 55, 544, 94], radius=6, fill=USBGRAY)
    center(d, 500, 102, "USB", reg(16), GRAY)
    for i in range(12):
        y = 129 + 36 * i
        for x, label, side in ((402, LEFT[i][1], "L"), (598, RIGHT[i], "R")):
            hit = label in on
            if hit:
                d.ellipse([x - 21, y - 21, x + 21, y + 21], outline=TEAL, width=2)
                d.ellipse([x - 12, y - 12, x + 12, y + 12], fill=TEAL, outline=DARKTEAL)
            else:
                d.ellipse([x - 12, y - 12, x + 12, y + 12], fill=PIN, outline=PIN_EDGE)
            f, c = (bold(20), DARKTEAL) if hit else (reg(20), INK)
            b = d.textbbox((0, 0), label, font=f)
            ty = y - (b[3] - b[1]) / 2 - b[1]
            if side == "L":
                d.text((372 - b[2], ty), label, font=f, fill=c)
            else:
                d.text((628 - b[0], ty), label, font=f, fill=c)
        small = LEFT[i][0]
        if small:
            b = d.textbbox((0, 0), small, font=reg(14))
            d.text((296 - b[2], y - (b[3] - b[1]) / 2 - b[1]), small, font=reg(14), fill=GRAY)
    center(d, 500, 572, note, reg(20), GRAY)
    return im

im = pinmap("PWM（明るさを変えられる）ピン", {"2", "5", "6/A2", "12/A3"},
            "この4本だけ。ほかのピンでは明るさが変わらない")
for ch in ("b2_pwm", "x2_trouble"):
    save(im, ch, "pins_pwm.png")
