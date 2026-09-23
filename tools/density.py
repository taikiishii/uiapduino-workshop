# -*- coding: utf-8 -*-
"""見える幅（** や backtick を除く）で数え、吹き出しの連続行は1段落にまとめる。"""
import io, os, re, sys, unicodedata
sys.stdout.reconfigure(encoding="utf-8")
def vis(s):
    s = re.sub(r"\*\*|`|\[([^\]]*)\]\([^)]*\)", r"\1", s)
    return sum(2 if unicodedata.east_asian_width(c) in "WFA" else 1 for c in s)
def rows(block, bw, qw):
    tot, buf = 0, []
    def flush():
        nonlocal tot
        if buf: tot += max(1, -(-vis(" ".join(buf)) // qw)); buf.clear()
    for ln in block.split("\n"):
        s = ln.strip()
        if not s or "\U0001F4F7" in s: flush(); continue
        if s.startswith(">"): buf.append(s.lstrip("> ")); continue
        flush(); tot += max(1, -(-vis(s) // bw))
    flush(); return tot
only = sys.argv[1] if len(sys.argv) > 1 else None
bad = 0
for ch in sorted(os.listdir("content")):
    p = os.path.join("content", ch, "index.md")
    if ch.startswith("_") or not os.path.exists(p): continue
    if only and ch != only: continue
    for sl in re.split(r"\n[ \t]*-{3,}[ \t]*\n", io.open(p, encoding="utf-8").read()):
        h = sl.strip().split("\n")[0]
        if not h.startswith("# "): continue
        c = re.split(r"^[ \t]*:::[ \t]*$", sl, flags=re.M)
        r = rows(c[1], 36, 30) if len(c) == 3 else rows(sl, 72, 60)
        lim = 10 if len(c) == 3 else 13
        if only: print("  %-24s %2d/%d %s" % (h[2:24], r, lim, "OK" if r <= lim else "[!]"))
        if r > lim: bad += 1
print("  超過 %d件" % bad)
