#!/usr/bin/env python3
"""Compares two window captures (original in DOSBox-X, engine in ScummVM) after cutting off their title bars and finding the best small shift.
Usage: re/.venv/bin/python re/reference/compare.py <original.png> <engine.png> [title-bar height of the first, of the second]   (default 57 and 64 pixels at 2x)
Prints the mean difference (three channels summed) and the share of pixels that differ by more than 60 in an inner window; writes <engine>-diff.png.
Both windows scale the 640x480 picture differently, so small values (mean under about 10, a few percent of edge pixels) mean the same picture."""
import sys
import numpy as np
from PIL import Image
a = np.asarray(Image.open(sys.argv[1]).convert("RGB")).astype(int); b = np.asarray(Image.open(sys.argv[2]).convert("RGB")).astype(int)
ta, tb = (int(sys.argv[3]), int(sys.argv[4])) if len(sys.argv) > 4 else (57, 64)
h = min(a.shape[0] - ta, b.shape[0] - tb) - 20; w = min(a.shape[1], b.shape[1]) - 20
best = None
for dy in range(-8, 9):
    for dx in range(-3, 4):
        A = a[ta + 10:ta + 10 + h, 10:10 + w]
        yb = tb + 10 + dy
        B = b[yb:yb + h, 10 + dx:10 + dx + w]
        if A.shape != B.shape or yb < 0: continue
        d = np.abs(A - B).sum(axis=2)
        if best is None or d.mean() < best[0]: best = (d.mean(), dy, dx, (d > 60).mean(), d)
print("best shift dy=%d dx=%d: mean difference %.2f, share of pixels differing by >60: %.4f" % (best[1], best[2], best[0], best[3]))
Image.fromarray(np.clip(best[4], 0, 255).astype("uint8")).save(sys.argv[2].replace(".png", "-diff.png"))
