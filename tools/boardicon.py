# -*- coding: utf-8 -*-
"""UIAPduino をデフォルメしたアイコン。hakase.png（いらすとや風）に合わせて
   やわらかい輪郭＋平らな塗り。4倍で描いて縮小し、ふちをなめらかにする。

向きは bb.py の配線図とそろえて USB を左に出す。
描くときは USB を右にしておき、最後に180度まわす（鏡像にすると
裏返った基板になってしまうため）。ランプの位置は点灯した実物の写真から。
"""
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
PWR   = (78, 146, 242)                  # PWR は青
LED_L = (246, 138, 46)                  # マイコンのとなりの L は だいだい
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
    # USB-C（基板のはしにのって外へ出っぱる。実物は たてが すこし長い）
    rr(d, [170, 43, 208, 91], 9, USB)
    rr(d, [194, 54, 203, 80], 4.5, (186, 192, 200), (162, 169, 178), 2.5)
    d.line(s([177, 83, 189, 83]), fill=(245, 247, 250), width=int(3 * S))
    # マイコン
    rr(d, [86, 50, 126, 86], 7, CHIP, (44, 48, 54))
    if not face:                                  # 顔のときは じゃまになる
        ci(d, 94, 58, 3, (126, 132, 140), None, 0)
    # リセットボタン
    rr(d, [38, 50, 74, 86], 9, BTN)
    ci(d, 56, 68, 10, GOLD, (219, 170, 62), 2.5)
    # ランプ2つ
    ci(d, 29, 52, 5.5, PWR,   (44, 104, 202), 2.5)   # PWR：USB から遠いはし寄り
    ci(d, 81, 92, 4.8, LED_L, (212, 104, 24), 2.5)   # L：マイコンの角

    im = im.transpose(Image.ROTATE_180)              # USB を左へ
    if face:                                         # 顔はまわしたあとに描く
        d = ImageDraw.Draw(im)
        for ex in (103, 118):
            ci(d, ex, 62, 5.4, WHITE, None, 0)
            ci(d, ex + 0.8, 63, 2.4, (36, 40, 45), None, 0)
        d.arc(s([104, 68, 117, 77]), 10, 170, fill=WHITE, width=int(2.4 * S))
    return im.resize((W * 2, H * 2), Image.LANCZOS)

for face, name in ((False, "board.png"), (True, "board-face.png")):
    im = draw(face)
    p = "docs/assets/" + name
    im.save(p)
    print("  \u2713 %s  %s" % (p, im.size))
