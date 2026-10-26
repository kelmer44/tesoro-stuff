#!/usr/bin/env python3
"""Run Jim's click walks through the production scene with its path (kind 8) on every room.

Uses the local ignored driver local-media/jim-implementation/detour_driver.cpp. Start points are
inside the floor, targets anywhere around it; reports how many walks ended at their target, how
many stopped short (no route), and any that never ended. A smoke test only: the 'inside' columns use a
generic point-in-polygon, which differs from the original's x-monotone test on some floors, so only
`never_ended` is a pass criterion. The route itself is compared with the original by compare_path_route.py.
"""
import argparse, json, random, struct, subprocess
from pathlib import Path
from compare_floor_scale import floors
from compare_jim_walk import load_banks, BANKS


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--game', type=Path, required=True)
    p.add_argument('--driver', type=Path, required=True)
    p.add_argument('--output', type=Path, required=True)
    p.add_argument('--walks', type=int, default=30)
    p.add_argument('--verbose', action='store_true')
    p.add_argument('--seed', type=int, default=1)
    a = p.parse_args()
    rng = random.Random(a.seed)
    raw, _ = load_banks(a.game)
    req = b''.join(struct.pack('<I', len(raw[n])) + raw[n] for n in BANKS)
    items = [f for f in floors(a.game) if f[2] and f[2]['points']]
    req += struct.pack('<I', len(items))
    plan = []
    for name, scene, floor in items:
        xs = [floor['x'] + q[0] for q in floor['points']]
        ys = [floor['y'] + q[1] for q in floor['points']]
        cx, cy = sum(xs) // len(xs), sum(ys) // len(ys)
        walks = []
        poly = [(floor['x'] + q[0], floor['y'] + q[1]) for q in floor['points']]

        def inside(px, py):
            c = False
            for i in range(len(poly)):
                (x1, y1), (x2, y2) = poly[i], poly[i - 1]
                if (y1 > py) != (y2 > py) and px < (x2 - x1) * (py - y1) / (y2 - y1) + x1:
                    c = not c
            return c
        while len(walks) < a.walks:
            sx, sy = rng.randint(min(xs), max(xs)), rng.randint(min(ys), max(ys))
            if not inside(sx, sy):
                continue
            tx, ty = rng.randint(min(xs) - 150, max(xs) + 150), rng.randint(min(ys) - 100, max(ys) + 100)
            walks.append((sx, sy, tx, ty))
        plan.append((name, walks, poly))
        req += struct.pack('<I', len(scene)) + scene + struct.pack('<I', len(walks))
        for sx, sy, tx, ty in walks:
            req += struct.pack('<iiiiII', sx, sy, tx, ty, 1000, 3000)
    out = subprocess.run([str(a.driver)], input=req, capture_output=True, timeout=1800)
    if out.returncode:
        raise SystemExit(out.stderr.decode()[-2000:])
    if a.verbose: print('\n'.join(l for l in out.stderr.decode().splitlines() if l.startswith('step'))[:4000])
    vals = struct.unpack('<%di' % (len(out.stdout) // 4), out.stdout)
    report, at, number = [], 0, 0
    for name, walks, poly in plan:
        hit = short = never = in_hit = in_total = 0
        for sx, sy, tx, ty in walks:
            x, y, arrived, done = vals[at:at + 4]; at += 4; number += 1
            c = False
            for i in range(len(poly)):
                (x1, y1), (x2, y2) = poly[i], poly[i - 1]
                if (y1 > ty) != (y2 > ty) and tx < (x2 - x1) * (ty - y1) / (y2 - y1) + x1:
                    c = not c
            if c:
                in_total += 1
                in_hit += abs(x - tx) <= 1 and abs(y - ty) <= 1
                if a.verbose and not (abs(x - tx) <= 1 and abs(y - ty) <= 1) and name.startswith('Puerto'): print('MISS', number, name, (sx, sy), (tx, ty), (x, y), arrived, done)
            if not arrived: never += 1
            elif abs(x - tx) <= 1 and abs(y - ty) <= 1: hit += 1
            else:
                short += 1
                if a.verbose and short <= 3: print(name, (sx, sy), (tx, ty), (x, y), arrived, done)
        report.append(dict(scene=name, walks=len(walks), at_target=hit, stopped_short=short, never_ended=never, targets_inside=in_total, inside_reached=in_hit))
    a.output.parent.mkdir(parents=True, exist_ok=True)
    a.output.write_text(json.dumps(report, indent=1) + '\n')
    tot = {k: sum(r[k] for r in report) for k in ('walks', 'at_target', 'stopped_short', 'never_ended', 'targets_inside', 'inside_reached')}
    print(json.dumps(tot))

main()
