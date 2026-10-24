# -*- coding: utf-8 -*-
"""e5 サーボの配線図。**力の電気は ボードとは別の電源から**。図は USB 給電で描く
（本文は「電池でも別給電の USB でもよい」と書く。オーナー判断 2026-10-24）。

実物の写真（content/e5_servo/image/index/servo_photo.jpg）に合わせた形：
  ・ボードには、パソコンからの USB（左）。プログラムの書きこみと、ボードの電気
  ・サーボの電気は、別の電源から下の ＋−レールへ入れる。図は写真どおり、もう1本の USB を
    **USB-C の電源基板**にさした形。電池なら ＋ を下の ＋レール、− を下の −レールへ（2026-10-24）
  ・サーボのコネクタから、ジャンパーワイヤーで**直接**つなぐ
      緑（信号）→ 5番の列（8列・a行）／ 赤 → 下の ＋レール ／ 黒 → 下の −レール
      （緑を b 行にすると、線がボードの下の行のピン 8・9 の上を通ってかくしてしまう）
  ・ボードの GND（3列）も、同じ下の −レールへ（GND を共通にする）
サーボの線の色は 茶＝GND・赤＝＋・橙＝信号。ジャンパーは写真どおり 黒・赤・緑。
"""
import sys
sys.path.insert(0, "tools")
sys.stdout.reconfigure(encoding="utf-8")
from bb import *

RED, BLACK, GREEN = (214, 60, 50), (40, 44, 48), (40, 150, 120)

im, d = new("サーボをつなぐ（力の電気は USB 電源から）", extra_w=460)
board(d)
mcu(d)

wire(d, (3, "a"), (3, "-b"), BLACK)            # ボードの GND → 下の −レール

usb_power(d, 28)                               # USB-C の電源基板（下のレールの 28列）

# サーボ（ボードの外）。コネクタから直接つなぐ
sx, sy = 1290, 250
brown, red, orange = servo(d, sx, sy)
lead(d, brown[0],  brown[1],  (24, "-b"), BLACK)   # 茶 → 黒 → −レール
lead(d, red[0],    red[1],    (25, "+b"), RED)     # 赤 → 赤 → ＋レール
lead(d, orange[0], orange[1], (8, "a"), GREEN)     # 橙 → 緑 → 5番の列

save(im, "e5_servo", "bb_servo.png")
