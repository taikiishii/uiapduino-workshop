# -*- coding: utf-8 -*-
"""UIAPduino をデフォルメしたアイコン。hakase.png（いらすとや風）に合わせて
   やわらかい輪郭＋平らな塗り。4倍で描いて縮小し、ふちをなめらかにする。"""
import sys
from PIL import Image, ImageDraw
sys.stdout.reconfigure(encoding="utf-8")

S = 4                                   # 拡大して描いてから縮める
W, H = 216, 134
LINE  = (181, 181, 182)
BOARD = (253, 251, 245)
GOLD  = (240, 196, 86)
CHIP  = (66, 71, 78)
USB   = (216, 220, 226)
BTN   = (240, 240, 238)
LED   = (116, 212, 158)
WHITE = (255, 255, 255)

def s(v):  return [int(round(x * S)) for x in v]

def rr(d, box, r, fill, outline=LINE, w=3):
    d.rounded_rectangle(s(box), radius=int(r * S), fill=fill,
                        outline=outline, width=int(w * S))

def ci(d, cx, cy, r, fill, outline=LINE, w=3):
    d.ellipse(s([cx - r, cy - r, cx + r, cy + r]), fill=fill,
              outline=outline, width=int(w * S))

def draw(face):
    im = Image.new("RGBA", (W * S, H * S), (0, 0, 0, 0))
    d = ImageDraw.Draw(im)
    # 基板
    rr(d, [10, 26, 200, 108], 14, BOARD)
    # 金色のランド（上下のふちに またがる）
    for i in range(8):
        x = 30 + i * 21
        for edge in (26, 108):
            rr(d, [x - 8, edge - 8, x + 8, edge + 8], 8, GOLD)
    # USB-C（基板の右はしにのって、外へ出っぱる）
    rr(d, [170, 43, 208, 91], 9, USB)          # 実物は たてが すこし長い
    rr(d, [194, 54, 203, 80], 4.5, (186, 192, 200), (162, 169, 178), 2.5)
    d.line(s([177, 51, 189, 51]), fill=(245, 247, 250), width=int(3 * S))
    # マイコン
    rr(d, [86, 50, 126, 86], 7, CHIP, (44, 48, 54))
    if face:
        for ex in (99, 114):
            ci(d, ex, 64, 5.4, WHITE, None, 0)
            ci(d, ex + 0.8, 65, 2.4, (36, 40, 45), None, 0)
        d.arc(s([100, 70, 113, 79]), 10, 170, fill=WHITE, width=int(2.4 * S))
    else:
        ci(d, 94, 58, 3, (126, 132, 140), None, 0)
    # リセットボタン
    rr(d, [38, 50, 74, 86], 9, BTN)
    ci(d, 56, 68, 10, GOLD, (219, 170, 62), 2.5)
    # 電源ランプ
    ci(d, 24, 92, 5, LED, (74, 178, 126), 2.5)
    return im.resize((W * 2, H * 2), Image.LANCZOS)

for face, name in ((False, "board.png"), (True, "board-face.png")):
    im = draw(face)
    p = "docs/assets/" + name
    im.save(p)
    print("  \u2713 %s  %s" % (p, im.size))
