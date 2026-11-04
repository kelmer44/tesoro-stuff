#!/usr/bin/env python3
"""Checks the reading of the sound device set-up against the original machine code in Unicorn:
CdAudioInit 00403b60 (MCI), DirectSoundInit 00403e00, Sound_IsReady 004040b0, Sound_Dtor 00404260.
MCI and DirectSound are faked (Python stubs, vtables in a mapped arena). Measurement tool only.
Run: re/.venv/bin/python re/tools/check_sound_init.py
"""
import random, struct, sys
from pathlib import Path
sys.path.insert(0, str(Path(__file__).resolve().parent))
from emu import Emu

ARENA = 0x03000000
HW = 0x1234
class World:
    def __init__(self, tracks=5, lengths=None, fail=()):
        e = self.e = Emu(); e.uc.mem_map(ARENA, 0x100000, 7); e.set_fs()
        self.brk = ARENA + 0x10000; self.log = []; self.fail = set(fail)
        self.tracks, self.lengths = tracks, lengths or [(0, 1)] * 200
        e.hook_function(0x41ba10, self.new, 1)
        e.hook_function(0x41be70, lambda em, p: self.log.append(("delete", p)), 1)
        e.hook_function(0x41b3b0, self.ds_create, 3, pops=True)
        e.stub_import(0x444360, lambda em, h, t, c, f: self.log.append(("msgbox", h, em.read(t, 80).split(b"\0")[0], f)) or 1, 4)
        e.stub_import(0x444390, lambda em, s, a, b, c: self.log.append(("mcistr", em.read(s, 20).split(b"\0")[0])) or (1 if "close" in self.fail else 0), 4)
        e.stub_import(0x444394, self.mci_cmd, 4)
        self.ds = self.alloc(8); self.pbuf = self.alloc(8); ds_vt = self.alloc(0x40); b_vt = self.alloc(0x80)
        e.write(self.ds, struct.pack("<I", ds_vt)); e.write(self.pbuf, struct.pack("<I", b_vt))
        self.sec = []
        def slot(vt, i, n, fn):
            t = self.alloc(4); e.stub_import(t, fn, n); e.write(vt + 4 * i, e.read(t, 4))
        slot(ds_vt, 2, 1, lambda em, t: self.log.append("ds.Release") or 0)
        slot(ds_vt, 3, 4, self.create_buffer)
        slot(ds_vt, 6, 3, lambda em, t, h, l: self.log.append(("coop", h, l)) or (1 if "coop" in self.fail else 0))
        slot(b_vt, 2, 1, lambda em, t: self.log.append(("buf.Release", t)) or 0)
        slot(b_vt, 14, 2, lambda em, t, w: self.log.append(("setformat", em.read(w, 18))) or (1 if "fmt" in self.fail else 0))
        self.b_vt = b_vt
    def alloc(self, n): a = self.brk; self.brk += (n + 15) & ~15; return a
    def new(self, em, n): a = self.alloc(n); em.write(a, bytes(n)); return a
    def ds_create(self, em, guid, pp, outer):
        self.log.append(("dscreate", guid))
        if "ds" in self.fail: return 1
        em.write(pp, struct.pack("<I", self.ds)); return 0
    def create_buffer(self, em, t, desc, pp, outer):
        d = em.read(desc, 36); self.log.append(("createbuf", d, em.read(struct.unpack("<I", d[16:20])[0], 18) if d[16:20] != bytes(4) else b""))
        if d[4] & 1:                                   # primary
            if "prim" in self.fail: return 1
            em.write(pp, struct.pack("<I", self.pbuf)); return 0
        if "sec%d" % (len(self.sec) + 1) in self.fail: self.sec.append(0); return 1
        b = self.alloc(8); em.write(b, struct.pack("<I", self.b_vt)); self.sec.append(b); em.write(pp, struct.pack("<I", b)); return 0
    def mci_cmd(self, em, dev, msg, flags, param):
        self.log.append(("mci", dev & 0xffffffff, msg, flags))
        if msg == 0x803:                               # open
            if "open" in self.fail: return 1
            em.write(param + 4, struct.pack("<I", 7)); self.opened = em.read(param, 12); return 0
        if msg == 0x814:                               # status
            item = struct.unpack("<I", em.read(param + 8, 4))[0]
            if item == 3:
                if "count" in self.fail: return 1
                em.write(param + 4, struct.pack("<I", self.tracks)); return 0
            trk = struct.unpack("<I", em.read(param + 0xc, 4))[0]
            m, s = self.lengths[trk - 1]; em.write(param + 4, bytes([m, s, 0, 0])); return 0
        return 0

def u32(w, a): return struct.unpack("<I", w.e.read(a, 4))[0]
def s32(x): return x - (1 << 32) if x & 0x80000000 else x
fails = 0
def check(c, what):
    global fails
    if not c: fails += 1; print("MISMATCH", what)

def cd(w):
    o = w.alloc(0x1c0); w.e.write(o, bytes(0x1c0)); w.e.call(0x403b60, [HW], conv="stdcall", this=o); return o
rng = random.Random(3)
# --- CD set-up ---
for tracks in (0, 1, 5, 20, 100, 101, 150):
    lengths = [(rng.randint(0, 79), rng.randint(0, 59)) for _ in range(200)]
    w = World(tracks, lengths); o = cd(w); n = min(tracks, 100)
    check(s32(u32(w, o + 0x1bc)) == n and u32(w, o + 0x14) == 7 and u32(w, o + 0x24) == HW, f"cd fields {tracks}")
    check([u32(w, o + 0x2c + 4 * i) for i in range(n)] == [m * 60 + s for m, s in lengths[:n]], f"cd lengths {tracks}")
    check(u32(w, o + 0x18) == 0x204 and ("mcistr", b"close all") in w.log, "cd open type")
    check(w.log[1] == ("mci", 0, 0x803, 0x3000) and w.log[2] == ("mci", 7, 0x814, 0x102), "cd command order")
    check([x for x in w.log if x[0] == "mci" and x[2] == 0x814 and x[3] == 0x112].__len__() == n, f"cd length queries {tracks}")
w = World(fail={"close"}); o = cd(w); check(s32(u32(w, o + 0x1bc)) == -1 and len(w.log) == 1, "close all failure leaves count -1, no box")
w = World(fail={"open"}); o = cd(w)
check(s32(u32(w, o + 0x1bc)) == -1 and s32(u32(w, o + 0x14)) == -1 and w.log[-1][:3] == ("msgbox", HW, b"Error al inicializar el CD"), "open failure")
w = World(fail={"count"}); o = cd(w)
check(s32(u32(w, o + 0x1bc)) == -1 and w.log[-2:] == [("msgbox", HW, b"Error al leer el CD", 0x30), ("mci", 7, 0x804, 0)], "count failure closes the device")

# --- DirectSound set-up ---
def snd(w):
    d = w.alloc(0x20); w.e.write(d, bytes(0x20)); w.e.call(0x403e00, [HW, 0x77], conv="stdcall", this=d); return d
w = World(); d = snd(w)
check(u32(w, d) == w.pbuf and u32(w, d + 4) == w.ds and u32(w, d + 0x14) == 0 and u32(w, d + 0x18) == 0, "ds fields")
recs = [u32(w, d + 8 + 4 * i) for i in range(3)]
check(all(u32(w, r) == b for r, b in zip(recs, w.sec)) and all(u32(w, r + 0x2c) == 0 for r in recs), "records")
check(u32(w, d + 0x1c) != 0 and ("dscreate", 0x77) in w.log and ("coop", HW, 2) in w.log, "cd object and DirectSound create")
prim = [x for x in w.log if x[0] == "createbuf"][0]
check(struct.unpack("<II", prim[1][:8]) == (0x24, 1), "primary buffer description")
check(w.log[[x[0] if isinstance(x, tuple) else x for x in w.log].index("setformat")][1] == struct.pack("<HHIIHHH", 1, 2, 22050, 88200, 4, 16, 0), "primary format 22050 Hz 16-bit stereo")
secs = [x for x in w.log if x[0] == "createbuf"][1:]
check(len(secs) == 3 and all(struct.unpack("<IIII", s[1][:16]) == (0x24, 0x100c0, 0x5622, 0) for s in secs), "secondary descriptions")
check(all(s[2] == struct.pack("<HHIIHHH", 1, 1, 22050, 22050, 1, 8, 0) for s in secs), "secondary format 22050 Hz 8-bit mono")
check(w.e.call(0x4040b0, [], this=d) & 0xff == 1, "ready")
# failures
for what, msg in (("ds", b"DIRECTSOUND: error al crear el objeto"), ("coop", b"DIRECTSOUND: error de nivel"),
                  ("prim", b"DIRECTSOUND: error al crear el buffer"), ("fmt", b"DIRECTSOUND: error definiendo")):
    w = World(fail={what}); d = snd(w); boxes = [x for x in w.log if x[0] == "msgbox"]
    check(len(boxes) == 1 and boxes[0][2].startswith(msg) and boxes[0][1] == HW, "failure box " + what)
    check(w.e.call(0x4040b0, [], this=d) & 0xff == 0, "not ready after " + what)
    check(not any(x[0] == "createbuf" and not (x[1][4] & 1) for x in w.log if x[0] == "createbuf"), "no secondary buffers after " + what)
w = World(fail={"sec2"}); d = snd(w); r = [u32(w, d + 8 + 4 * i) for i in range(3)]
check(u32(w, r[1]) == 0 and u32(w, r[0]) != 0 and u32(w, r[2]) != 0 and w.e.call(0x4040b0, [], this=d) & 0xff == 1, "one failed secondary buffer leaves a null record, device still ready")
w = World(fail={"count"}); d = snd(w)
check(u32(w, d + 4) == 0 and not any(x[0] == "dscreate" for x in w.log if isinstance(x, tuple)) and w.e.call(0x4040b0, [], this=d) & 0xff == 0, "unreadable CD: DirectSound never created, not ready")
# --- destructor ---
w = World(); d = snd(w); cdo = u32(w, d + 0x1c); w.log.clear(); w.e.call(0x404260, [], this=d)
check(w.log == [("mci", 7, 0x808, 0), ("mci", 7, 0x804, 0), ("delete", cdo), ("buf.Release", w.pbuf)], "dtor order")
check(w.e.read(cdo + 0x28, 1) == b"\0", "dtor clears playing flag")
w = World(fail={"open"}); d = snd(w)
w2 = World(); d2 = w2.alloc(0x20); o = w2.alloc(0x1c0); w2.e.write(o + 0x14, struct.pack("<i", -1)); w2.e.write(d2, struct.pack("<I", w2.pbuf)); w2.e.write(d2 + 0x1c, struct.pack("<I", o))
w2.e.call(0x404260, [], this=d2); check(w2.log == [("delete", o), ("buf.Release", w2.pbuf)], "dtor skips MCI when no device is open")
print("FAILURES:", fails); sys.exit(1 if fails else 0)
