#!/usr/bin/env python3
"""Checks the reading of the DirectInput device classes (base 004035b0, keyboard 00403820/004038f0,
mouse 00403980/00403aa0, dtor 004037c0, IsReady 004036c0) against the original machine code in Unicorn.
DirectInput is faked: IDirectInput / IDirectInputDevice are vtables of Python stubs.
Measurement tool only. Run: re/.venv/bin/python re/tools/check_devices.py
"""
import random, struct, sys
from pathlib import Path
sys.path.insert(0, str(Path(__file__).resolve().parent))
from emu import Emu

ARENA = 0x03000000
class World:
    def __init__(self):
        e = self.e = Emu(); e.uc.mem_map(ARENA, 0x100000, 7); e.set_fs()
        self.brk = ARENA + 0x10000; self.log = []; self.state = None; self.fail_di = False
        self.fail = {}                  # device call name -> HRESULT to return
        self.acquired_ok = True        # GetDeviceState succeeds only after Acquire when False at start
        self.need_acquire = False
        e.hook_function(0x41ba10, self.new, 1)                      # operator_new (cdecl)
        e.hook_function(0x41a076, self.di_create, 4, pops=True)     # DirectInputCreateA (stdcall)
        e.stub_import(0x444360, lambda em, *a: self.log.append(("msgbox", em.read(a[1], 60).split(b"\0")[0])) or 1, 4)
        e.stub_import(0x44426c, lambda em: self.tick)
        self.tick = 1000
        # fake COM objects
        self.di = self.alloc(8); self.dev = self.alloc(8)
        self.di_vt = self.alloc(0x40); self.dev_vt = self.alloc(0x80)
        e.write(self.di, struct.pack("<I", self.di_vt)); e.write(self.dev, struct.pack("<I", self.dev_vt))
        def slot(vt, i, name, n, fn):
            tmp = self.alloc(4); e.stub_import(tmp, fn, n); e.write(vt + 4 * i, e.read(tmp, 4))
        slot(self.di_vt, 2, "di.Release", 1, lambda em, t: self.log.append("di.Release") or 0)
        slot(self.di_vt, 3, "CreateDevice", 4, self.create_device)
        slot(self.dev_vt, 2, "dev.Release", 1, lambda em, t: self.log.append("dev.Release") or 0)
        slot(self.dev_vt, 7, "Acquire", 1, self.acquire)
        slot(self.dev_vt, 8, "Unacquire", 1, lambda em, t: self.log.append("Unacquire") or 0)
        slot(self.dev_vt, 9, "GetDeviceState", 3, self.get_state)
        slot(self.dev_vt, 11, "SetDataFormat", 2, lambda em, t, f: self.log.append(("fmt", f)) or self.fail.get("fmt", 0))
        slot(self.dev_vt, 13, "SetCoop", 3, lambda em, t, h, f: self.log.append(("coop", h, f)) or self.fail.get("coop", 0))
    def alloc(self, n): a = self.brk; self.brk += (n + 15) & ~15; return a
    def new(self, em, n):
        a = self.alloc(n); em.write(a, bytes(n)); return a
    def di_create(self, em, hinst, ver, pp, outer):
        self.log.append(("create", hinst, ver, outer))
        if self.fail_di: return 0x80004005
        em.write(pp, struct.pack("<I", self.di)); return 0
    def create_device(self, em, t, guid, pp, outer):
        self.log.append(("createdev", em.read(guid, 16).hex()))
        if self.fail.get("dev"): return 1
        em.write(pp, struct.pack("<I", self.dev)); return 0
    def acquire(self, em, t):
        self.log.append("Acquire")
        if not self.fail.get("acq"): self.need_acquire = False
        return self.fail.get("acq", 0)
    def get_state(self, em, t, size, buf):
        self.log.append(("state", size))
        if self.need_acquire: return 0x8007000c
        em.write(buf, self.state[:size]); return 0

def rd(w, a, n=4): return struct.unpack("<I", w.e.read(a, 4))[0] if n == 4 else w.e.read(a, n)
def s32(x): return x - (1 << 32) if x & 0x80000000 else x
HW, HI = 0x1234, 0x5678
fails = 0
def check(cond, what):
    global fails
    if not cond: fails += 1; print("MISMATCH", what)

# --- constructors, IsReady, destructor ---
w = World(); e = w.e
kb = w.alloc(0x40); e.call(0x403820, [HW, HI, 6], conv="stdcall", this=kb)
check(rd(w, kb) == 0x428070, "kb vtable")
check(rd(w, kb + 4) == w.dev and rd(w, 0x442078) == w.di and rd(w, 0x44207c) == 1, "kb device/globals")
check(("createdev", "612b1d6fa0d5cf11bfc7444553540000") in w.log and ("fmt", 0x42c1b8) in w.log and ("coop", HW, 6) in w.log, "kb setup calls")
check(("create", HI, 0x700, 0) in w.log, "DirectInputCreate args")
check([rd(w, kb + o) for o in (0x18, 0xc, 0x10, 0x30)] == [16, 200, 20000, 5], "kb fields")
check(rd(w, kb + 0x34) == rd(w, kb + 0x1c) and rd(w, kb + 0x14) != 0, "kb arrays")
check(rd(w, kb + 0x20, 5) == bytes([1, 0x1c, 0x39, 0x1f, 0x26]), "kb key list")
check(e.call(0x4036c0, [], this=kb) & 0xff == 1, "ready")
ms = w.alloc(0x40); e.call(0x403980, [HW, HI, 5], conv="stdcall", this=ms)
check(rd(w, ms) == 0x428074 and rd(w, 0x44207c) == 2, "mouse vtable/refcount")
check([s32(rd(w, ms + o)) for o in (0x18, 0x20, 0x24, 0x28, 0x2c, 0x38, 0x3c, 0xc, 0x10)] ==
      [3, 0, 0, 640, 480, 320, 240, 300, 500], "mouse fields")
check(("createdev", "602b1d6fa0d5cf11bfc7444553540000") in w.log and ("fmt", 0x42c1d0) in w.log and ("coop", HW, 5) in w.log, "mouse setup calls")
w.log.clear(); e.call(0x4037c0, [], this=ms)
check(w.log == ["Unacquire", "dev.Release"] and rd(w, 0x44207c) == 1 and rd(w, ms + 4) == 0, "dtor 1")
check(e.call(0x4036c0, [], this=ms) & 0xff == 0, "not ready after dtor")
w.log.clear(); e.call(0x4037c0, [], this=kb)
check(w.log == ["Unacquire", "dev.Release", "di.Release"] and rd(w, 0x44207c) == 0 and rd(w, 0x442078) == 0, "dtor 2 releases DirectInput")
# failure paths
for what, setup, expect in (("di", lambda w: setattr(w, "fail_di", True), b"DIRECTINPUT: error al crear el objeto DirectInput"),
                            ("dev", lambda w: w.fail.update(dev=1), b"DIRECTINPUT: error al inicializar el dispositivo de entrada"),
                            ("fmt", lambda w: w.fail.update(fmt=1), b"DIRECTINPUT: error definiendo el formato del dispositivo de entrada"),
                            ("coop", lambda w: w.fail.update(coop=1), "cooperaci".encode())):
    w2 = World(); setup(w2); o = w2.alloc(0x40); w2.e.call(0x403820, [HW, HI, 6], conv="stdcall", this=o)
    m = [x for x in w2.log if isinstance(x, tuple) and x[0] == "msgbox"]
    check(len(m) == 1 and expect.split(b"\xf3")[0][:30] in m[0][1], "failure message " + what)
    ready = w2.e.call(0x4036c0, [], this=o) & 0xff
    check(ready == 0, "not ready after " + what)
    if what == "di": check(rd(w2, 0x44207c) == 0, "refcount untouched when DirectInput failed")

# --- keyboard update ---
rng = random.Random(7)
def kb_world():
    w = World(); k = w.alloc(0x40); w.e.call(0x403820, [HW, HI, 6], conv="stdcall", this=k); return w, k
def model_keys(scan, keys): return bytes((scan[k] >> 5) & 4 for k in keys)
w, k = kb_world(); keys = [1, 0x1c, 0x39, 0x1f, 0x26]
for i in range(300):
    scan = bytes(rng.choice([0, 0x80, 0xff, 0x7f, 0x20]) if rng.random() < .3 else 0 for _ in range(256))
    scan = bytearray(256)
    for j in keys: scan[j] = rng.choice([0, 0x80, 0xff, 0x7f, 0x20, 0xa0])
    w.state = bytes(scan); w.tick += rng.choice([1, 50, 250, 400, 30000])
    # reference: the shared state machine on the model's raw bytes, run by the original 00403700
    ref = World(); r = ref.alloc(0x40); ref.e.call(0x403820, [HW, HI, 6], conv="stdcall", this=r)
    ref.e.write(rd(ref, r + 0x1c, 0) if False else rd(ref, r + 0x1c), w.e.read(rd(w, k + 0x1c), 16))
    ref.e.write(rd(ref, r + 0x14), w.e.read(rd(w, k + 0x14), 64)); ref.tick = w.tick
    raw = ref.alloc(8); ref.e.write(raw, model_keys(scan, keys) + bytes(3)); ref.e.call(0x403700, [raw, 5], conv="stdcall", this=r)
    ret = w.e.call(0x4038f0, [], this=k) & 0xff
    check(ret == 1, "kb update returns 1")
    check(w.e.read(rd(w, k + 0x1c), 16) == ref.e.read(rd(ref, r + 0x1c), 16), f"kb states step {i}")
# acquire retry
w, k = kb_world(); w.state = bytes(256); w.need_acquire = True; w.log.clear()
check(w.e.call(0x4038f0, [], this=k) & 0xff == 1 and w.log[:3] == [("state", 256), "Acquire", ("state", 256)], "kb reacquire")
w.need_acquire = True; w.fail["acq"] = 1; w.log.clear()
w.need_acquire = True
w.log.clear(); w.state = bytes(256)
check(w.e.call(0x4038f0, [], this=k) & 0xff == 0, "kb returns 0 when the second read fails")
check(w.log == [("state", 256), "Acquire", ("state", 256)], "kb failed read sequence")
w.fail.pop("acq")

# --- mouse update (acquire fixed again) ---
w = World(); m = w.alloc(0x40); w.e.call(0x403980, [HW, HI, 5], conv="stdcall", this=m)
px, py = 320, 240
st = bytes(rd(w, m + 0x1c, 3))
for i in range(300):
    dx, dy, dz = rng.choice([0, 1, -1, 50, -400, 900, -900, 3000]), rng.choice([0, 7, -7, 300, -600, 700, 5000]), rng.randint(-3, 3)
    b = [rng.choice([0, 0x80, 0xff, 0x7f]) for _ in range(4)]
    w.state = struct.pack("<iii", dx, dy, dz) + bytes(b); w.tick += rng.choice([1, 100, 400, 700])
    ref = World(); r = ref.alloc(0x40); ref.e.call(0x403980, [HW, HI, 5], conv="stdcall", this=r); ref.tick = w.tick
    ref.e.write(rd(ref, r + 0x1c), w.e.read(rd(w, m + 0x1c), 3)); ref.e.write(rd(ref, r + 0x14), w.e.read(rd(w, m + 0x14), 12))
    raw = ref.alloc(8); ref.e.write(raw, bytes((x >> 5) & 4 for x in b[:3]) + b"\0"); ref.e.call(0x403700, [raw, 3], conv="stdcall", this=r)
    px = min(max(px + dx, 0), 640); py = min(max(py + dy, 0), 480)
    ret = w.e.call(0x403aa0, [], this=m) & 0xff
    got = (s32(rd(w, m + 0x38)), s32(rd(w, m + 0x3c)), s32(rd(w, m + 0x30)), s32(rd(w, m + 0x34)))
    check(ret == 1, "mouse returns 1")
    check(got == (px, py, dx, dy), f"mouse position step {i}: {got} vs {(px, py, dx, dy)}")
    check(w.e.read(rd(w, m + 0x1c), 3) == ref.e.read(rd(ref, r + 0x1c), 3), f"mouse buttons step {i}")
# centre helper and timings helper
w.e.write(m + 0x20, struct.pack("<iiii", -320, -240, 320, 240)); w.e.call(0x403a70, [], this=m)
check((s32(rd(w, m + 0x38)), s32(rd(w, m + 0x3c))) == (0, 0), "MouseCenter uses the current limits")
w.e.call(0x4036e0, [111, 222], conv="stdcall", this=m); check((rd(w, m + 0xc), rd(w, m + 0x10)) == (111, 222), "SetTimings")
print("FAILURES:", fails); sys.exit(1 if fails else 0)
