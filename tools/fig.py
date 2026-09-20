#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""教材の説明図をつくるための共通部品。

教材の配色（ティール／オレンジ）にそろえてある。
日本語は Meiryo、等幅は MS ゴシック（Consolas は日本語を持たないため）。
"""
import os
from PIL import Image, ImageDraw, ImageFont

OUT = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), "content")

# ---- 色 -------------------------------------------------------------
INK        = (40, 48, 52)
TEAL       = (0, 169, 157)
DARKTEAL   = (0, 118, 110)
SOFT       = (228, 246, 244)
ORANGE     = (255, 159, 120)
DEEPORANGE = (230, 118, 40)
GOLD       = (240, 190, 60)
BLUE       = (52, 120, 200)
PALE       = (214, 226, 226)
GRAY       = (130, 140, 144)
RED        = (215, 60, 50)
WHITE      = (255, 255, 255)
CODEBG     = (246, 249, 249)

# ---- フォント -------------------------------------------------------
_F = r"C:\Windows\Fonts"
def _font(names, size):
    for n in names:
        p = os.path.join(_F, n)
        if os.path.exists(p):
            try:
                return ImageFont.truetype(p, size)
            except Exception:
                pass
    return ImageFont.load_default()

def bold(size):  return _font(["meiryob.ttc", "YuGothB.ttc", "msgothic.ttc"], size)
def reg(size):   return _font(["meiryo.ttc", "YuGothR.ttc", "msgothic.ttc"], size)
def mono(size):  return _font(["msgothic.ttc", "consola.ttf"], size)
def monob(size): return _font(["msgothic.ttc", "consolab.ttf"], size)

# ---- 描画のたすけ ---------------------------------------------------
def canvas(w, h, bg=WHITE):
    im = Image.new("RGB", (w, h), bg)
    return im, ImageDraw.Draw(im)

def center(d, cx, y, text, font, fill=INK):
    """cx を中心に、y を上端として描く"""
    b = d.textbbox((0, 0), text, font=font)
    d.text((cx - (b[2] - b[0]) / 2 - b[0], y), text, font=font, fill=fill)

def middle(d, cx, cy, text, font, fill=INK):
    """cx, cy を中心に描く（たて・よこ とも中央）"""
    b = d.textbbox((0, 0), text, font=font)
    d.text((cx - (b[2] - b[0]) / 2 - b[0], cy - (b[3] - b[1]) / 2 - b[1]),
           text, font=font, fill=fill)

def box(d, x, y, w, h, fill=SOFT, outline=TEAL, radius=12, width=3):
    d.rounded_rectangle([x, y, x + w, y + h], radius=radius,
                        fill=fill, outline=outline, width=width)

def arrow(d, x1, y1, x2, y2, fill=GRAY, width=4, head=12):
    """x1,y1 から x2,y2 へ矢印を引く"""
    import math
    d.line([x1, y1, x2, y2], fill=fill, width=width)
    a = math.atan2(y2 - y1, x2 - x1)
    for s in (+1, -1):
        d.line([x2, y2,
                x2 - head * math.cos(a + s * 0.45),
                y2 - head * math.sin(a + s * 0.45)], fill=fill, width=width)

def codebox(d, x, y, w, h, lines, font, lh=34, pad=20, bg=CODEBG):
    d.rounded_rectangle([x, y, x + w, y + h], radius=10, fill=bg,
                        outline=PALE, width=2)
    for i, ln in enumerate(lines):
        d.text((x + pad, y + pad + i * lh), ln, font=font, fill=INK)

def save(im, chapter, name):
    dst = os.path.join(OUT, chapter, "image", "index")
    os.makedirs(dst, exist_ok=True)
    p = os.path.join(dst, name)
    im.save(p)
    print("  \u2713 %s/%s  %s" % (chapter, name, im.size))
