#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""ブレッドボードの配線図をかくための部品。

実物の配置にそろえてある：
  30列／上下に電源レール／UIAPduino は列1〜12・行 c〜g を占有／USB は左に出る
ピンと同じ列の あいている穴（上は h i j、下は a b）から線を引き出す、という
実際の使い方がそのまま図になる。
"""
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from fig import *

COLS  = 30
PITCH = 36
BB    = (243, 243, 240)
HOLE  = (252, 252, 252)
HRIM  = (198, 202, 202)
MINUS = (52, 110, 200)
PLUS  = (222, 62, 56)
PCB   = (250, 250, 248)
SILK  = (56, 62, 66)

X0 = 96
Y0 = 116

ROWS_TOP = ["j", "i", "h", "g", "f"]
ROWS_BOT = ["e", "d", "c", "b", "a"]

_Y = {}
_y = Y0
for _k in ("-t", "+t"):
    _Y[_k] = _y; _y += PITCH
_y += 26
for _r in ROWS_TOP:
    _Y[_r] = _y; _y += PITCH
_y += 22
GUTTER_Y = _y - 36
for _r in ROWS_BOT:
    _Y[_r] = _y; _y += PITCH
_y += 26
for _k in ("-b", "+b"):
    _Y[_k] = _y; _y += PITCH
BOARD_H = _y - Y0
W = X0 + COLS * PITCH + 90
H = _y + 12

def cx(col):  return X0 + (col - 1) * PITCH
def cy(row):  return _Y[row]

def hole(d, x, y, rim=HRIM):
    d.rounded_rectangle([x - 8, y - 8, x + 8, y + 8], radius=3,
                        fill=HOLE, outline=rim, width=2)

def board(d):
    """ブレッドボード本体"""
    d.rounded_rectangle([40, Y0 - 34, W - 40, Y0 + BOARD_H - 2], radius=12,
                        fill=BB, outline=(206, 210, 210), width=3)
    for key, col, sign in (("-t", MINUS, "−"), ("+t", PLUS, "＋"),
                           ("-b", MINUS, "−"), ("+b", PLUS, "＋")):
        y = cy(key)
        d.line([X0 - 24, y, cx(COLS) + 24, y], fill=col, width=3)
        for c in range(1, COLS + 1):
            if c % 5 == 0:
                continue
            hole(d, cx(c), y, col)
        d.text((52, y - 15), sign, font=bold(24), fill=col)
        d.text((W - 78, y - 15), sign, font=bold(24), fill=col)
    for r in ROWS_TOP + ROWS_BOT:
        y = cy(r)
        for c in range(1, COLS + 1):
            hole(d, cx(c), y)
        d.text((60, y - 13), r, font=reg(19), fill=GRAY)
        d.text((W - 76, y - 13), r, font=reg(19), fill=GRAY)
    d.line([56, GUTTER_Y, W - 56, GUTTER_Y], fill=(214, 218, 218), width=4)
    for c in list(range(1, COLS + 1, 5)) + [COLS]:
        center(d, cx(c), cy("j") - 32, str(c), reg(17), GRAY)
        center(d, cx(c), cy("a") + 20, str(c), reg(17), GRAY)

# ピンは基板のシルク印刷にそろえ、ちがう名前で呼ぶものだけ番号を小さく添える
PINS_TOP = [("5V", ""), ("GND", ""), ("RST", ""), ("3V3", ""),
            ("A3", "12"), ("A2", "6"), ("A1", "0"), ("A0", "1"),
            ("7", ""), ("9", ""), ("8", ""), ("10", "")]
PINS_BOT = [("TX", "15"), ("RX", "16"), ("GND", ""), ("GND", ""),
            ("3", ""), ("4", ""), ("2", ""), ("5", ""),
            ("11", ""), ("7", ""), ("8", ""), ("9", "")]
MCU_COLS = 12
PIN_T, PIN_B = "g", "c"          # 実物は c〜g の5行ぶんを占有する

def mcu(d):
    x1, x2 = cx(1) - 20, cx(MCU_COLS) + 20
    y1, y2 = cy(PIN_T) - 24, cy(PIN_B) + 24
    ym = (y1 + y2) / 2
    d.rounded_rectangle([x1 - 44, ym - 26, x1 + 6, ym + 26], radius=6,
                        fill=(172, 176, 178))
    center(d, x1 - 19, ym - 10, "USB", reg(15), WHITE)
    d.rounded_rectangle([x1, y1, x2, y2], radius=8, fill=PCB,
                        outline=(120, 128, 132), width=3)
    center(d, (x1 + x2) / 2, ym - 14, "UIAPduino", bold(22), SILK)
    for c, (lab, num) in enumerate(PINS_TOP, 1):
        d.ellipse([cx(c) - 9, cy(PIN_T) - 9, cx(c) + 9, cy(PIN_T) + 9], fill=GOLD)
        center(d, cx(c), cy(PIN_T) + 14, lab, bold(14), SILK)
        if num:
            center(d, cx(c), cy(PIN_T) + 32, num, reg(12), GRAY)
    for c, (lab, num) in enumerate(PINS_BOT, 1):
        d.ellipse([cx(c) - 9, cy(PIN_B) - 9, cx(c) + 9, cy(PIN_B) + 9], fill=GOLD)
        center(d, cx(c), cy(PIN_B) - 28, lab, bold(14), SILK)
        if num:
            center(d, cx(c), cy(PIN_B) - 46, num, reg(12), GRAY)

def wire(d, a, b, col=(70, 150, 90), w=9, arc=0):
    """ジャンパー線。a,b は (列, 行)。arc>0 で下にふくらませ、他の線をまたぐ"""
    (c1, r1), (c2, r2) = a, b
    x1, y1, x2, y2 = cx(c1), cy(r1), cx(c2), cy(r2)
    if arc:
        pts = []
        for i in range(31):
            s = i / 30
            pts.append((x1 + (x2 - x1) * s,
                        y1 + (y2 - y1) * s + arc * (4 * s * (1 - s))))
        d.line(pts, fill=WHITE, width=w + 8, joint="curve")   # 白いふち＝またいでいる
        d.line(pts, fill=col, width=w, joint="curve")
    else:
        d.line([x1, y1, x2, y2], fill=WHITE, width=w + 8)
        d.line([x1, y1, x2, y2], fill=col, width=w)
    for x, y in ((x1, y1), (x2, y2)):
        d.ellipse([x - 7, y - 7, x + 7, y + 7], fill=col)

def led(d, col, row, color=(228, 60, 48), name="赤", note=True):
    """LED。となりあう2列にさす。col＝長い足（＋）、col+1＝短い足（−）"""
    x1, x2, y = cx(col), cx(col + 1), cy(row)
    mx, my = (x1 + x2) / 2, y - 30
    for x in (x1, x2):
        d.line([x, y, mx, my], fill=(176, 180, 182), width=5)
    d.ellipse([mx - 25, my - 25, mx + 25, my + 25], fill=color,
              outline=(120, 120, 120), width=2)
    center(d, mx, my - 12, name, bold(19), WHITE)
    if note:
        bb_ = d.textbbox((0, 0), "長い足", font=bold(14))
        d.text((x1 - 22 - (bb_[2] - bb_[0]), y - 9), "長い足",
               font=bold(14), fill=(200, 70, 60))

def resistor(d, col_a, col_b, row, label="1kΩ",
             bands=((124, 74, 32), (38, 38, 40), (205, 42, 36)), note=True):
    """抵抗。足を直角に折り曲げて、はなれた2列にさす"""
    x1, x2, y = cx(col_a), cx(col_b), cy(row)
    ty = y - 16                                  # 折り曲げた高さ
    d.line([x1, y, x1, ty], fill=(176, 180, 182), width=5)
    d.line([x2, y, x2, ty], fill=(176, 180, 182), width=5)
    d.line([x1, ty, x2, ty], fill=(176, 180, 182), width=5)
    mx = (x1 + x2) / 2
    hw = max(40, (x2 - x1) / 2 - 16)
    d.rounded_rectangle([mx - hw, ty - 12, mx + hw, ty + 12], radius=6,
                        fill=(222, 198, 158), outline=(178, 150, 110), width=2)
    for i, bc in enumerate(bands):
        d.rectangle([mx - 22 + i * 15, ty - 12, mx - 14 + i * 15, ty + 12], fill=bc)
    if note:
        center(d, mx, ty - 34, label, bold(15), (120, 100, 70))

def resistor_v(d, col, row_a, row_b, label="1kΩ",
               bands=((124, 74, 32), (38, 38, 40), (205, 42, 36)), note=True):
    """たて向きの抵抗。同じ列で、main の行と 電源レール をつなぐときに使う"""
    x, y1, y2 = cx(col), cy(row_a), cy(row_b)
    d.line([x, y1, x, y2], fill=(176, 180, 182), width=5)
    my = (y1 + y2) / 2
    d.rounded_rectangle([x - 13, my - 40, x + 13, my + 40], radius=6,
                        fill=(222, 198, 158), outline=(178, 150, 110), width=2)
    for i, bc in enumerate(bands):
        d.rectangle([x - 13, my - 22 + i * 15, x + 13, my - 14 + i * 15], fill=bc)
    if note:
        d.text((x + 20, my - 10), label, font=bold(15), fill=(120, 100, 70))

def tact(d, col_a, col_b):
    """大型タクトスイッチ。端子は d行 と g行。
    同じ行の端子どうしは いつもつながっていて、押すと d と g がつながる。"""
    x1, x2 = cx(col_a), cx(col_b)
    yg, yd = cy("g"), cy("d")
    d.rounded_rectangle([x1 - 26, yg - 26, x2 + 26, yd + 26], radius=8,
                        fill=(46, 50, 54), outline=(30, 34, 38), width=2)
    mx, my = (x1 + x2) / 2, (yg + yd) / 2
    d.ellipse([mx - 52, my - 52, mx + 52, my + 52], fill=(70, 74, 78))
    d.ellipse([mx - 44, my - 44, mx + 44, my + 44], fill=(222, 60, 48),
              outline=(170, 40, 32), width=3)
    center(d, mx, my - 13, "おす", bold(21), WHITE)
    for x in (x1, x2):
        for y in (yg, yd):
            d.ellipse([x - 10, y - 10, x + 10, y + 10], fill=GOLD,
                      outline=(180, 150, 40), width=2)
    center(d, mx, yg - 62, "g行どうしは いつもつながっている", bold(15), (120, 128, 132))
    center(d, mx, yd + 40, "d行どうしも いつもつながっている", bold(15), (120, 128, 132))

def vr(d, col, row):
    """可変抵抗（つまみ）。上から見ると正方形で、3行ぶんの大きさ。
    端子は3本、同じ行のとなりあう3列。まん中がワイパー。
    本体は端子の行から、溝がわへ 3行ぶん のびる。"""
    y, cxm = cy(row), cx(col + 1)
    S = PITCH * 3                       # 3行ぶん＝正方形
    x1, y1 = cxm - S / 2, y - 14
    d.rounded_rectangle([x1, y1, x1 + S, y1 + S], radius=8,
                        fill=(44, 92, 198), outline=(28, 62, 150), width=3)
    mx, my = cxm, y1 + S / 2
    d.ellipse([mx - 38, my - 38, mx + 38, my + 38], fill=(96, 140, 226),
              outline=(28, 62, 150), width=3)
    d.line([mx + 24, my, mx - 24, my], fill=WHITE, width=8)
    d.polygon([(mx - 34, my), (mx - 18, my - 12), (mx - 18, my + 12)], fill=WHITE)
    for i in range(3):                  # 端子は本体の上にのせて見えるように
        x = cx(col + i)
        d.ellipse([x - 10, y - 10, x + 10, y + 10], fill=GOLD,
                  outline=(180, 150, 40), width=3)
    d.text((x1 + S + 14, my - 28), "まん中が", font=bold(15), fill=(60, 90, 170))
    d.text((x1 + S + 14, my - 6), "ワイパー", font=bold(15), fill=(60, 90, 170))

def buzzer(d, x, y, r=92):
    """圧電ブザー。赤・黒のリード線がついている。（ボードの外に置く）"""
    d.ellipse([x - r, y - r, x + r, y + r], fill=(38, 40, 44),
              outline=(22, 24, 26), width=3)
    d.ellipse([x - r + 26, y - r + 26, x + r - 26, y + r - 26],
              fill=(56, 58, 62), outline=(28, 30, 32), width=2)
    d.ellipse([x - 22, y - 22, x + 22, y + 22], fill=(226, 214, 120),
              outline=(180, 168, 80), width=2)
    center(d, x, y + r + 12, "ブザー", bold(20), INK)

def lead(d, x1, y1, a, col, w=8):
    """ブザーなどのリード線。(x1,y1) から 穴 a=(列,行) へ"""
    x2, y2 = cx(a[0]), cy(a[1])
    pts = []
    for i in range(31):
        s = i / 30
        pts.append((x1 + (x2 - x1) * s,
                    y1 + (y2 - y1) * s - 18 * (4 * s * (1 - s))))
    d.line(pts, fill=WHITE, width=w + 8, joint="curve")
    d.line(pts, fill=col, width=w, joint="curve")
    d.ellipse([x2 - 7, y2 - 7, x2 + 7, y2 + 7], fill=col)

def to92(d, col, row, labels=("VDD", "OUT", "GND"), name="温度センサー"):
    """TO-92 の部品（温度センサー）。足は3本、となりあう3列。片面が平ら。"""
    x1, x3, y = cx(col), cx(col + 2), cy(row)
    mx = (x1 + x3) / 2
    d.pieslice([mx - 42, y - 60, mx + 42, y + 6], 180, 360,
               fill=(48, 50, 54), outline=(28, 30, 32), width=2)
    d.rectangle([mx - 42, y - 28, mx + 42, y - 8], fill=(48, 50, 54))
    d.line([mx - 42, y - 28, mx - 42, y - 8], fill=(130, 134, 138), width=5)
    d.text((mx + 52, y - 46), name, font=bold(15), fill=INK)
    d.text((mx + 52, y - 22), "平らな面を手前に", font=reg(13), fill=GRAY)
    for i, lab in enumerate(labels):
        x = cx(col + i)
        d.line([x, y - 10, x, y], fill=(176, 180, 182), width=4)
        d.ellipse([x - 10, y - 10, x + 10, y + 10], fill=GOLD,
                  outline=(180, 150, 40), width=3)
        center(d, x, y + 16, lab, bold(14), INK)

def cds(d, col, row):
    """CdS（光センサー）。足は2本、となりあう2列。向きはない。"""
    x1, x2, y = cx(col), cx(col + 1), cy(row)
    mx, my = (x1 + x2) / 2, y - 34
    for x in (x1, x2):
        d.line([x, y, mx, my], fill=(176, 180, 182), width=5)
    d.ellipse([mx - 28, my - 28, mx + 28, my + 28], fill=(236, 228, 190),
              outline=(150, 140, 100), width=3)
    for k in range(-2, 3):
        d.line([mx - 18, my + k * 8, mx + 18, my + k * 8 - 4],
               fill=(120, 110, 70), width=3)
    center(d, mx, my + 34, "CdS", bold(15), (120, 100, 40))

def sr04(d, col, row, pins=("Vcc", "Trig", "Echo", "Gnd")):
    """HC-SR04（45×20mm）。端子4本を1行にさす。本体は端子の行から 溝がわへ のびる。
    実物は幅 17列ぶんもあるので、さした位置より ずっと左右に張り出す。"""
    y = cy(row)
    mx = (cx(col) + cx(col + 3)) / 2
    hw = 8.85 * PITCH                      # 45mm ＝ 約17.7列
    x1, x2, top = mx - hw, mx + hw, y + 18
    h = hw * 2 / 2.25                      # 20mm ぶんの高さ
    d.rounded_rectangle([x1, top, x2, top + h], radius=10,
                        fill=(38, 64, 140), outline=(24, 44, 100), width=3)
    for k in (0.245, 0.755):
        cxx, cyy, r = x1 + (x2 - x1) * k, top + h * 0.56, h * 0.36
        d.ellipse([cxx - r, cyy - r, cxx + r, cyy + r], fill=(178, 182, 186),
                  outline=(118, 122, 126), width=3)
        for q in (0.78, 0.56, 0.34, 0.14):
            rr = r * q
            d.ellipse([cxx - rr, cyy - rr, cxx + rr, cyy + rr],
                      outline=(128, 132, 136), width=2)
    center(d, mx, top + 12, "HC-SR04", bold(22), WHITE)
    for i, lab in enumerate(pins):
        x = cx(col + i)
        d.line([x, y, x, top], fill=(176, 180, 182), width=4)
        d.ellipse([x - 10, y - 10, x + 10, y + 10], fill=GOLD,
                  outline=(180, 150, 40), width=3)
        center(d, x, y - 30, lab, bold(14), INK)

def ext_module(d, x, y, w, h, title, pins, pin_col=GOLD):
    """ボードの外に置く部品（HC-SR04・サーボなど）。pins は端子名のならび。
    もどり値は 各端子の (x, y)。"""
    d.rounded_rectangle([x, y, x + w, y + h], radius=10, fill=(38, 64, 140),
                        outline=(24, 44, 100), width=3)
    center(d, x + w / 2, y + 14, title, bold(20), WHITE)
    out, step = [], w / (len(pins) + 1)
    for i, name in enumerate(pins, 1):
        px, py = x + step * i, y + h
        d.ellipse([px - 9, py - 9, px + 9, py + 9], fill=pin_col,
                  outline=(180, 150, 40), width=2)
        center(d, px, py - 34, name, bold(15), WHITE)
        out.append((px, py))
    return out

def servo(d, x, y, w=250, h=150):
    """サーボモーター（ボードの外に置く）。3本のリード線の付け根を返す"""
    d.rounded_rectangle([x, y, x + w, y + h], radius=8, fill=(40, 44, 48),
                        outline=(24, 26, 28), width=3)
    d.rounded_rectangle([x + w * 0.28, y - 34, x + w * 0.72, y + 6], radius=6,
                        fill=(56, 60, 64))
    d.ellipse([x + w / 2 - 22, y - 56, x + w / 2 + 22, y - 12], fill=(230, 232, 234))
    d.rectangle([x + w / 2 - 60, y - 40, x + w / 2 + 60, y - 28], fill=(230, 232, 234))
    center(d, x + w / 2, y + h / 2 - 12, "サーボ", bold(22), WHITE)
    d.rounded_rectangle([x - 62, y + h * 0.28, x - 10, y + h * 0.76], radius=5,
                        fill=(40, 42, 46), outline=(24, 26, 28), width=2)
    center(d, x - 36, y + h * 0.78, "コネクタ", bold(13), INK)
    return [(x - 62, y + h * (0.34 + 0.18 * i)) for i in range(3)]

def dip(d, col, top_pins, bot_pins, title="DRV8835", fill=(28, 110, 70)):
    """300mil の DIP モジュール。溝をまたいで f行・e行 に入る（6列ぶん）。
    端子名は本体の中に書く（まわりの配線と重ならないように）。"""
    n = len(top_pins)
    x1, x2 = cx(col) - 20, cx(col + n - 1) + 20
    y1, y2 = cy("f") - 26, cy("e") + 26
    d.rounded_rectangle([x1, y1, x2, y2], radius=8, fill=fill,
                        outline=(16, 80, 50), width=3)
    center(d, (x1 + x2) / 2, cy("g") - 32, title, bold(20), (20, 90, 58))
    for i, lab in enumerate(top_pins):
        x = cx(col + i)
        d.ellipse([x - 9, cy("f") - 9, x + 9, cy("f") + 9], fill=GOLD)
        center(d, x, cy("f") + 12, lab, bold(11), WHITE)
    for i, lab in enumerate(bot_pins):
        x = cx(col + i)
        d.ellipse([x - 9, cy("e") - 9, x + 9, cy("e") + 9], fill=GOLD)
        center(d, x, cy("e") - 26, lab, bold(11), WHITE)

def motor(d, x, y, r=76):
    d.ellipse([x - r, y - r, x + r, y + r], fill=(150, 154, 158),
              outline=(110, 114, 118), width=3)
    d.ellipse([x - 26, y - 26, x + 26, y + 26], fill=(90, 94, 98))
    center(d, x, y + r + 12, "モーター", bold(19), INK)

def battery(d, x, y, w=210, h=110):
    d.rounded_rectangle([x, y, x + w, y + h], radius=8, fill=(232, 120, 40),
                        outline=(180, 88, 24), width=3)
    center(d, x + w / 2, y + h / 2 - 12, "電池 6V", bold(21), WHITE)
    return (x, y + h * 0.32), (x, y + h * 0.68)

def new(title, extra_w=0):
    im, d = canvas(W + extra_w, H)
    center(d, (W + extra_w) / 2, 20, title, bold(30), INK)
    return im, d
