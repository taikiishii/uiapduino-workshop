#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""この教材をビルドする（content/ → docs/）。

    python build.py

しくみの本体は workshop-kit（となりのフォルダ）にあり、すべての
ワークショップで共有している。このファイルは呼び出すだけ。
教材ごとのちがいは site.toml に書く。
"""
import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))

try:
    import wskit
except ImportError:
    # pip install -e ../workshop-kit をしていないとき用の保険。
    # となりに workshop-kit フォルダがあれば、それを直接使う。
    sys.path.insert(0, os.path.join(HERE, "..", "workshop-kit"))
    try:
        import wskit
    except ImportError:
        sys.exit("workshop-kit が見つかりません。\n"
                 "  となりのフォルダに git clone してから、次を1度だけ実行してください:\n"
                 "    pip install -e ../workshop-kit")

wskit.build(HERE)
