#!/usr/bin/env python3
"""Local script smoke test. Starts every label of every script block of the game, one process each, in a sanitizer build of the engine that carries a
LOCAL-ONLY patch (TESORO_SMOKE=BLOCK:label:frames; never committed, see re/reference/README.md); runs 1200 frames in fast mode with the player skipping
lines, and reports warnings, sanitizer findings and crashes. Usage: smoke_scripts.py [engine binary] [name-filter]
Output: local-media/sanitizer/smoke/<block>-<label>.log (ignored) and a summary on stdout."""
import os, re, struct, subprocess, sys
from collections import Counter, defaultdict
from concurrent.futures import ThreadPoolExecutor
os.chdir(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", ".."))
BIN = sys.argv[1] if len(sys.argv) > 1 else "local-media/sanitizer/engine/scummvm"
FLT = sys.argv[2] if len(sys.argv) > 2 else ""
TESORO = os.environ.get("TESORO_DIR", "/Users/gabriel/Desktop/ags/TESORO")
tre = open(TESORO + "/TESORO.TRE", "rb").read(); dat = open(TESORO + "/TESORO.DAT", "rb")
ents = {}
for i in range(0, len(tre) - 27, 28):
    off, _, fl = struct.unpack_from("<IIB", tre, i)
    if fl & 4: ents[tre[i + 9:i + 28].split(b"\0")[0].decode("latin1").upper()] = off
pairs = []
for n in ("ANIMS.DLG", "CONVERS.DLG", "ICONS.DLG", "OBJETOS.DLG"):
    dat.seek(ents[n]); size = struct.unpack("<I", dat.read(4))[0]; text = dat.read(size).decode("latin1")
    block = None
    for line in text.split("\n"):
        line = line.strip()
        if line.startswith(".") and not line.startswith(".END"): block = line[1:].split()[0]
        elif line.startswith("_") and block: pairs.append((block, line[1:].split()[0]))
if FLT: pairs = [p for p in pairs if FLT in p[0]]
os.makedirs("local-media/sanitizer/smoke", exist_ok=True)
env = dict(os.environ, ASAN_OPTIONS="detect_leaks=0", UBSAN_OPTIONS="print_stacktrace=1", SDL_VIDEODRIVER="dummy", SDL_AUDIODRIVER="dummy")
IGNORE = ("SMOKE:", "translations.dat", "gui-icons.dat", "theme 'scummremastered'", "SMOKE: frame", "SMOKE: main list")
def one(pair):
    block, label = pair
    env2 = dict(env, TESORO_SMOKE=f"{block}:{label}:1200")
    try:
        cfg = os.path.abspath(f"local-media/sanitizer/cfg/{block}-{label}.ini")
        open(cfg, "w").write("[scummvm]\nenable_unsupported_game_warning=false\ngfx_mode=surfacesdl\n")
        p = subprocess.run([BIN, f"--config={cfg}", f"--path={TESORO}", "tesoro"], capture_output=True, text=True, env=env2, timeout=180, stdin=subprocess.DEVNULL)
        out, code = p.stdout + "\n" + p.stderr, p.returncode
    except subprocess.TimeoutExpired as ex:
        out, code = (ex.stdout or b"").decode("latin1", "replace") + "\n" + (ex.stderr or b"").decode("latin1", "replace"), 124
    open(f"local-media/sanitizer/smoke/{block}-{label}.log", "w").write(out)
    san = sorted(set(re.findall(r"(?:runtime error: [^\n]*|ERROR: AddressSanitizer: [^\n]*)", out)))
    warns = sorted(set(re.sub(r"\d+", "N", w) for w in re.findall(r"WARNING: ([^\n]*)", out) if not any(i in w for i in IGNORE)))
    started = "starts in level" in out
    return block, label, code, san, warns, started, "SMOKE: end after" in out
with ThreadPoolExecutor(max_workers=int(os.environ.get("JOBS", "3"))) as ex: res = list(ex.map(one, pairs))
crash = [r for r in res if r[3] or (r[2] not in (0, 124)) or not r[6]]
print(f"{len(res)} labels run; {sum(1 for r in res if r[5])} started in a level that holds their characters; {len(crash)} with crashes or sanitizer findings")
for b, l, code, san, warns, started, ended in crash: print(f"  CRASH {b}/{l}: exit {code}", san[:3], "(did not finish)" if not ended else "")
wc = Counter(); where = defaultdict(list)
for b, l, code, san, warns, started, ended in res:
    for w in warns: wc[w] += 1; where[w].append(f"{b}/{l}")
print("\nwarnings, by how many labels raise them:")
for w, n in wc.most_common(40): print(f"{n:4d}  {w[:150]}   e.g. {where[w][0]}")
print("\nlabels with no level holding their characters:", [f"{b}/{l}" for b, l, c, s, w, st, e in res if not st][:20])
