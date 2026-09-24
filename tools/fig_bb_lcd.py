# -*- coding: utf-8 -*-
"""m1 LCD 1602（I2C 変換基板つき）の配線図。2枚つくる。

  bb_lcd.png    … LCD だけ（文字を出す）
  bb_lcd_vr.png … つまみを足す（つまみの値を数で見る）

LCD は4本だけ：GND・VCC・SDA・SCL。
  SDA → 3番 ／ SCL → 4番。どちらも**下の行**のピンなので、線は a〜b 行から出す
  （上半分 f〜j から出すと、同じ列の上の行のピンにつながってしまう）。
  VCC・GND は 上の ＋−レール へ（ボードの 5V・GND をレールに出しておく）。
LCD は メス−オスのジャンパーワイヤーで、ブレッドボードに直接さす。
"""
import sys
sys.path.insert(0, "tools")
sys.stdout.reconfigure(encoding="utf-8")
from bb import *

RED, BLACK, YELLOW, GREEN, ORANGE = ((214, 60, 50), (40, 44, 48),
    (232, 200, 70), (90, 180, 110), (240, 150, 40))

def lcd1602(d, x, y, line1, line2, w=400, h=170):
    """LCD 1602 を表から見た形（実物の写真にあわせてある）。
    上辺に 16個の端子（1番が左）、画面は黒い枠の中。
    I2C の4本は、裏の変換基板から**左はし**に出ている（裏から見ると右はし）。
    端子の列を上にした向きなら、表から見ても裏から見ても
    **上から GND・VCC・SDA・SCL**（左右が入れかわるだけで、上下の順は同じ）。
    実物写真 content/m1_lcd/image/index/lcd_back.jpg で確認ずみ。
    もどり値は 4本のピンの (x, y)。"""
    d.rounded_rectangle([x, y, x + w, y + h], radius=8, fill=(30, 130, 60),
                        outline=(18, 90, 40), width=3)
    for i in range(16):                               # 上辺の16個の端子
        px = x + 44 + i * 13
        d.rectangle([px - 4, y + 6, px + 4, y + 16], fill=GOLD)
    # 黒い枠と画面（16文字×2行）
    bx1, by1, bx2, by2 = x + 70, y + 26, x + w - 18, y + h - 30
    d.rounded_rectangle([bx1, by1, bx2, by2], radius=6, fill=(34, 36, 40))
    sx1, sy1, sx2, sy2 = bx1 + 12, by1 + 14, bx2 - 12, by2 - 14
    d.rectangle([sx1, sy1, sx2, sy2], fill=(40, 60, 190))
    cw = (sx2 - sx1 - 12) / 16
    ch = (sy2 - sy1 - 16) / 2
    for r, text in enumerate((line1, line2)):
        for c in range(16):
            cx1 = sx1 + 6 + c * cw
            cy1 = sy1 + 5 + r * (ch + 6)
            d.rectangle([cx1 + 1, cy1, cx1 + cw - 2, cy1 + ch], fill=(58, 82, 214))
            if c < len(text):
                middle(d, cx1 + cw / 2, cy1 + ch / 2, text[c], monob(22), WHITE)
    center(d, (bx1 + bx2) / 2, by2 + 4, "LCD 1602", bold(16), WHITE)
    # I2C の4本（左はし）
    names = ["GND", "VCC", "SDA", "SCL"]
    out, step = [], h / (len(names) + 1)
    d.rectangle([x - 8, y + step * 0.6, x + 6, y + step * 4.4], fill=(34, 36, 40))
    for i, name in enumerate(names, 1):
        px, py = x - 1, y + step * i
        d.ellipse([px - 8, py - 8, px + 8, py + 8], fill=GOLD,
                  outline=(180, 150, 40), width=2)
        d.text((x + 12, py - 10), name, font=bold(14), fill=WHITE)
        out.append((px, py))
    return out

def draw(with_vr):
    title = "LCD とつまみをつなぐ" if with_vr else "LCD（I2C）をつなぐ"
    im, d = new(title, extra_w=480)
    board(d)
    mcu(d)

    # ボードの 5V・GND を 上の ＋−レールへ
    wire(d, (1, "j"), (1, "+t"), RED)
    wire(d, (2, "i"), (2, "-t"), BLACK)

    line2 = "1023" if with_vr else "0"
    # LCD は右下に置き、SDA・SCL のピンの高さを b行・a行 にそろえる。
    # こうすると2本は ボード本体の下（あいている a・b 行）を通り、ピン名を隠さない。
    gnd, vcc, sda, scl = lcd1602(d, 1296, cy("b") - 102, "Hello!", line2)
    lead(d, gnd[0], gnd[1], (25, "-t"), BLACK)   # 赤と重ならないよう列を離す
    lead(d, vcc[0], vcc[1], (29, "+t"), RED)
    lead(d, sda[0], sda[1], (5, "b"), YELLOW)     # SDA → 3番
    lead(d, scl[0], scl[1], (6, "a"), GREEN)      # SCL → 4番

    if with_vr:                                   # つまみ（b4 と同じ形）
        vr(d, 21, "h")
        wire(d, (21, "i"), (21, "+t"), RED)       # 左はし → ＋レール（5V）
        wire(d, (6, "j"), (22, "j"), ORANGE)      # A2    → まん中
        wire(d, (23, "j"), (23, "-t"), BLACK)     # 右はし → −レール

    center(d, 1496, cy("b") - 170, "LCD はメス−オスの", reg(17), GRAY)
    center(d, 1496, cy("b") - 146, "ジャンパーワイヤーで直接さす", reg(17), GRAY)
    save(im, "m1_lcd", "bb_lcd_vr.png" if with_vr else "bb_lcd.png")

draw(False)
draw(True)
