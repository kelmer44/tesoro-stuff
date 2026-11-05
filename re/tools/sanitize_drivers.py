#!/usr/bin/env python3
"""Builds every comparison driver of run_startup_check.sh with AddressSanitizer and UndefinedBehaviorSanitizer and runs it on the existing vectors
(the vectors are not regenerated: run the suite first). Reports every sanitizer finding once, with the driver it came from.
Usage: re/.venv/bin/python re/tools/sanitize_drivers.py [name-filter]      Output: local-media/sanitizer/<driver>.log (ignored)
Needs: ENG (the engine worktree, default the Codex one)."""
import os, re, subprocess, sys
from concurrent.futures import ThreadPoolExecutor
os.chdir(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", ".."))
ENG = os.environ.get("ENG", "/Users/gabriel/Desktop/source/scummvm-tesoro-codex")
TESORO = os.environ.get("TESORO_DIR", "/Users/gabriel/Desktop/ags/TESORO")
script = open("re/tools/run_startup_check.sh").read().replace("\\\n", " ")
jobs = []; lines = [l.strip() for l in script.split("\n")]
for i, l in enumerate(lines):
    if l.startswith("c++ ") and " -o local-media/bin/" in l:
        name = re.search(r"-o local-media/bin/(\S+)", l).group(1)
        run = next((m for m in lines[i + 1:i + 4] if m.startswith("local-media/bin/" + name + " ") or m == "local-media/bin/" + name), None)
        if run: jobs.append((name, l, run))
flt = sys.argv[1] if len(sys.argv) > 1 else ""
jobs = [j for j in jobs if flt in j[0]]
os.makedirs("local-media/bin-san", exist_ok=True); os.makedirs("local-media/sanitizer", exist_ok=True)
SAN = "-fsanitize=address,undefined -fno-omit-frame-pointer -g"
env = dict(os.environ, ENG=ENG, TESORO_DIR=TESORO, ASAN_OPTIONS="detect_leaks=0:abort_on_error=0", UBSAN_OPTIONS="print_stacktrace=1")
def one(job):
    name, build, run = job
    # the build lines refer to $ENG and $C
    prefix = f'ENG="{ENG}"; C="$ENG/engines/tesoro/core"; '
    b = build.replace("c++ ", "c++ " + SAN + " ", 1).replace("-o local-media/bin/", "-o local-media/bin-san/")
    r = subprocess.run(prefix + b, shell=True, capture_output=True, text=True, env=env, executable="/bin/zsh")
    if r.returncode: return name, "BUILD FAILED: " + r.stderr[-300:], ""
    cmd = run.replace("local-media/bin/", "local-media/bin-san/")
    cmd = cmd.replace('"${TESORO_DIR:-/Users/gabriel/Desktop/ags/TESORO}"', f'"{TESORO}"')
    p = subprocess.run(prefix + cmd, shell=True, capture_output=True, text=True, env=env, executable="/bin/zsh", timeout=3000)
    log = p.stdout + "\n" + p.stderr
    open(f"local-media/sanitizer/{name}.log", "w").write(log)
    finds = sorted(set(re.findall(r"(?:runtime error: [^\n]*|ERROR: AddressSanitizer: [^\n]*)", log)))
    return name, "", finds
with ThreadPoolExecutor(max_workers=4) as ex:
    results = list(ex.map(one, jobs))
bad = 0
for name, err, finds in results:
    if err: print(f"{name}: {err}"); bad += 1
    elif finds:
        bad += 1; print(f"{name}: {len(finds)} finding(s)")
        for f in finds[:6]: print("   ", f[:230])
    else: print(f"{name}: clean")
print(f"{len(results)} drivers, {bad} with findings or build failures")
