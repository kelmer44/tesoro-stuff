#!/usr/bin/env python3
"""Reads files from the FAT16 partition of the reference disk image, without mounting it and without writing to it.
Usage: fatread.py <image> find <NAME-PATTERN>        lists matching files (path, size, date)
       fatread.py <image> ls <PATH>                   lists a folder (short 8.3 names, e.g. /PROGRA~1)
       fatread.py <image> cat <PATH> <out-file>       copies a file out
Windows may keep recent writes in its cache: shut Windows down (Start > Apagar) before reading files that were just saved.
Only short (8.3) names are read; long names are skipped."""
import fnmatch, struct, sys

class Fat16:
    def __init__(self, path):
        self.f = open(path, "rb")
        mbr = self.f.read(512)
        self.base = None
        for i in range(4):
            e = mbr[446 + 16 * i:462 + 16 * i]
            if e[4] in (4, 6, 14): self.base = struct.unpack("<I", e[8:12])[0] * 512; break
        if self.base is None: raise SystemExit("no FAT16 partition")
        self.f.seek(self.base); b = self.f.read(512)
        self.bps, self.spc, self.res, self.nfat, self.rootn = struct.unpack("<HBHBH", b[11:19])
        self.spf = struct.unpack("<H", b[22:24])[0]
        self.fat_off = self.base + self.res * self.bps
        self.root_off = self.fat_off + self.nfat * self.spf * self.bps
        self.data_off = self.root_off + self.rootn * 32
        self.csize = self.spc * self.bps
        self.f.seek(self.fat_off); self.fat = self.f.read(self.spf * self.bps)
    def chain(self, c):
        while 2 <= c < 0xfff0:
            yield c; c = struct.unpack("<H", self.fat[2 * c:2 * c + 2])[0]
    def read_clusters(self, c):
        out = b""
        for k in self.chain(c):
            self.f.seek(self.data_off + (k - 2) * self.csize); out += self.f.read(self.csize)
        return out
    def entries(self, c):
        raw = self.read_clusters(c) if c else (self.f.seek(self.root_off), self.f.read(self.rootn * 32))[1]
        for i in range(0, len(raw), 32):
            e = raw[i:i + 32]
            if e[0] == 0: break
            if e[0] == 0xe5 or e[11] == 0x0f or e[11] & 8: continue
            name = e[:8].decode("latin1").rstrip(); ext = e[8:11].decode("latin1").rstrip()
            full = name + ("." + ext if ext else "")
            clus = struct.unpack("<H", e[26:28])[0]; size = struct.unpack("<I", e[28:32])[0]
            d, t = struct.unpack("<HH", e[24:26] + e[22:24])
            yield full, bool(e[11] & 0x10), clus, size, "%04d-%02d-%02d %02d:%02d" % (1980 + (d >> 9), (d >> 5) & 15, d & 31, t >> 11, (t >> 5) & 63)
    def walk(self, c=0, path=""):
        for name, isdir, clus, size, when in self.entries(c):
            if name in (".", ".."): continue
            yield path + "/" + name, isdir, clus, size, when
            if isdir: yield from self.walk(clus, path + "/" + name)
    def lookup(self, path):
        c = 0; isdir = True; size = 0
        for part in [p for p in path.upper().split("/") if p]:
            for name, d, clus, sz, w in self.entries(c):
                if name.upper() == part: c, isdir, size = clus, d, sz; break
            else: raise SystemExit("not found: " + path)
        return c, isdir, size

if __name__ == "__main__":
    fs = Fat16(sys.argv[1]); cmd = sys.argv[2]
    if cmd == "find":
        for p, d, c, s, w in fs.walk():
            if fnmatch.fnmatch(p.split("/")[-1].upper(), sys.argv[3].upper()): print(p, "<dir>" if d else s, w)
    elif cmd == "ls":
        c, isdir, _ = fs.lookup(sys.argv[3])
        for name, d, clus, size, when in fs.entries(c): print(("<dir>" if d else str(size)).rjust(10), when, name)
    elif cmd == "cat":
        c, isdir, size = fs.lookup(sys.argv[3]); open(sys.argv[4], "wb").write(fs.read_clusters(c)[:size])
